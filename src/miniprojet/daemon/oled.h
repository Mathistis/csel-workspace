#pragma once
#ifndef __OLED
#define __OLED

void  init_oled();

void update_temp(int temp);

void update_freq( int freq);

void update_mode(int mode);

#endif