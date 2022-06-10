#pragma once
#ifndef __FIFO
#define __FIFO

#include "event.h"
#define PATH "../ipc"
#define FIFO_INTERRUPT 0x13
#define FIFO_SET_MANUAL 0x14
#define FIFO_SET_FREQ 0x15

void fifo_get_event(struct k_fd* event, int fifofd, void (*handler)(struct custom_event_data*));

int init_fifo();

int fifo_read(int fd, void * buf, int size);

#endif