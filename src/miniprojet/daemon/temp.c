#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define TEMP_DRIVER "/sys/class/thermal/thermal_zone0/temp"

int init_temp(){
    int fd = open(TEMP_DRIVER, O_RDONLY);
    if (fd <= 0){
        printf("Can't open " TEMP_DRIVER);
        printf("\n");
    }
    return fd;
}

int get_temp(){
    static int tempfd = 0;
    if(tempfd <= 0){
        tempfd = init_temp();
    } 
    char buff[20] = "";
    int temperature = 0;
    pread(tempfd, buff, 20, 0);
    sscanf(buff, "%d", &temperature);
    return temperature/1000;
}