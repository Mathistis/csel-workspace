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
    printf("INITIALIZING FIFO\n");
    int err = mkfifo(PATH, MODE);
    if(err < 0){
        printf("Can't create Fifo err: %d\n", err);
    }
    int fd = open(PATH, O_RDONLY | O_NONBLOCK | O_CREAT, MODE);
    if(fd < 0){
        printf("Invalid Fifo fd : %d\n", fd);
    }
    return fd;
}

void fifo_get_event(struct k_fd* event, int fifofd){
    event->fd = fifofd;
    event->events.events = EPOLLET;
    event->events.data.ptr = &event->data;
    event->data.fd = fifofd;
    event->data.functionnality = FIFO_INTERRUPT;
}


int fifo_read(int fd, void* buf, int size){
    
    int readen = read(fd, buf, size);    
    if (readen == size) return 0;
    else return readen;
}