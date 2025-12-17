// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include "pid_info.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jbettini");
MODULE_DESCRIPTION("Misc device with the prupose of handle logic of the project");

long handleIoctl(struct file *f, unsigned int cmd, unsigned long arg);

// -----------------------------------------------------------
// Operations basics

static int my_open(struct inode *node, struct file *file)
{
	pr_info("Misc Device number %d %d - Open function called\n", imajor(node), iminor(node));
	if (file->f_mode & FMODE_READ)
		pr_info("Misc Device - Open function called with read permissions\n");
	if (file->f_mode & FMODE_WRITE)
		pr_info("Misc Device - Open function called with write permissions\n");
	return 0;
}

static int my_close(struct inode *node, struct file *file)
{
	pr_info("Misc Device number %d %d - Close function called\n", imajor(node), iminor(node));
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t user_len, loff_t *ppos)
{
	pr_alert("This misc cannot handle write ops.\n");
	return -EACCES;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	pr_alert("This misc not handle read ops atm.\n");
	return -EACCES;
}

// Operations basics
// -----------------------------------------------------------
// Data structures 

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = my_write,
	.read = my_read,
	.open = my_open,
	.release = my_close,
	.unlocked_ioctl = handleIoctl
};

static struct miscdevice my_misc = {
	.name = "proc-mem-dev",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

// Data structures 
// -----------------------------------------------------------
// Lifecycle

static int __init proc_and_memory(void)
{
	int status = misc_register(&my_misc);

	if (status) {
		pr_err("Error: misc register\n");
		return -status;
	}
	return 0;
}

static void __exit clean(void)
{
	misc_deregister(&my_misc);
	pr_info("Cleaning up module.\n");
}


// Lifecycle
// -----------------------------------------------------------
// Main logic

// static void traverse_mounts(struct mount *mnt, struct seq_file *m)
// {
//     struct mount *child;
//     struct path path;
//     char *buf = kmalloc(PATH_MAX, GFP_KERNEL);

//     list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
// 	path.mnt = &child->mnt;
// 	path.dentry = path.mnt->mnt_root;
//     	seq_printf(m, "%-15s%s\n", child->mnt_devname, d_path(&path, buf, PATH_MAX));
// 	kfree(buf);
// 	traverse_mounts(child, m);
//     }
// }

long handleIoctl(struct file *f, unsigned int cmd, unsigned long arg) {
	switch (cmd) {
		case GET_PID_INFO:
			printk("Case GET_PID_INFO is Matched.");

			int32_t pid_from_user = 0;
			if (copy_from_user(&pid_from_user, (void *)arg, sizeof(int32_t)))
				return -EFAULT;

			struct pid *pid_struct = find_get_pid(pid_from_user);
			if (!pid_struct)
				return -ESRCH;

			struct task_struct *task = get_pid_task(pid_struct, PIDTYPE_PID);
			put_pid(pid_struct); 
			if (!task)
				return -ESRCH;

			struct pid_info *kinfo = kmalloc(sizeof(struct pid_info), GFP_KERNEL | __GFP_ZERO);
			if (!kinfo)
		        return -ENOMEM;

			kinfo->pid = pid_from_user;
			kinfo->state = READ_ONCE(task->__state);
			kinfo->parent_pid = task->real_parent ? task->real_parent->pid : 0;
			kinfo->stack_ptr = (uint64_t)task->stack;
			kinfo->utime = task->utime;
			kinfo->stime = task->stime;
			kinfo->total_time = (task->stime + task->utime);
			// Logic here parsing of task struct

			if (copy_to_user((void *)arg, kinfo, sizeof(struct pid_info))) {
                kfree(kinfo);
                return -EFAULT;
            }

			kfree(kinfo);
			put_task_struct(task);
			return 0;

		default:
			printk("No command find for this ioctl call.");
			return -ENOTTY;
    }
    return -EINVAL;
}


module_init(proc_and_memory);
module_exit(clean);