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
#include <stdio.h>
#include "gpio.h"
#include "fifo.h"

/*
 * status led - gpioa.10 --> gpio10
 * power led  - gpiol.10 --> gpio362
 */

#define GPIO_K1 "/sys/class/gpio/gpio0"
#define GPIO_K2 "/sys/class/gpio/gpio2"
#define GPIO_K3 "/sys/class/gpio/gpio3"
#define MYDEVICE "/sys/class/led_class/led_device"
#define FREQ "/freq"
#define IS_MANU "/is_manu"

#define ACK_READ_SIZE 20
#define LED "10"
#define PIN_K1 "0"
#define PIN_K2 "2"
#define PIN_K3 "3"
#define PIN_NBR 3 // Nbr of btn to open

// #define TIM_FUNC 0x01
#define RST_FREQ 0x10 // Attribute functionnality to a button
#define DEC_FREQ 0x11 //  "
#define INC_FREQ 0x12 //  "

#define FIFO_SET_MANUAL 0x14
#define FIFO_SET_FREQ 0x15   
#define INCREMENT_HZ 1



struct cmd  {
    int cmd;
    int value;
};


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
        }};

    int led = open_led();
    pwrite(led, "1", sizeof("1"), 0);

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
    if(fifo_fd < 0){
        printf("Error creating Fifo\n");
    }

    struct k_fd fifo_events;
    fifo_get_event(&fifo_events, fifo_fd);

    err = epoll_ctl(epfd, EPOLL_CTL_ADD, fifo_fd, &fifo_events.events);
    if(err < 0){
        printf("Can't add FIFO to epoll err: %d\n", err);
    }
    struct epoll_event list_events[10];
    int nbr_events = 0;
    char buff[ACK_READ_SIZE] = ""; // prepare buffre to read fd that interrupt (acknowledge)
    
    struct cmd command;


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
            int len = 0;
            int fd = 0;
            switch (data->functionnality)
            {
            case FIFO_INTERRUPT:
                err = fifo_read(data->fd, &command, sizeof(command)); //
                if (err < 0)
                    printf("Error reading FIFO. err: %d\n", err);
                switch (command.cmd)
                {
                case FIFO_SET_FREQ:
                    fd = open(MYDEVICE FREQ, O_RDWR);
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
                break;
            case INC_FREQ:

                len = pread(data->fd, buff, ACK_READ_SIZE, 0);
                if (len < 0)
                {
                    printf("Error reading INC_FREQ fd");
                    break;
                }
                printf("BTN_FREQ_INQ\n");
                fd = open(MYDEVICE FREQ, O_RDWR);
                len = read(fd, buff, ACK_READ_SIZE);
                int freq_inc = 0;
                sscanf(buff, "%d", &freq_inc);
                freq_inc += INCREMENT_HZ;
                sprintf(buff, "%d", freq_inc);
                write(fd, buff, strlen(buff));
                close(fd);
                break;

            case DEC_FREQ:
                len = pread(data->fd, buff, ACK_READ_SIZE, 0);
                if (len < 0)
                {
                    printf("Error reading DEC_FREQ fd");
                    break;
                }
                printf("BTN_FREQ_DEC\n");
                fd = open(MYDEVICE FREQ, O_RDWR);
                len = read(fd, buff, ACK_READ_SIZE);
                if (fd < 0)
                {
                    printf("Error openning IS_MANU fd %d", fd);
                    break;
                }
                int freq_inc_dec = 0;
                sscanf(buff, "%d", &freq_inc_dec);
                freq_inc_dec -= INCREMENT_HZ;
                if (freq_inc_dec <= 0)
                    freq_inc_dec = 1;
                sprintf(buff, "%d", freq_inc_dec);
                write(fd, buff, strlen(buff));
                close(fd);
                break;

            case RST_FREQ:
                len = pread(data->fd, buff, ACK_READ_SIZE, 0);
                if (len < 0)
                {
                    printf("Error reading RST_FREQ fd");
                    break;
                }
                fd = open(MYDEVICE IS_MANU, O_RDWR);
                if (fd < 0)
                {
                    printf("Error openning IS_MANU fd %d", fd);
                    break;
                }

                len = read(fd, buff, ACK_READ_SIZE);
                int is_manu = 0;
                sscanf(buff, "%d", &is_manu);
                is_manu = 1 - is_manu;

                sprintf(buff, "%d", is_manu);
                write(fd, buff, strlen(buff));
                printf("BTN_FREQ_RST\n");
                close(fd);
                break;
            default:
                printf("Unknow functionnality ... ?! \n");
            }
        }
    }

    return 0;
}