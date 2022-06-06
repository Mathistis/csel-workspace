#pragma once
#ifndef __FIFO
#define __FIFO
#define PATH "../ipc"

extern int init_fifo();

int fifo_read(int fd, int* buf, int len);

#endif