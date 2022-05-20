#define _GNU_SOURCE /* See feature_test_macros(7) */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <sched.h>

#define DEBUG_CPU_USED

#define MSG_MAX_SIZE 256
#define ALERT_SITHS "ALERT Segnor sith invade Tatooine\n"
#define EXIT "Too late! Exit the planet !\n"
#define I_AM_YOUR_FATHER "Luke I'am your father !\n"
#define YOU_ARE_WRONG "No ! You are a liar ! AaaAAaaaAAaaaaaAaaaAAAaaa\n"
#define SIG_INTERRUPT "[Empire] Signal rebel intercepted\n"
void catch_signal(int signo)
{
    printf(SIG_INTERRUPT);
    printf("[Empire] signal number %d\n", signo);
}

int Anakin(int fd)
{
    int len = write(fd, I_AM_YOUR_FATHER, strlen(I_AM_YOUR_FATHER));
    if (len < 0)
        printf("Write error");
    sleep(1);
    len = write(fd, ALERT_SITHS, strlen(ALERT_SITHS));
    if (len < 0)
        printf("Write error");

    return 0;
}
int Luke(int fd)
{
    int isInvasionInProgress = 0;
    // char buf[MSG_MAX_SIZE];
    while (!isInvasionInProgress)
    {
        char *buf = calloc(MSG_MAX_SIZE, sizeof(char));
        int len = read(fd, buf, MSG_MAX_SIZE);
        if (len > 0)
        {
            printf("[New message] %s", buf);

            if (!strcmp(buf, I_AM_YOUR_FATHER))
            {
                printf(YOU_ARE_WRONG);
            }

            if (!strcmp(buf, ALERT_SITHS))
            {
                printf(EXIT);
                isInvasionInProgress = 1;
            }
        }
        else
        {
            printf("Nothing \n");
        }
        free(buf);
        // sleep(10);
    }
    return 0;
}
struct sigaction clone_counter_attack = {
    .sa_handler = catch_signal,
};

int main()
{

    for (int i = 1; i < 65; i++)
    {
        if ((i != SIGKILL) && (i != SIGSTOP) && (i != 32) && (i != 33))
        {
            sigaction(i, &clone_counter_attack, NULL);
        }
    }
    int fd[2];
    int err = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    if (err == -1)
    {
        printf("socketpair = -1");
        exit(-1);
    }
    pid_t pid = fork();
    if (pid == 0)
    {

        /* code de l’enfant */
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(1, &set);
        int ret = sched_setaffinity(0, sizeof(set), &set);
        if (ret == -1)
            printf("Error set_affinity == -1");
        /* error */

        Luke(fd[0]);
        #ifdef DEBUG_CPU_USED
        int i = 10224200;
        while (i--)
        {
            int a = i * i;
            if (a > 0){
                a = 20;
            }else {
                a = -1;
            }
            close(1);
            printf("asdasdasdasdasdasd");
        }
        #endif
    }
    else if (pid > 0)
    {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(2, &set);
        int ret = sched_setaffinity(0, sizeof(set), &set);
        if (ret == -1)
            printf("Error set_affinity == -1");
        /* error */
        /* code du parent */
        Anakin(fd[1]);
        int status = 0;
        #ifdef DEBUG_CPU_USED
        int i = 10224200;
        while (i--)
        {
            int a = i * i;
            if (a > 0){
                a = 20;
            }else {
                a = -1;
            }
            close(1);
            printf("asdasdasdasdasdasd");
        }
        #endif
        wait(&status);
    }
    else
    {
        printf("fork() = -1");
        exit(-1);
        /* error */
    }
}