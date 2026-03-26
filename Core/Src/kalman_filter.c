/*
 * kalman_filter.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Samkelo Nkabinde
 */

#include "kalman_filter.h"

/*
 * Initials a kalman filter instance
 */
void  kalman_init(kalman_filter_t *kf, float initial)
{
    kf->x = initial;
    kf->p = 1.0f;
    kf->k = 0.0f;
    kf->q = 0.01;
    kf->r = 0.1;

    return;
}


float kalman_update(kalman_filter_t *kf, float measurement)
{
    kf->p = kf->p + kf->q;

    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1.0f - kf->k) * kf->p;

    return kf->x;
}
