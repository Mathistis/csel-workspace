#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <unistd.h>

#define ADDR 0x01c14200

int main()
{
    /* open memory file descriptor */
    int fd = open("/dev/mem", O_RDWR);
    if (fd < 0) {
        printf("File not oppened, error\n");
        return -1;
    }
    size_t page_sz = getpagesize();
    off_t offset   = ADDR -  ADDR % getpagesize();


    volatile uint32_t* regs = mmap(0, page_sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    /*Volatile to say to compiler to not optimize in momory*/
    if (regs == MAP_FAILED)  
    {
        printf("Mmap not opened");
        return -1;
    }

    uint32_t chip[4] = {
        [0] = *(regs + (ADDR % page_sz + 0x00) / sizeof(uint32_t)),
        [1] = *(regs + (ADDR % page_sz + 0x04) / sizeof(uint32_t)),
        [2] = *(regs + (ADDR % page_sz + 0x08) / sizeof(uint32_t)),
        [3] = *(regs + (ADDR % page_sz + 0x0c) / sizeof(uint32_t)),
    };

    printf("ID=%08x'%08x'%08x'%08x\n",
           chip[0],
           chip[1],
           chip[2],
           chip[3]);

    munmap((void*)regs, page_sz);
    close(fd);

    return 0;
}