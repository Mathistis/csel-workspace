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

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

/*
 * status led - gpioa.10 --> gpio10
 * power led  - gpiol.10 --> gpio362
 */
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_LED "/sys/class/gpio/gpio"
#define DIRECTION "/direction"
#define VALUE "/value"

#define ACK_READ_SIZE 20
#define LED_STATUS "10"
#define LED_POWER "362"
#define PIN_K1 "0"
#define PIN_K2 "2"
#define PIN_K3 "3"
#define PIN_NBR 3 // Nbr of btn to open

// #define TIM_FUNC 0x01

int open_btn(struct pin * pin, struct k_fd* opened_fd, int len)
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
        pread(opened_fd[i].fd, buff, ACK_READ_SIZE, 0);
        opened_fd[i].events.events = EPOLLERR;
        opened_fd[i].events.data.ptr = &opened_fd[i].data;
        opened_fd[i].data.fd = opened_fd[i].fd;
        opened_fd[i].data.handler = pin[i].handler;
    }
    return 1;
}

int open_led()
{


    int f = open(GPIO_UNEXPORT, O_WRONLY);
    write(f, LED_POWER, strlen(LED_POWER));
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    write(f, LED_POWER, strlen(LED_POWER));
    close(f);

    // config pin
    f = open(GPIO_LED LED_POWER DIRECTION, O_WRONLY);
    write(f, "out", 3);
    close(f);

    // open gpio value attribute
    f = open(GPIO_LED LED_POWER VALUE, O_RDWR);
    return f;
}

void switch_power_led(int value){
    static int led_power_fd = 0;
    if(!led_power_fd){
        led_power_fd = open_led();
    }
    char buf[ACK_READ_SIZE];
    snprintf(buf, ACK_READ_SIZE ,"%d", value );
    pwrite(led_power_fd, buf, sizeof(buf), 0);

}