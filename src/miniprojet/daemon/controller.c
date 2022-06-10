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
 * Autĥor:	 Raemy Mathis, Rémy Macherel
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
#include <stdio.h>
#include <syslog.h>
#include "gpio.h"
#include "fifo.h"
#include "timer.h"
#include "temp.h"
#include "oled.h"

#define GPIO_K1 "/sys/class/gpio/gpio0"
#define GPIO_K2 "/sys/class/gpio/gpio2"
#define GPIO_K3 "/sys/class/gpio/gpio3"
#define MYDEVICE "/sys/class/led_class/led_device"
#define FREQ "/freq"
#define IS_MANU "/is_manu"

#define ACK_READ_SIZE 30
#define LED "10"
#define PIN_K1 "0"
#define PIN_K2 "2"
#define PIN_K3 "3"
#define PIN_NBR 3 // Nbr of btn to open

#define INCREMENT_HZ 1

struct cmd
{
    int cmd;
    int value;
};


void fifo_handler(struct custom_event_data *data)
{
    printf("Fifo_Handler\n");
    int err, fd = 0;
    char buff[ACK_READ_SIZE] = "";
    struct cmd command;
    err = fifo_read(data->fd, &command, sizeof(command)); //
    if (err < 0)
        printf("Error reading FIFO. err: %d\n", err);
    switch (command.cmd)
    {
    case FIFO_SET_FREQ:
        fd = open(MYDEVICE FREQ, O_WRONLY);
        sprintf(buff, "%d", command.value);
        printf("Set new freq to %d\n", command.value);
        write(fd, buff, strlen(buff));
        close(fd);
        break;
    case FIFO_SET_MANUAL:
        fd = open(MYDEVICE IS_MANU, O_RDWR);
        sprintf(buff, "%d", command.value);
        printf("Set mode to %d\n", command.value);

        write(fd, buff, strlen(buff));
        break;
    default:
        break;
    }
}
void btn_inq_freq_handler(struct custom_event_data *data)
{
    printf("BTN_FREQ_INQ\n");
    int len, fd = 0;
    char buff[ACK_READ_SIZE] = "";
    len = pread(data->fd, buff, ACK_READ_SIZE, 0);
    if (len < 0)
    {
        printf("Error reading INC_FREQ fd");
        return;
    }
    int value_btn = 0;
    sscanf(buff, "%d", &value_btn);
    if (value_btn == 0)
    {
        switch_power_led(0);

        fd = open(MYDEVICE FREQ, O_RDWR);
        len = read(fd, buff, ACK_READ_SIZE);
        int freq_inc = 0;
        sscanf(buff, "%d", &freq_inc);
        freq_inc += INCREMENT_HZ;
        sprintf(buff, "%d", freq_inc);
        write(fd, buff, strlen(buff));
        close(fd);
    }
    else if (value_btn == 1)
    {
        switch_power_led(1);
    }
}
void btn_man_auto_handler(struct custom_event_data *data)
{

    printf("Man/Auto functionnality\n");
    int len, fd = 0;
    char buff[ACK_READ_SIZE] = "";
    len = pread(data->fd, buff, ACK_READ_SIZE, 0);
    if (len < 0)
    {
        printf("Error reading RST_FREQ fd");
        return;
    }
    fd = open(MYDEVICE IS_MANU, O_RDWR);
    if (fd < 0)
    {
        printf("Error openning IS_MANU fd %d", fd);
        return;
    }
    int value_btn = 0;
    sscanf(buff, "%d", &value_btn);
    if (value_btn == 0)
    {
        len = read(fd, buff, ACK_READ_SIZE);
        int is_manu = 0;
        sscanf(buff, "%d", &is_manu);
        is_manu = 1 - is_manu;
        sprintf(buff, "%d", is_manu);
        write(fd, buff, strlen(buff));
        printf("BTN_FREQ_RST\n");
        close(fd);
    }
}
void btn_dec_freq_handler(struct custom_event_data *data)
{
    printf("Dec functionnality\n");
    int len, fd = 0;
    char buff[ACK_READ_SIZE] = "";
    len = pread(data->fd, buff, ACK_READ_SIZE, 0);
    if (len < 0)
    {
        printf("Error reading DEC_FREQ fd");
        return;
    }
    printf("BTN_FREQ_DEC\n");
    int value_btn = 0;
    sscanf(buff, "%d", &value_btn);
    if (value_btn == 0)
    {
        switch_power_led(0);
        fd = open(MYDEVICE FREQ, O_RDWR);
        len = read(fd, buff, ACK_READ_SIZE);
        if (fd < 0)
        {
            printf("Error openning IS_MANU fd %d", fd);
            return;
        }
        int freq_inc_dec = 0;
        sscanf(buff, "%d", &freq_inc_dec);
        freq_inc_dec -= INCREMENT_HZ;
        if (freq_inc_dec <= 0)
            freq_inc_dec = 1;
        sprintf(buff, "%d", freq_inc_dec);
        write(fd, buff, strlen(buff));
        close(fd);
    }
    else if (value_btn == 1)
    {
        switch_power_led(1);
    }
}
void timer_handler(struct custom_event_data *data)
{
    char buff[ACK_READ_SIZE] = "";
    int len = 0;
    read(data->fd, buff, ACK_READ_SIZE);
    int freq = 0;
    int temp = get_temp();
    int freqfd = open(MYDEVICE FREQ, O_RDONLY);
    if (freqfd > 0)
    {
        len = pread(freqfd, buff, ACK_READ_SIZE, 0);
        close(freqfd);
    }
    if (len > 0)
    {
        sscanf(buff, "%d", &freq);
    }
    int is_manufd = open(MYDEVICE IS_MANU, O_RDONLY);
    if (is_manufd > 0)
    {
        len = pread(is_manufd, buff, ACK_READ_SIZE, 0);
        close(is_manufd);
    }
    if (len > 0)
    {
        sscanf(buff, "%d", &is_manufd);
    }
    update_temp(temp);
    update_freq(freq);
    update_mode(is_manufd);
}

