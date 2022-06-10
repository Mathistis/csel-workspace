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
#define SET_FREQ_CMD "freq"
#define SET_MANUAL_CMD "manual"
#define INVALID_COMMAND "Invalid command\n"
#define CMD_LENGTH 20
#define USR_CMD_NUMBER 2
// ascii art from https://www.asciiart.eu/movies/star-wars
#define HEADER "    __.-._\n '-._\"7'\n /'.-c\n |  /T\n snd _)_/LI \t CSEL project CLI - Macherel & Raemy\n Enter command help for informations"

struct IPC_Command  {
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

struct usr_command {
    char* command;
    int nArgs;
    void (*fct)(int);
};

void set_frequency(int freq){
    if(freq < 0){
        printf("Invalid frequency value, it has to be greater than 0 !\n");
        return;
    }
    else if(freq > 100){
        printf("Careful, frequency over 100Hz may not be visible but it will be set anyway \n");
    }

    // printf("Send set frequency with freq = %d",freq);
    struct IPC_Command freq_command = {
        .cmd = FIFO_SET_FREQ,
        .value = freq,
    };
    fifo_write(&freq_command, sizeof(freq_command));
    
}

void set_manual(int val){
    if(val == 0 || val == 1){
        struct IPC_Command manual_command = {
            .cmd = FIFO_SET_MANUAL,
            .value = val,
        };
        fifo_write(&manual_command, sizeof(manual_command));
        // printf("Set manual with value of %d\n",val);
    }
    else{
        printf("Invalid value of manual command ! Remember it has to be 0 or 1 \n");
    }
}

int main()
{
    // struct IPC_Command mycommand = {
    //     .cmd = FIFO_SET_MANUAL,
    //     .value = 1,
    // };
    
    struct usr_command usr_commands[] = {
        [0] = {
            .command = SET_FREQ_CMD,
            .fct = set_frequency,
        },
        [1] = {
            .command = SET_MANUAL_CMD,
            .fct = set_manual,
        },
    };
    
    
    printf(HEADER);
    printf("--------------------------------------------------\n");
    char command[CMD_LENGTH];
    int arg;
    int parsedArgsCount;
    while(1){
        printf("command >> ");
        parsedArgsCount = scanf("%s %d",command, &arg);
        if(parsedArgsCount == 1){
            if(strcmp(command,"quit") == 0){
            break;
            }
            if(strcmp(command,"help") == 0){
                printf("This CLI app has three commands possibilities which are : \n \
                1. quit -> Quits the app\n \
                2. manual <val> -> Enable/Disable manual mode (values are 0 or 1)\n \
                3. freq <val> -> Sets LED blinking frequency (must be higher than 0)\n");
            }
        }
        else if (parsedArgsCount == 2)
        {
            int len = sizeof(usr_commands)/sizeof(struct usr_command);
            int function_found = 0;
            for(int i = 0; i < len ; i++){
                if(strcmp(command,usr_commands[i].command)==0){
                    usr_commands[i].fct(arg);
                    function_found++;
                    break;
                }
            }
            if(function_found == 0){
                printf(INVALID_COMMAND);
            }
        }
        else{
            printf(INVALID_COMMAND);
        }
    }
    // printf("Sending \"FIFO_SET_MANUAL\" with value 1\n Press [ENTER] to continue\n");
    // while(getchar() != '\n') ;
    // fifo_write(&mycommand, sizeof(mycommand));

    // mycommand.value = 0;
    // printf("Sending \"FIFO_SET_MANUAL\" with value 0\n Press [ENTER] to continue\n");
    // while(getchar() != '\n');
    // fifo_write(&mycommand, sizeof(mycommand));

    // mycommand.value = 1;
    // printf("Sending \"FIFO_SET_MANUAL\" with value 1\n Press [ENTER] to continue\n");
    // while(getchar() != '\n');
    // fifo_write(&mycommand, sizeof(mycommand));

    // mycommand.cmd = FIFO_SET_FREQ;
    // mycommand.value = 10;
    // printf("Sending \"FIFO_SET_FREQ\" with value 10\n Press [ENTER] to continue\n");
    // while(getchar() != '\n');
    // fifo_write(&mycommand, sizeof(mycommand));

    // mycommand.cmd = FIFO_SET_FREQ;
    // mycommand.value = 2;
    // printf("Sending \"FIFO_SET_FREQ\" with value 2\n Press [ENTER] to continue\n");
    // while(getchar() != '\n');
    // fifo_write(&mycommand, sizeof(mycommand));


    return 0;
}
