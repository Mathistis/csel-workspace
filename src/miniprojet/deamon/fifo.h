#pragma once
#ifndef __FIFO
#define __FIFO

#include "event.h"
#define PATH "../ipc"
#define FIFO_INTERRUPT 0x13

void fifo_get_event(struct k_fd* event, int fifofd);

int init_fifo();

int fifo_read(int fd, void * buf, int size);

#endif