int start_controller()
{
    int err = 0;

    struct pin btn_pins[] = {
        [0] = {
            .k_pin = PIN_K1,
            .k_gpio_direction = GPIO_K1 "/direction",
            .k_gpio_value = GPIO_K1 "/value",
            .edge_type = GPIO_K1 "/edge",
            .edge = "both",
            .handler = btn_inq_freq_handler,
        },
        [1] = {
            .k_pin = PIN_K2,
            .k_gpio_direction = GPIO_K2 "/direction",
            .k_gpio_value = GPIO_K2 "/value",
            .edge_type = GPIO_K2 "/edge",
            .edge = "both",
            .handler = btn_man_auto_handler,
        },
        [2] = {
            .k_pin = PIN_K3,
            .k_gpio_direction = GPIO_K3 "/direction",
            .k_gpio_value = GPIO_K3 "/value",
            .edge_type = GPIO_K3 "/edge",
            .edge = "both",
            .handler = btn_dec_freq_handler,
        }};
    init_oled();
    struct k_fd btn_fd[PIN_NBR];
    err = open_btn(btn_pins, btn_fd, PIN_NBR);
    if (err < 0)
    {
        printf("Error Openning BTN\n");
        return -1;
    }

    int epfd = epoll_create1(0);
    if (epfd < 0)
        printf("[Epoll] epoll_create1() = -1\n");

    for (int i = 0; i < PIN_NBR; i++)
    {
        printf("%d\n", btn_fd[i].fd);
        err = epoll_ctl(epfd, EPOLL_CTL_ADD, btn_fd[i].fd, &btn_fd[i].events);
        if (err < 0)
        {
            printf("[Buttons] epoll_ctl() = -1\n");
            return -1;
        }
    }

    int fifo_fd = init_fifo();
    if (fifo_fd < 0)
    {
        printf("Error creating Fifo\n");
    }

    struct k_fd fifo_events;
    fifo_get_event(&fifo_events, fifo_fd, fifo_handler);

    err = epoll_ctl(epfd, EPOLL_CTL_ADD, fifo_fd, &fifo_events.events);
    if (err < 0)
    {
        printf("Can't add FIFO to epoll err: %d\n", err);
    }
    struct timespec default_spec;
    default_spec.tv_nsec = 500000000;
    default_spec.tv_sec = 0;

    const struct itimerspec my_interval =
        {
            default_spec,
            default_spec,
        };

    int timerfd = set_interval(&my_interval);

    struct custom_event_data timer_data = {
        .fd = timerfd,
        .handler = timer_handler};

    struct epoll_event event_timer = {
        .events = EPOLLIN,
        .data.ptr = &timer_data,
    };

    err = epoll_ctl(epfd, EPOLL_CTL_ADD, timerfd, &event_timer);
    if (err < 0)
        printf("[Timers] epoll_ctl() = -1\n");

    struct epoll_event list_events[10];
    int nbr_events = 0;

    while (1)
    {
        nbr_events = epoll_wait(epfd, list_events, 10, -1);
        if (nbr_events < 0)
        {
            printf("Error in wait. Abort.\n");
            break;
        }
        for (int i = 0; i < nbr_events; i++)
        {
            struct custom_event_data *data = list_events[i].data.ptr;
            data->handler(data);
        }
    }
    return 0;
}
