// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>

#define LOGIN_SIZE 8
#define LOGIN "jbettini"

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

long handleIoctl(struct file *f, unsigned int cmd, unsigned long arg) {
	switch (cmd) {
		case 1:
			printk("Case 1 is Matched.");
			return 0;
		default:
			printk("No command find for this ioctl call.");
			return -ENOTTY;
    }
    return -EINVAL;
}


module_init(proc_and_memory);
module_exit(clean);