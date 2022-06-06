#pragma once
#ifndef __GPIO
#define __GPIO
#include <sys/epoll.h>


struct pin
{
    char *k_pin;
    char *k_gpio_direction;
    char *k_gpio_value;
    char *edge_type;
    char *edge;
    int functionnality;
};

struct custom_event_data {
    int fd;
    int functionnality;
};

struct k_fd
{
    int fd;
    struct epoll_event events;
    struct custom_event_data data;
};

int open_btn(struct pin * pin, struct k_fd* opened_fd, int len);

int open_led();

#endif