/*
 * mp6050.h
 *
 *  Created on: May 9, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_MP6050_H_
#define INC_MP6050_H_

#define MPU6050_OK     0
#define MPU6050_ERROR  1

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

uint8_t MPU6050_Init(void);
uint8_t MPU6050_ReadAccel(MPU6050_AccelData *data);
uint8_t MPU6050_ReadTemperature(float *temp_c);

void MPU6050_SetCalibration(MPU6050_Calibration cal);
MPU6050_Calibration MPU6050_CalculateAxisCalibration(
    float raw_pos,
    float raw_neg,
    float raw_zero
);


#endif /* INC_MP6050_H_ */
