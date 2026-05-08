/*
 * fuel.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_FUEL_H_
#define INC_FUEL_H_

extern float last_liters;
extern float last_distance;

// Fuel Efficiency

extern float km_per_l;
extern float l_per_100km;

void compute_fuel_efficiency(void);
#endif /* INC_FUEL_H_ */
