#pragma once
#ifndef __CONTROLLER
#define __CONTROLLER

#define MODE_AUTO   0
#define MODE_MANUAL 1

/**
 * @brief Init GPIO, timer and temp sensor
 * 
 * @return int <0 if an error occured
 */
int init_controller(int* is_manu, int* freq_manual);

/**
 * @brief Set the current mode for the frequency management
 * Use macro MODE_AUTO or MODE_MANUAL
 * 
 * @param mode Should be MODE_AUTO ou MODE_MANUAL
 * 
 */
void set_mode(int mode);


/**
 * @brief Stop timer and  free GPIO
 * 
 * @return int <0 if an error occured
 */
int stop_controller(void);

#endif