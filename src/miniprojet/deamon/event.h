#pragma once
#ifndef __EVENT
#define __EVENT

#include <sys/epoll.h>

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

#endif