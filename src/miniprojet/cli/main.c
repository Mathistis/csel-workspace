#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MODE 0666
#define PATH "../ipc"
#define FIFO_SET_MANUAL 0x14
#define FIFO_SET_FREQ 0x15


struct cmd  {
    int cmd;
    int value;
};

int fifo_open(){
    
    int fd = open(PATH, O_WRONLY | O_NONBLOCK| O_CREAT, MODE);    
    if (fd < 0) {
        printf("Can't open FIFO " PATH " \n");
    }
    return fd;
}

int fifo_write(void* buf, int size){
    int fd = fifo_open();
    int readen = write(fd, buf, size);
    close(fd);    
    if (readen == size) return 0;
    return -1;
}

int main()
{

    struct cmd mycommand = {
        .cmd = FIFO_SET_MANUAL,
        .value = 1,
    };

    printf("Sending \"FIFO_SET_MANUAL\" with value 1\n Press [ENTER] to continue\n");
    while(getchar() != '\n') ;
    fifo_write(&mycommand, sizeof(mycommand));

    mycommand.value = 0;
    printf("Sending \"FIFO_SET_MANUAL\" with value 0\n Press [ENTER] to continue\n");
    while(getchar() != '\n');
    fifo_write(&mycommand, sizeof(mycommand));

    mycommand.value = 1;
    printf("Sending \"FIFO_SET_MANUAL\" with value 1\n Press [ENTER] to continue\n");
    while(getchar() != '\n');
    fifo_write(&mycommand, sizeof(mycommand));

    mycommand.cmd = FIFO_SET_FREQ;
    mycommand.value = 10;
    printf("Sending \"FIFO_SET_FREQ\" with value 10\n Press [ENTER] to continue\n");
    while(getchar() != '\n');
    fifo_write(&mycommand, sizeof(mycommand));

    mycommand.cmd = FIFO_SET_FREQ;
    mycommand.value = 2;
    printf("Sending \"FIFO_SET_FREQ\" with value 2\n Press [ENTER] to continue\n");
    while(getchar() != '\n');
    fifo_write(&mycommand, sizeof(mycommand));


    return 0;
}
