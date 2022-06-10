#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "timer.h"




int init_timer()
{
    int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerfd == -1)
        printf("timer_create() = -1\n");
    return timerfd;
}

int set_interval(const struct itimerspec* it)
{
    static int timerfd = 0;
    if (!timerfd)
    {
        timerfd = init_timer();
    }


    int err = timerfd_settime(timerfd, 0, it, NULL);
    if (err < 0)
        printf("[Timers] timerfd_settime() = -1\n [TIMERFD] %d\n", timerfd);

    return timerfd;
}