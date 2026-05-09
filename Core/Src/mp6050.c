/*
 * mp6050.c
 *
 *  Created on: May 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "main.h"
#include "mp6050.h"

#define MPU6050_SCL_PORT GPIOA
#define MPU6050_SCL_PIN  GPIO_PIN_8

#define MPU6050_SDA_PORT GPIOC
#define MPU6050_SDA_PIN  GPIO_PIN_8

#define MPU6050_ADDR     0x68

#define REG_SMPLRT_DIV   0x19
#define REG_CONFIG       0x1A
#define REG_GYRO_CONFIG  0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_XOUT_H 0x3B
#define REG_TEMP_OUT_H   0x41
#define REG_PWR_MGMT_1   0x6B
#define REG_WHO_AM_I     0x75

/*
 * AFS_SEL = 0 gives +/- 2g.
 * The MPU-6050 register map gives 16384 LSB/g for this setting.
 */
#define ACCEL_LSB_PER_G  16384.0f



static MPU6050_Calibration accel_cal =
{
    .mx = 1.0f, .my = 1.0f, .mz = 1.0f,
    .cx = 0.0f, .cy = 0.0f, .cz = 0.0f
};

static void delay_i2c(void)
{
    /*
     * Small delay for software I2C.
     * Increase this if your board is unstable.
     */
    for (volatile int i = 0; i < 40; i++)
    {
        __NOP();
    }
}

static void SDA_High(void)
{
    HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_SET);
}

static void SDA_Low(void)
{
    HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_RESET);
}

static void SCL_High(void)
{
    HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_SET);
}

static void SCL_Low(void)
{
    HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_RESET);
}

static GPIO_PinState SDA_Read(void)
{
    return HAL_GPIO_ReadPin(MPU6050_SDA_PORT, MPU6050_SDA_PIN);
}

static void I2C_Start(void)
{
    SDA_High();
    SCL_High();
    delay_i2c();

    SDA_Low();
    delay_i2c();

    SCL_Low();
    delay_i2c();
}

static void I2C_Stop(void)
{
    SDA_Low();
    delay_i2c();

    SCL_High();
    delay_i2c();

    SDA_High();
    delay_i2c();
}

static uint8_t I2C_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
        {
            SDA_High();
        }
        else
        {
            SDA_Low();
        }

        delay_i2c();

        SCL_High();
        delay_i2c();

        SCL_Low();
        delay_i2c();

        data <<= 1;
    }

    SDA_High();
    delay_i2c();

    SCL_High();
    delay_i2c();

    uint8_t ack = (SDA_Read() == GPIO_PIN_RESET);

    SCL_Low();
    delay_i2c();

    return ack;
}

static uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t data = 0;

    SDA_High();

    for (uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;

        SCL_High();
        delay_i2c();

        if (SDA_Read() == GPIO_PIN_SET)
        {
            data |= 1;
        }

        SCL_Low();
        delay_i2c();
    }

    if (ack)
    {
        SDA_Low();
    }
    else
    {
        SDA_High();
    }

    delay_i2c();

    SCL_High();
    delay_i2c();

    SCL_Low();
    delay_i2c();

    SDA_High();

    return data;
}

static uint8_t MPU6050_WriteReg(uint8_t reg, uint8_t value)
{
    I2C_Start();

    if (!I2C_WriteByte((MPU6050_ADDR << 1) | 0))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    if (!I2C_WriteByte(reg))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    if (!I2C_WriteByte(value))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    I2C_Stop();
    return MPU6050_OK;
}

static uint8_t MPU6050_ReadRegs(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    I2C_Start();

    if (!I2C_WriteByte((MPU6050_ADDR << 1) | 0))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    if (!I2C_WriteByte(reg))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    I2C_Start();

    if (!I2C_WriteByte((MPU6050_ADDR << 1) | 1))
    {
        I2C_Stop();
        return MPU6050_ERROR;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        buffer[i] = I2C_ReadByte(i < (length - 1));
    }

    I2C_Stop();
    return MPU6050_OK;
}

uint8_t MPU6050_Init(void)
{
    uint8_t who = 0;

    HAL_Delay(100);

    if (MPU6050_ReadRegs(REG_WHO_AM_I, &who, 1) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    if (who != 0x68)
    {
        return MPU6050_ERROR;
    }

    /*
     * The register map says the device powers up in sleep mode.
     * Clear sleep and use the X gyro clock by writing 0x01.
     */
    if (MPU6050_WriteReg(REG_PWR_MGMT_1, 0x01) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    HAL_Delay(30);

    /*
     * Sample rate:
     * With DLPF enabled, base rate is 1 kHz.
     * sample_rate = 1000 / (1 + SMPLRT_DIV)
     * 9 gives 100 Hz.
     */
    if (MPU6050_WriteReg(REG_SMPLRT_DIV, 9) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    /*
     * DLPF_CFG = 3.
     * This gives a filtered output suitable for vehicle motion.
     */
    if (MPU6050_WriteReg(REG_CONFIG, 0x03) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    /*
     * Gyro full scale: +/- 250 deg/s.
     */
    if (MPU6050_WriteReg(REG_GYRO_CONFIG, 0x00) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    /*
     * Accelerometer full scale: +/- 2g.
     * AFS_SEL = 0, sensitivity = 16384 LSB/g.
     */
    if (MPU6050_WriteReg(REG_ACCEL_CONFIG, 0x00) != MPU6050_OK)
    {
        return MPU6050_ERROR;
    }

    return MPU6050_OK;
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
     * This keeps X, Y, and Z from the same sample instant.
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
     * temp C = raw / 340 + 36.53
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
     * A simple scale and offset estimate:
     * m = 2 / (raw_pos - raw_neg)
     * c = -m * raw_zero
     */

    float m = 2.0f / (raw_pos - raw_neg);
    float c = -m * raw_zero;

    cal.mx = m;
    cal.cx = c;

    return cal;
}
uint8_t MPU6050_ReadWhoAmI(uint8_t *who)
{
    if (who == 0)
    {
        return MPU6050_ERROR;
    }

    return MPU6050_ReadRegs(REG_WHO_AM_I, who, 1);
}
