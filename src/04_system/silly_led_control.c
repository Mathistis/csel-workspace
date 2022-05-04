/**
 * Copyright 2018 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project:	HEIA-FR / HES-SO MSE - MA-CSEL1 Laboratory
 *
 * Abstract: System programming -  file system
 *
 * Purpose:	NanoPi silly status led control system
 *
 * Autĥor:	 Daniel Gachet, Raemy Mathis, Rémy Macherel
 * Date:	 07.11.2018
 * Modified: 04.05.2022
 */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <stdio.h>

/*
 * status led - gpioa.10 --> gpio10
 * power led  - gpiol.10 --> gpio362
 */
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_LED "/sys/class/gpio/gpio10"
#define GPIO_K1 "/sys/class/gpio/gpio0"
#define GPIO_K2 "/sys/class/gpio/gpio2"
#define GPIO_K3 "/sys/class/gpio/gpio3"
#define LED "10"
#define PIN_K1 "0"
#define PIN_K2 "2"
#define PIN_K3 "3"
#define PIN_NBR 3 // Nbr of btn to open
#define ACK_READ_SIZE 8

#define TIM_FUNC 0x01
#define RST_FREQ 0x10 // Attribute functionnality to a button
#define DEC_FREQ 0x11 //  "
#define INC_FREQ 0x12 //  "


struct pin
{
    char *k_pin;
    char *k_gpio_direction;
    char *k_gpio_value;
    char *edge_type;
    char *edge;
    int functionnality;
};
// struct pin
// {
//     char *k_pin[PIN_NBR];
//     char *k_gpio_direction[PIN_NBR];
//     char *k_gpio_value[PIN_NBR];
//     int   functionnality[PIN_NBR];
// };

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


static int open_led()
{
    // unexport pin out of sysfs (reinitialization)
    int f = open(GPIO_UNEXPORT, O_WRONLY);
    write(f, LED, strlen(LED));
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    write(f, LED, strlen(LED));
    close(f);

    // config pin
    f = open(GPIO_LED "/direction", O_WRONLY);
    write(f, "out", 3);
    close(f);

    // open gpio value attribute
    f = open(GPIO_LED "/value", O_RDWR);
    return f;
}

static int open_btn(struct pin * pin, struct k_fd* opened_fd, int len)
{
    // unexport pin out of sysfs (reinitialization)
    int f = open(GPIO_UNEXPORT, O_WRONLY);
    for (int i = 0; i < len; i++)
    {
        write(f, pin[i].k_pin, strlen(pin[i].k_pin));
    }
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    for (int i = 0; i < len; i++)
    {
        write(f, pin[i].k_pin, strlen(pin[i].k_pin));
    }
    close(f);

    // config pin
    for (int i = 0; i < len; i++)
    {
        f = open(pin[i].k_gpio_direction, O_WRONLY);
        write(f, "in", 2);
        close(f);
    }
    for (int i = 0; i < len; i++)
    {
        f = open(pin[i].edge_type, O_WRONLY);
        write(f, pin[i].edge, strlen(pin[i].edge)+1);
        close(f);
    }
    // open gpio value attribute
    for (int i = 0; i < len; i++)
    {
        opened_fd[i].fd = open(pin[i].k_gpio_value, O_RDWR);
        if(opened_fd[i].fd < 0) return -1;
        char buff[ACK_READ_SIZE] = ""; // prepare buffre to read fd that interrupt (acknowledge)
        read(opened_fd[i].fd, buff, ACK_READ_SIZE);
        opened_fd[i].events.events = EPOLLIN;
        opened_fd[i].events.data.ptr = &opened_fd[i].data;
        opened_fd[i].data.fd = opened_fd[i].fd;
        opened_fd[i].data.functionnality = pin[i].functionnality;
    }
    return 1;
}

