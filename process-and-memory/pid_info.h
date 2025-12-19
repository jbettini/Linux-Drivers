#ifndef PID_INFO_H
#define PID_INFO_H

#include <linux/limits.h>

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define MAX_CHILDREN 4096
#define GET_PID_INFO 1

struct pid_info {
    int pid;
    int state;
    int parent_pid;
    long long total_time;
    long long stack_ptr;
    
    unsigned int nb_children;
    int children[MAX_CHILDREN];
    
    char root[PATH_MAX];
    char pwd[PATH_MAX];
};

#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef NB_get_pid_info
    #define NB_get_pid_info 463
#endif

static inline long get_pid_info(struct pid_info *info, int pid) {
    return syscall(NB_get_pid_info, info, pid);
}

#endif
