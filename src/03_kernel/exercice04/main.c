#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


static const char* text =
    "Considérant la fragilité contextuelle, je recommande\n" 
    "d'examiner la somme des stratégies emblématiques, dans une perspective correcte\n";

static const char* text2 =
    "Tant que durera la situation conjoncturelle, il est\n"
    "préférable d'analyser la somme des problématiques de bon sens, parce que nous le valons bien.\n";

static const char* text3 =
    "Avec la crise observée, il est préférable de considérer\n"
    "les relations des organisations matricielles opportunes, à long terme.\n";

int main(int argc, char* argv[])
{
    int s;
    if (argc <= 1) return 0;

    int write_fd = open(argv[1], O_RDWR);
    write(write_fd, text, strlen(text));
    write(write_fd, text2, strlen(text2));
    do {
        s = write(write_fd, text3, strlen(text3));
    } while (s >= 0); //writing until we obtain EIO
    close(write_fd);

    int read_fd = open(argv[1], O_RDONLY);
    while (1) {
        char buff[100];
        ssize_t len = read(read_fd, buff, sizeof(buff) - 1);
        if (len <= 0) break;
        buff[sizeof(buff) - 1] = 0;
        printf("%s", buff);
    }
    close(read_fd);

    return 0;
}