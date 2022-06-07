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
    int functionnality;
};



int open_btn(struct pin * pin, struct k_fd* opened_fd, int len);

int open_led();

#endif