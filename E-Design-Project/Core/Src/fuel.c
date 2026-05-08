/*
 * fuel.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */

#include "fuel.h"

float last_liters = 0.0f;
float last_distance = 0.0f;

float km_per_l =  0.0f;
float l_per_100km = 0.0f;

void compute_fuel_efficiency(void)
{
    km_per_l = (last_liters > 0.0f) ? last_distance / last_liters : 0.0f;
    l_per_100km = (last_distance > 0.0f) ? (last_liters / last_distance) * 100.0f : 0.0f;
}
