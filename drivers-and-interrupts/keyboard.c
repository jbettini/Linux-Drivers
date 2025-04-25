#include "keyboard.h"

void keyboard_init_released(void)
{
	for (int i = 0; i < 256; ++i) {
		if (press_keys[i].keycode != 0) {
			uint8_t keycode = (i | 0x80);
			keyboard_key_t *key = &release_keys[keycode];
			key->value = press_keys[i].value;
			key->name = press_keys[i].name;
			key->keycode = keycode;
			key->status = RELEASE;

		}
	}
}

irqreturn_t keyboard_handler(int irq, void *dev_id)
{
	if (inb(0x64) & 0x01) {          										// read status
		uint8_t keycode = inb(0x60); 										// read data
		if (release_keys[keycode].keycode != 0) {
			printk(KERN_ALERT "Key number %d pressed\n", keycode);
		} else if (release_keys[keycode].keycode != 0) {
			printk(KERN_ALERT "Key number %d pressed\n", keycode);
		} else {
			printk(KERN_ALERT "Current layout did not handle the keycode number %d\n", keycode);		
		}
	}
	return IRQ_HANDLED;
}
