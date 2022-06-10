#pragma once
#ifndef __TIMER
#define __TIMER
#include "event.h"
#include <sys/timerfd.h>


int set_interval(const struct itimerspec* it);


#endif