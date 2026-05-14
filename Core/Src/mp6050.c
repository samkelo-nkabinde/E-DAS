/*
 * mp6050.c
 *
 *  Created on: May 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "main.h"
#include "mp6050.h"

#define MPU6050_SCL_PORT GPIOB
#define MPU6050_SCL_PIN  GPIO_PIN_8

#define MPU6050_SDA_PORT GPIOB
#define MPU6050_SDA_PIN  GPIO_PIN_9

#define MPU6050_ADDR_LOW   (0x68 << 1)
#define MPU6050_ADDR_HIGH  (0x69 << 1)

static uint16_t mpu6050_addr = MPU6050_ADDR_LOW;

/* MPU-6050 registers */
#define REG_SMPLRT_DIV    0x19
#define REG_CONFIG        0x1A
#define REG_GYRO_CONFIG   0x1B
#define REG_ACCEL_CONFIG  0x1C
#define REG_ACCEL_XOUT_H  0x3B
#define REG_TEMP_OUT_H    0x41
#define REG_PWR_MGMT_1    0x6B
#define REG_WHO_AM_I      0x75

/*
 * AFS_SEL = 0 gives +/- 2g.
 * The MPU-6050 register map gives 16384 LSB/g for this setting.
 */
#define ACCEL_LSB_PER_G   16384.0f


MPU6050_AccelData acceleration;

bool MPU6050_ready = false;

static MPU6050_Calibration accel_cal =
{
    .mx = 1.0f, .my = 1.0f, .mz = 1.0f,
    .cx = 0.0f, .cy = 0.0f, .cz = 0.0f
};

static uint8_t MPU6050_WriteReg(uint8_t reg, uint8_t value)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(
        &hi2c1,
        mpu6050_addr,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        &value,
        1,
        100
    );

    if (status == HAL_OK)
    {
        return MPU6050_OK;
    }

    return MPU6050_ERROR;
}

static uint8_t MPU6050_ReadRegs(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    HAL_StatusTypeDef status;

    if (buffer == 0 || length == 0)
    {
        return MPU6050_ERROR;
    }

    status = HAL_I2C_Mem_Read(
        &hi2c1,
        mpu6050_addr,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        buffer,
        length,
        100
    );

    if (status == HAL_OK)
    {
        return MPU6050_OK;
    }

    return MPU6050_ERROR;
}

uint8_t MPU6050_ReadWhoAmI(uint8_t *who)
{
    if (who == 0)
    {
        return MPU6050_ERROR;
    }

    mpu6050_addr = MPU6050_ADDR_LOW;

    if (MPU6050_ReadRegs(REG_WHO_AM_I, who, 1) == MPU6050_OK)
    {
        return MPU6050_OK;
    }

    mpu6050_addr = MPU6050_ADDR_HIGH;

    return MPU6050_ReadRegs(REG_WHO_AM_I, who, 1);
}

uint8_t MPU6050_ReadAccel(MPU6050_AccelData *data)
{
    uint8_t raw[6];

    if (data == 0)
    {
        return MPU6050_ERROR;
    }

    /*
     * Burst read from ACCEL_XOUT_H.
     * This reads X, Y, and Z from the same sample instant.
     */
    if (MPU6050_ReadRegs(REG_ACCEL_XOUT_H, raw, 6) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    data->x_raw = (int16_t)((raw[0] << 8) | raw[1]);
    data->y_raw = (int16_t)((raw[2] << 8) | raw[3]);
    data->z_raw = (int16_t)((raw[4] << 8) | raw[5]);

    data->x_g_raw = (float)data->x_raw / ACCEL_LSB_PER_G;
    data->y_g_raw = (float)data->y_raw / ACCEL_LSB_PER_G;
    data->z_g_raw = (float)data->z_raw / ACCEL_LSB_PER_G;

    data->x_g = accel_cal.mx * data->x_g_raw + accel_cal.cx;
    data->y_g = accel_cal.my * data->y_g_raw + accel_cal.cy;
    data->z_g = accel_cal.mz * data->z_g_raw + accel_cal.cz;

    return MPU6050_OK;
}

uint8_t MPU6050_ReadTemperature(float *temp_c)
{
    uint8_t raw[2];
    int16_t temp_raw;

    if (temp_c == 0)
    {
        return MPU6050_ERROR;
    }

    if (MPU6050_ReadRegs(REG_TEMP_OUT_H, raw, 2) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    temp_raw = (int16_t)((raw[0] << 8) | raw[1]);

    /*
     * Formula from the MPU-6050 register map:
     * temperature in Celsius = raw / 340 + 36.53
     */
    *temp_c = ((float)temp_raw / 340.0f) + 36.53f;

    return MPU6050_OK;
}

void MPU6050_SetCalibration(MPU6050_Calibration cal)
{
    accel_cal = cal;
}

MPU6050_Calibration MPU6050_CalculateAxisCalibration(
    float raw_pos,
    float raw_neg,
    float raw_zero
)
{
    MPU6050_Calibration cal =
    {
        .mx = 1.0f,
        .my = 1.0f,
        .mz = 1.0f,
        .cx = 0.0f,
        .cy = 0.0f,
        .cz = 0.0f
    };

    /*
     * For one axis:
     * + orientation should read +1g.
     * - orientation should read -1g.
     * zero orientation should read 0g.
     *
     * Simple scale and offset:
     * m = 2 / (raw_pos - raw_neg)
     * c = -m * raw_zero
     */
    float m = 2.0f / (raw_pos - raw_neg);
    float c = -m * raw_zero;

    cal.mx = m;
    cal.cx = c;

    return cal;
}

uint8_t MPU6050_Init(void)
{
    uint8_t who = 0;

    HAL_Delay(100);

    mpu6050_addr = MPU6050_ADDR_LOW;

    if (MPU6050_ReadRegs(REG_WHO_AM_I, &who, 1) != MPU6050_OK || who != 0x68)
    {
        mpu6050_addr = MPU6050_ADDR_HIGH;

        if (MPU6050_ReadRegs(REG_WHO_AM_I, &who, 1) != MPU6050_OK || who != 0x68)
        {
            return MPU6050_ERROR;
        }
    }

    if (MPU6050_WriteReg(REG_PWR_MGMT_1, 0x01) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    HAL_Delay(30);

    if (MPU6050_WriteReg(REG_SMPLRT_DIV, 9) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    if (MPU6050_WriteReg(REG_CONFIG, 0x03) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    if (MPU6050_WriteReg(REG_GYRO_CONFIG, 0x00) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    if (MPU6050_WriteReg(REG_ACCEL_CONFIG, 0x00) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    return MPU6050_OK;
}

float MPU6050_CalculateAccelerationG(const MPU6050_AccelData *data)
{
    if (data == 0)
    {
        return 0.0f;
    }

    return sqrtf(
        (data->x_g * data->x_g) +
        (data->y_g * data->y_g) +
        (data->z_g * data->z_g)
    );
}

float MPU6050_CalculateAccelerationMS2(const MPU6050_AccelData *data)
{
    float acceleration_g;

    if (data == 0)
    {
        return 0.0f;
    }

    acceleration_g = MPU6050_CalculateAccelerationG(data);

    /*
     * 1g is about 9.81 m/s^2.
     */
    return acceleration_g * 9.81f;
}
