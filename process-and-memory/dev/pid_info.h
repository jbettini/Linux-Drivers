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
    int32_t pid;
    int32_t state;
    int32_t parent_pid;
    uint64_t utime;
    uint64_t stime;
    uint64_t total_time;
    uint64_t stack_ptr;
    
    uint32_t nb_children;
    uint32_t children[MAX_CHILDREN];
    
    uint32_t root_len;
    uint8_t root[PATH_MAX];
    uint32_t pwd_len;
    uint8_t pwd[PATH_MAX];
};

#endif
