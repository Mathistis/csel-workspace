#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fifo.h"

#define MODE 0666

int init_fifo(){
    mkfifo(PATH, MODE);
    int fd = open(PATH, O_RDONLY);
    
    return fd;
}

int fifo_read(int fd, int* buf, int len){
    
    int readen = read(fd, buf, sizeof(int) * len);    
    if (readen == len) return 0;
    else return -1;
}