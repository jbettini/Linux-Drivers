#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include "pid_info.h"

#define NS_TO_S(ns) (ns / 1000000000.0)

int main()
{
    int fd;
    char input[32];

    fd = open("/dev/proc-mem-dev", O_RDWR);
    if(fd < 0) {
        perror("Cannot open misc");
        return 1;
    }

    struct pid_info *info = malloc(sizeof(struct pid_info));
    if (!info) {
        perror("No space left");
        close(fd);
        return 1;
    }
    
    memset(info, 0, sizeof(struct pid_info));

    printf("PID to use: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        free(info);
        close(fd);
        return 1;
    }
    info->pid = atoi(input);
    printf("\n------------------------\n");

    int ret = ioctl(fd, GET_PID_INFO, info);

    if (ret < 0) {
        printf("Error catched : %s (code %d)\n", strerror(errno), errno);
    } else {
        printf("PID    : %d\n", info->pid);
        printf("State   : %d\n", info->state);
        printf("Parent : %d\n", info->parent_pid);
        printf("Child: %d find\n", info->nb_children);
        
        for(int i=0; i < info->nb_children && i < 5; i++)
            printf("  > Enfant %d: %d\n", i, info->children[i]);
    
        printf("Stack addr : %p\n", (void *)info->stack_ptr);
        printf("Time in user mode : %.3fs\n", NS_TO_S(info->utime));
        printf("Time in sys mode : %.3fs\n", NS_TO_S(info->stime));
        printf("Total time: %.3fs\n", NS_TO_S(info->total_time));

        printf("Root_path : %s.\n", info->root);
        printf("Pwd : %s.\n", info->pwd);
}
    
    free(info);
    close(fd);
    return 0;
}