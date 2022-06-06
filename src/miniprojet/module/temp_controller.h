#pragma once
#ifndef __TEMP
#define __TEMP
#define THERMAL_ZONE "cpu-thermal"

/**
 * @brief Init temperature controller
 * 
 */
void init_temp(void);


/**
 * @brief Get the temp object
 * 
 * @param temp value measured 
 * @return int 
 */
int get_temp(int* temp);


#endif