#pragma once
#ifndef __GPIO
#define __GPIO
#define GPIO_LED 10


/**
 * @brief Initialized GPIO. Only LED STATUS for now
 * 
 * @return int <0 if error occured
 */
int init_gpio(void);

/**
 * @brief Toggle STATUS led on/off (may be a good idea to giv it to the timer)
 * 
 * @return int <0 if error occured
 */
int toggle_led(void);


/**
 * @brief We need to release GPIO when this module is removed
 * 
 */
void exit_gpio(void);


#endif