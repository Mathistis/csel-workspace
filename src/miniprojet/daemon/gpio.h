#pragma once
#ifndef __GPIO
#define __GPIO
#include "event.h"

struct pin
{
    char *k_pin;
    char *k_gpio_direction;
    char *k_gpio_value;
    char *edge_type;
    char *edge;
    void (*handler)(struct custom_event_data* data);

};



int open_btn(struct pin * pin, struct k_fd* opened_fd, int len);


void switch_power_led(int value);


#endif