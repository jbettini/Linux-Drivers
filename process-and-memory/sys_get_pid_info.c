// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/fs_struct.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/pid_info.h>

static void get_child(struct task_struct *task, struct pid_info *kinfo);
static void get_fs_path(struct path *path, char *buffer);

static void get_child(struct task_struct *task, struct pid_info *kinfo)
{
    struct task_struct *child_task;
    int count = 0;

    list_for_each_entry(child_task, &task->children, sibling) {
        if (count < MAX_CHILDREN) {
            kinfo->children[count] = child_task->pid;
            count++;
        }
    }
    kinfo->nb_children = count;
}

static void get_fs_path(struct path *path, char *buffer) {
    char *res = d_path(path, buffer, PATH_MAX);
    if (IS_ERR(res))
        buffer[0] = '\0';
    else
        memmove(buffer, res, strlen(res) + 1);
}

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, user_info, int, target_pid)
{
    long s;
    struct pid *pid_struct;
    struct task_struct *task;
    struct pid_info *kinfo;

    pid_struct = find_get_pid(target_pid);
    if (!pid_struct)
        return -ESRCH;

    task = get_pid_task(pid_struct, PIDTYPE_PID);
    put_pid(pid_struct); 
    if (!task)
        return -ESRCH;

    kinfo = kmalloc(sizeof(struct pid_info), GFP_KERNEL | __GFP_ZERO);
    if (!kinfo) {
        put_task_struct(task);
        return -ENOMEM;
    }

    kinfo->pid = target_pid;											// PID
    
    s = READ_ONCE(task->__state);										// State
    if (s == 0)
		kinfo->state = 0;      												// Running
    else if (s <= 2)
		kinfo->state = 1; 													// Sleeping
    else
		kinfo->state = 2;             										// Zombie/Dead

    kinfo->stack_ptr = (long long)task->stack;							// Stack Ptr
    kinfo->total_time = ktime_get_ns() - task->start_time;				// Age

    get_child(task, kinfo);												// Children
    kinfo->parent_pid = task->real_parent ? task->real_parent->pid : 0; // Parent PID

    if (task->fs) {
        get_fs_path(&task->fs->root, kinfo->root);						// Root Path
        get_fs_path(&task->fs->pwd, kinfo->pwd);						// Pwd Path
    }

    if (copy_to_user((void *)user_info, kinfo, sizeof(struct pid_info))) {
        kfree(kinfo);
        put_task_struct(task);
        return -EFAULT;
    }

    kfree(kinfo);
    put_task_struct(task);
    return 0;
}