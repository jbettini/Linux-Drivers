#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <pid_info.h>

#define NS_TO_S(ns) (ns / 1000000000.0)

const char *get_state_string(int state) {
    if (state == 0) return "Running";
    if (state == 1) return "Sleeping"; 
    if (state == 2) return "Zombie/Dead";
    return "Unknown";
}

void print_one_line_info(struct pid_info *info) {
    printf("PID: %-5d | PPID: %-5d | State: %-10s | Stack: 0x%llx | Age: %.2fs | Pwd: %s\n",
           info->pid, 
           info->parent_pid, 
           get_state_string(info->state),
           info->stack_ptr,
           NS_TO_S(info->total_time),
           info->pwd);
}


void iterate_childrens(struct pid_info *info) {
    printf("\n------------------------\n");
    if (info->nb_children == 0) {
        printf("\nNo Childrens\n");
        return ;
    }
    printf("\nChildrens\n");
    struct pid_info *tmp_info = malloc(sizeof(struct pid_info));
    
    for (int i = 0; i < info->nb_children; i++) {
        long ret = get_pid_info(tmp_info, info->children[i]);
        if (ret < 0) {
            printf("Error caught : %s (code %d)\n", strerror(errno), errno);
            continue ;
        }
        print_one_line_info(tmp_info);
        memset(tmp_info, 0, sizeof(struct pid_info));
    }
    free(tmp_info);
}

void iterate_parents(struct pid_info *info) {
    printf("\n------------------------\n");
    int current_parent = info->parent_pid;
    if (current_parent == 0) {
        printf("\nNo Parents\n");
        return ;
    }
    printf("\nParents\n");
    
    struct pid_info *tmp_info = malloc(sizeof(struct pid_info));

    while (current_parent > 0) {
        long ret = get_pid_info(tmp_info, current_parent);
        if (ret < 0) {
            printf("Error caught : %s (code %d)\n", strerror(errno), errno);
            continue ;
        }
        print_one_line_info(tmp_info);
        current_parent = tmp_info->parent_pid;
        memset(tmp_info, 0, sizeof(struct pid_info));

    }
    free(tmp_info);
}

int main()
{
    char input[32];
    int target_pid;

    struct pid_info *info = malloc(sizeof(struct pid_info));
    if (!info) {
        perror("No space left");
        return 1;
    }
    
    memset(info, 0, sizeof(struct pid_info));

    printf("PID to use: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        free(info);
        return 1;
    }
    target_pid = atoi(input);

    printf("\n------------------------\n");

    long ret = get_pid_info(info, target_pid);

    if (ret < 0) {
        printf("Error caught : %s (code %d)\n", strerror(errno), errno);
    } else {
        printf("PID    : %d\n", info->pid);
        printf("State  : %s (%d)\n", get_state_string(info->state), info->state);
        printf("Parent : %d\n", info->parent_pid);
        printf("Child  : %d found\n", info->nb_children);
        
        for(unsigned int i=0; i < info->nb_children && i < 5; i++)
            printf("  > Enfant %d: %d\n", i, info->children[i]);
    
        printf("Stack addr : 0x%llx\n", info->stack_ptr);
        printf("Age        : %.3fs\n", NS_TO_S(info->total_time));

        printf("Root_path  : %s\n", info->root);
        printf("Pwd        : %s\n", info->pwd);
    }
    iterate_childrens(info);
    iterate_parents(info);
    free(info);
    return 0;
}