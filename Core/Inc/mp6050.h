/*
 * mp6050.h
 *
 *  Created on: May 9, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_MP6050_H_
#define INC_MP6050_H_

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "main.h"

extern I2C_HandleTypeDef hi2c1;


#define MPU6050_OK     1
#define MPU6050_ERROR  0

typedef struct
{
    int16_t x_raw;
    int16_t y_raw;
    int16_t z_raw;

    float x_g_raw;
    float y_g_raw;
    float z_g_raw;

    float x_g;
    float y_g;
    float z_g;

} MPU6050_AccelData;

typedef struct
{
    float mx;
    float my;
    float mz;

    float cx;
    float cy;
    float cz;

} MPU6050_Calibration;


extern MPU6050_AccelData acceleration;
extern bool MPU6050_ready;
/*
 * Initializes the MPU-6050.
 * Returns MPU6050_OK if successful.
 */
uint8_t MPU6050_Init(void);

/*
 * Reads the accelerometer values.
 * The raw values are stored in x_raw, y_raw, and z_raw.
 * The converted g values are stored in x_g, y_g, and z_g.
 */
uint8_t MPU6050_ReadAccel(MPU6050_AccelData *data);

/*
 * Reads the internal MPU-6050 temperature.
 */
uint8_t MPU6050_ReadTemperature(float *temp_c);

/*
 * Sets the accelerometer calibration values.
 */
void MPU6050_SetCalibration(MPU6050_Calibration cal);

/*
 * Calculates simple one-axis calibration values.
 */
MPU6050_Calibration MPU6050_CalculateAxisCalibration(
    float raw_pos,
    float raw_neg,
    float raw_zero
);

/*
 * Reads the WHO_AM_I register.
 * Expected value is 0x68.
 */
uint8_t MPU6050_ReadWhoAmI(uint8_t *who);

float MPU6050_CalculateAccelerationG(const MPU6050_AccelData *data);
float MPU6050_CalculateAccelerationMS2(const MPU6050_AccelData *data);

#endif /* INC_MP6050_H_ */
