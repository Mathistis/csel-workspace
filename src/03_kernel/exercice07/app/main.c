#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* dev = "/dev/mymodule";
    if (argc > 1) dev = argv[1];
    int fd = open(dev, O_RDWR);
    fd_set select_fd;
    FD_ZERO(&select_fd);

    int interrupts = 0;
    while (1) {
        FD_SET(fd, &select_fd);
        int err = select(fd + 1, &select_fd, 0, 0, 0);
        if (err == -1) printf("error while waiting on signal...\n");
        if (FD_ISSET(fd, &select_fd)) {
            interrupts++;
            printf("Event %d occurred\n", interrupts);
        }
        printf("Hello ! (%d), err=%d...\n", fd, err);
    }

    close(fd);
    return 0;
}