#pragma once
#ifndef __TIMER
#define __TIMER


/**
 * @brief Init timer for frequency led toggle
 * 
 * @param on_timer_rised Timer Handler
 * @return int < 0 mean error
 */
int init_my_timer(int (*on_timer_rised)(void));


/**
 * @brief Stop timer. Need to call it when module is removed to prevent kernel oops
 * 
 * @return int <0 if error occured
 */
int exit_timer(void);

#endif