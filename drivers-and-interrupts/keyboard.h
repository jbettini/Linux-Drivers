#pragma once

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <asm/irq.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/input.h>
#include "utils.h"

/*
 * PS/2 Controller Configuration Byte (exemple : 0b01010111)
 * 
 * Bit 7 : 0  - Non utilisé (réservé, toujours 0)
 * Bit 6 : 1  - Translation activée (le contrôleur convertit les scancodes Set 2 du clavier en Set 1 pour l’hôte)
 * Bit 5 : 0  - Horloge du port 2 (souris) activée (0 = horloge active, 1 = horloge désactivée)
 * Bit 4 : 1  - Horloge du port 1 (clavier) désactivée (0 = horloge active, 1 = horloge désactivée)
 * Bit 3 : 0  - Non utilisé (réservé, toujours 0)
 * Bit 2 : 1  - IRQ du port 2 (souris) activée (1 = interruptions activées, 0 = désactivées)
 * Bit 1 : 1  - IRQ du port 1 (clavier) activée (1 = interruptions activées, 0 = désactivées)
 * Bit 0 : 1  - Buffer du port 1 (clavier) activé (1 = buffer activé, 0 = désactivé)
 *
 * Représentation binaire : 0b01010111
 */

#define PS2_CONF 0b01010111
#define PRESS 1
#define RELEASE 0


typedef enum {
	KEY_LETTERS = 0,
	KEY_DIGIT,
	KEY_PUNCTUATION,
	KEY_SPACES,
	KEY_NUMPAD,
    KEY_CONTROL,
    KEY_FUNCTION,
    KEY_TOGGLE,
    KEY_SPECIAL,
    KEY_NAVIGATION
} KeyCategory;

typedef struct keyboard_key {
	uint16_t        value;
    char            *name;
	uint16_t        keycode;
    bool            state;
	KeyCategory     category;
    size_t          usage;
}   keyboard_key_t;

typedef struct inputs_t {
    t_list *inputs_lst;
    char   *inputs_buffer;
    size_t ibuf_len;
} inputs_t;


void print_key(keyboard_key_t key);
void keyboard_set_key(keyboard_key_t *dst, keyboard_key_t src, int state);
void keyboard_init_key_table(void);
void keyboard_flush_ps2(void);
void keyboard_load_ps2_conf(int conf);
void keyboard_init_released_set1(void);
int	keyboard_init(void);
int keyboard_input_init(void);
irqreturn_t keyboard_handler(int irq, void *dev_id);
void keyboard_add_to_ibuf(char *current_input, int len);
void keyboard_save_input(keyboard_key_t key);
struct tm keyboard_get_current_time(void);

extern keyboard_key_t key_table[256];
extern struct mutex mut;
extern inputs_t inputs;
extern struct input_dev *kbd_input_dev;