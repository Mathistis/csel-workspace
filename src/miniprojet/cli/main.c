#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MODE 0777
#define PATH "/workspace/src/miniprojet/ipc"
#define FIFO_SET_MANUAL 0x14
#define FIFO_SET_FREQ 0x15
#define SET_FREQ_CMD "freq"
#define QUIT_CMD "quit"
#define HELP_CMD "help"
#define CMD_FORMAT "%s %d"
#define SET_MANUAL_CMD "manual"
#define INVALID_COMMAND "Invalid command\n"
#define CMD_LENGTH 20
#define USR_CMD_NUMBER 2
#define HEADER "CSEL project CLI - Macherel & Raemy\n Enter command help for informations\n"

struct IPC_Command
{
    int cmd;
    int value;
};

int fifo_open()
{

    int fd = open(PATH, O_WRONLY | O_NONBLOCK |O_CREAT, MODE);
    if (fd < 0)
    {
        printf("Can't open FIFO " PATH " %d\n", fd);
    }
    return fd;
}

int fifo_write(void *buf, int size)
{
    int fd = fifo_open();
    int readen = write(fd, buf, size);
    close(fd);
    if (readen == size)
        return 0;
    return -1;
}

struct usr_command
{
    char command[CMD_LENGTH];
    int nArgs;
    void (*fct)(int);
};

void set_frequency(int freq)
{
    if (freq < 0)
    {
        printf("Invalid frequency value, it has to be greater than 0 !\n");
        return;
    }
    else if (freq > 100)
    {
        printf("Careful, frequency over 100Hz may not be visible but it will be set anyway \n");
    }

    struct IPC_Command freq_command = {
        .cmd = FIFO_SET_FREQ,
        .value = freq,
    };
    fifo_write(&freq_command, sizeof(freq_command));
}

void set_manual(int val)
{
    if (val == 0 || val == 1)
    {
        struct IPC_Command manual_command = {
            .cmd = FIFO_SET_MANUAL,
            .value = val,
        };
        fifo_write(&manual_command, sizeof(manual_command));
    }
    else
    {
        printf("Invalid value of manual command ! Remember it has to be 0 or 1 \n");
    }
}

void quit(int useless)
{
    (void)useless;
    exit(0);
}
void help(int useless)
{
    (void)useless;
    printf("This CLI app has three commands possibilities which are : \n \
                1. quit -> Quits the app\n \
                2. manual <val> -> Enable/Disable manual mode (values are 0 or 1)\n \
                3. freq <val> -> Sets LED blinking frequency (must be higher than 0)\n");
}

int parseArgs(char command[CMD_LENGTH], int *args)
{
    char tmp[CMD_LENGTH] = "";
    strcpy(tmp, command);
    return sscanf(tmp, CMD_FORMAT, command, args);
    
}

int main()
{

    struct usr_command usr_commands[] = {
        [0] = {
            .command = SET_FREQ_CMD,
            .fct = set_frequency,
        },
        [1] = {
            .command = SET_MANUAL_CMD,
            .fct = set_manual,
        },
        [2] = {
            .command = QUIT_CMD,
            .fct = quit,
        },
        [3] = {
            .command = HELP_CMD,
            .fct = help,
        },
    };

    printf(HEADER);
    while (1)
    {
        char command[CMD_LENGTH];
        int args = -1;
        printf("command >> ");
        fgets(command, CMD_LENGTH, stdin);
        parseArgs(command, &args);
        int len = sizeof(usr_commands) / sizeof(struct usr_command);
        int function_found = 0;
        for (int i = 0; i < len; i++)
        {
            if (strcmp(command, usr_commands[i].command) == 0)
            {
                usr_commands[i].fct(args);
                function_found++;
                break;
            }
        }
        if (function_found == 0)
        {
            printf(INVALID_COMMAND);
        }
    }

    return 0;
}
