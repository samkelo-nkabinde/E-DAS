/*
 * kalman_filter.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_KALMAN_FILTER_H_
#define INC_KALMAN_FILTER_H_

typedef struct
{
    float q;  // process noise covariance
    float r;  // measurement noise covariance
    float x;  // estimated value
    float p;  // estimation error covariance
    float k;  // Kalman gain

} kalman_filter_t;

void  kalman_init(kalman_filter_t *kf, float q, float r, float initial);
float kalman_update(kalman_filter_t *kf, float measurement);

#endif /* INC_KALMAN_FILTER_H_ */