int main()
{
    int err = 0;

    struct pin btn_pins[] = {
        [0] = {
            .k_pin = PIN_K1,
            .k_gpio_direction = GPIO_K1 "/direction",
            .k_gpio_value = GPIO_K1 "/value",
            .edge_type = GPIO_K1 "/edge",
            .edge = "falling",
            .functionnality = INC_FREQ,
        },
        [1] = {
            .k_pin = PIN_K2,
            .k_gpio_direction = GPIO_K2 "/direction",
            .k_gpio_value = GPIO_K2 "/value",
            .edge_type = GPIO_K2 "/edge",
            .edge = "falling",
            .functionnality = RST_FREQ,
        },
        [2] = {
            .k_pin = PIN_K3,
            .k_gpio_direction = GPIO_K3 "/direction",
            .k_gpio_value = GPIO_K3 "/value",
            .edge_type = GPIO_K3 "/edge",
            .edge = "falling",
            .functionnality = DEC_FREQ,
        }
    };
    int led = open_led();
    pwrite(led, "1", sizeof("1"), 0);
    
    struct k_fd btn_fd[PIN_NBR];
    err = open_btn( btn_pins, btn_fd, PIN_NBR);
    if(err <0) {
        printf("Error Openning BTN\n");
        return -1;
    }

    int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerfd == -1)
        printf("timer_create() = -1\n");

    struct timespec time_spec = {
        0,         /* Seconds */
        500000000, /* Nanoseconds */
    };

    const struct itimerspec my_interval =
        {
            time_spec,
            time_spec,
        };
        
    err = timerfd_settime(timerfd, 0, &my_interval, NULL);
    if (err < 0)
        printf("timerfd_settime() = -1\n");
    int epfd = epoll_create1(0);
    if (epfd < 0)
        printf("epoll_create1() = -1\n");

    struct custom_event_data timer_data = {
        .fd = timerfd,
        .functionnality = TIM_FUNC
    };
    struct epoll_event event_timer = {
        .events = EPOLLIN,
        .data.ptr = &timer_data,
    };

    err = epoll_ctl(epfd, EPOLL_CTL_ADD, timerfd, &event_timer);
    if (err < 0)
        printf("[Timers] epoll_ctl() = -1\n");
    for (int i = 0; i < PIN_NBR; i++)
    {
        printf("%d\n", btn_fd[i].fd);
        err |= epoll_ctl(epfd, EPOLL_CTL_ADD, btn_fd[i].fd, &btn_fd[i].events);
    }
    if (err < 0)
    {
        printf("[Buttons] epoll_ctl() = -1\n");
        return -1;
    }

    struct epoll_event list_events[10];
    int nbr_events = 0;
    int k = 0;
    char buff[ACK_READ_SIZE] = ""; // prepare buffre to read fd that interrupt (acknowledge)
    int j = 3;
    while (j--)
    {
        nbr_events = epoll_wait(epfd, list_events, 10, -1);
        if(nbr_events <0) { 
            printf("Error in wait. Abort.\n");
            break;
        }
        for (int i = 0; i < nbr_events; i++)
        {
            struct custom_event_data* data = list_events[i].data.ptr;
            printf("fd : %d\n",data->fd);
            int len = read(data->fd, buff, ACK_READ_SIZE); // acknowledge the fd anyway what it is. Is this wrong ?
            printf("len: %d\n", len);
            switch(data->functionnality){
                case TIM_FUNC:
                    k = (k + 1) % 2;
                    if (k == 0)
                    {
                        pwrite(led, "1", sizeof("1"), 0);
                        printf("turning on\n");
                    }
                    else
                    {
                        pwrite(led, "0", sizeof("0"), 0);
                        printf("turning off\n");
                    }
                    break;
                case INC_FREQ:
                    printf("BTN_FREQ_INQ\n");
                    break;
                case DEC_FREQ:
                    printf("BTN_FREQ_DEC\n");
                    break;
                case RST_FREQ:
                    printf("BTN_FREQ_RST\n");
                    break;
                default:
                    printf("WTF ?! Unknow functionnality ... ?! \n");

            }
        }
    }

    return 0;
}