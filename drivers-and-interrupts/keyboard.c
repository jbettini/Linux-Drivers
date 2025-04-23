#include "keyboard.h"

void keyboard_bind_key(keyboard_key_t key)
{
	if (key.keycode > 256)
		kpanic("ERROR: bad parameters to keyboard_register_routine\n");
	current_layout[key.keycode] = (scancode_routine_t){.handler = handler, .key = key};
}

void keyboard_unbind_key(uint8_t keycode) { current_layout[keycode] = UNDEFINED_ROUTINE; }

void keyboard_init(void)
{
	for (int i = 0; i < 256; ++i)
		current_layout[i] = UNDEFINED_ROUTINE;
	keyboard_init_default_table();
	keyboard_remap_layout(default_key_table, KEY_MAX);
}

void keyboard_handle(void)
{
	if (inb(0x64) & 0x01) {          // read status
		uint8_t keycode = inb(0x60); // read data
	}
}
