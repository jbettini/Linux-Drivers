#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
    int fd;
    int32_t value, number;

    printf("\nOpening Driver\n");
    fd = open("/dev/proc-mem-dev", O_RDWR);
    if(fd < 0) {
            printf("Cannot open device file...\n");
            return 0;
    }
    int ret = 0;
    
    ret = ioctl(fd, 1, 0);
    if (ret == 0)
        printf("SUCESS\n");
    else
        printf("ERROR\n");
    close(fd);
}
