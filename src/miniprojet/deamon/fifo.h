#pragma once
#ifndef __FIFO
#define __FIFO
#define PATH "../ipc"

extern int init_fifo();

int fifo_read(int fd, void * buf, int size);

#endif