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

#define PRESS 1
#define RELEASE 0


typedef struct keyboard_key {
	uint16_t        value;
    char            *name;
	uint16_t        keycode;
    bool            status;
}   keyboard_key_t;

void keyboard_init_released(void);
irqreturn_t keyboard_handler(int irq, void *dev_id);

static keyboard_key_t release_keys[256];
static keyboard_key_t press_keys[256] = {

    // letters
    [0x1E] =    {'a',   "A",                    0x1E, PRESS},
    [0x30] =    {'b',   "B",                    0x30, PRESS},
    [0x2E] =    {'c',   "C",                    0x2E, PRESS},
    [0x20] =    {'d',   "D",                    0x20, PRESS},
    [0x12] =    {'e',   "E",                    0x12, PRESS},
    [0x21] =    {'f',   "F",                    0x21, PRESS},
    [0x22] =    {'g',   "G",                    0x22, PRESS},
    [0x23] =    {'h',   "H",                    0x23, PRESS},
    [0x17] =    {'i',   "I",                    0x17, PRESS},
    [0x24] =    {'j',   "J",                    0x24, PRESS},
    [0x25] =    {'k',   "K",                    0x25, PRESS},
    [0x26] =    {'l',   "L",                    0x26, PRESS},
    [0x32] =    {'m',   "M",                    0x32, PRESS},
    [0x31] =    {'n',   "N",                    0x31, PRESS},
    [0x18] =    {'o',   "O",                    0x18, PRESS},
    [0x19] =    {'p',   "P",                    0x19, PRESS},
    [0x10] =    {'q',   "Q",                    0x10, PRESS},
    [0x13] =    {'r',   "R",                    0x13, PRESS},
    [0x1F] =    {'s',   "S",                    0x1F, PRESS},
    [0x14] =    {'t',   "T",                    0x14, PRESS},
    [0x16] =    {'u',   "U",                    0x16, PRESS},
    [0x2F] =    {'v',   "V",                    0x2F, PRESS},
    [0x11] =    {'w',   "W",                    0x11, PRESS},
    [0x2D] =    {'x',   "X",                    0x2D, PRESS},
    [0x15] =    {'y',   "Y",                    0x15, PRESS},
    [0x2C] =    {'z',   "Z",                    0x2C, PRESS},
    // top keysPRESS
    [0x02] =    {'1',   "1",                    0x02, PRESS},
    [0x03] =    {'2',   "2",                    0x03, PRESS},
    [0x04] =    {'3',   "3",                    0x04, PRESS},
    [0x05] =    {'4',   "4",                    0x05, PRESS},
    [0x06] =    {'5',   "5",                    0x06, PRESS},
    [0x07] =    {'6',   "6",                    0x07, PRESS},
    [0x08] =    {'7',   "7",                    0x08, PRESS},
    [0x09] =    {'8',   "8",                    0x09, PRESS},
    [0x0A] =    {'9',   "9",                    0x0A, PRESS},
    [0x0B] =    {'0',   "0",                    0x0B, PRESS},
    // specials
    [0x0C] =    {'-',   "MINUS",                0x0C, PRESS},
    [0x0D] =    {'=',   "EQUALS",               0x0D, PRESS},
    [0x1A] =    {'[',   "LBRACKET",             0x1A, PRESS},
    [0x1B] =    {']',   "RBRACKET",             0x1B, PRESS},
    [0x27] =    {';',   "SEMICOLON",            0x27, PRESS},
    [0x28] =    {'\'',  "APOSTROPHE",           0x28, PRESS},
    [0x29] =    {'`',   "BACKTICK",             0x29, PRESS}, 
    [0x2B] =    {'\\',  "BACKSLASH",            0x2B, PRESS},
    [0x33] =    {',',   "COMMA",                0x33, PRESS},
    [0x34] =    {'.',   "PERIOD",               0x34, PRESS},
    [0x35] =    {'/',   "SLASH",                0x35, PRESS},
    // Space and Tab
    [0x39] =    {' ',   "SPACE",                0x39, PRESS},
    [0x0F] =    {'\t',  "TAB",                  0x0F, PRESS},
    // Num pad (main operators)
    [0x37] =    {'*',   "NUMPAD_ASTERISK",      0x37, PRESS},
    [0x4A] =    {'-',   "NUMPAD_MINUS",         0x4A, PRESS},
    [0x4E] =    {'+',   "NUMPAD_PLUS",          0x4E, PRESS},
    // Control 
    [0x1D] =    {0,     "LEFT_CTRL",            0x1D, PRESS},
    [0x2A] =    {0,     "LEFT_SHIFT",           0x2A, PRESS},
    [0x36] =    {0,     "RIGHT_SHIFT",          0x36, PRESS},
    [0x38] =    {0,     "LEFT_ALT",             0x38, PRESS},
    // Toggle (Lock keys)
    [0x3A] =    {0,     "CAPS_LOCK",            0x3A, PRESS},
    [0x45] =    {0,     "NUM_LOCK",             0x45, PRESS},
    [0x46] =    {0,     "SCROLL_LOCK",          0x46, PRESS},
    // Nav Keys
    [0x47] =    {'7',   "NUMPAD_7/HOME",        0x47, PRESS},
    [0x48] =    {'8',   "NUMPAD_8/ARROW_UP",    0x48, PRESS},
    [0x49] =    {'9',   "NUMPAD_9/PAGE_UP",     0x49, PRESS},
    [0x4B] =    {'4',   "NUMPAD_4/ARROW_LEFT",  0x4B, PRESS},
    [0x4C] =    {'5',   "NUMPAD_5/CENTER",      0x4C, PRESS},
    [0x4D] =    {'6',   "NUMPAD_6/ARROW_RIGHT", 0x4D, PRESS},
    [0x4F] =    {'1',   "NUMPAD_1/END",         0x4F, PRESS},
    [0x50] =    {'2',   "NUMPAD_2/ARROW_DOWN",  0x50, PRESS},
    [0x51] =    {'3',   "NUMPAD_3/PAGE_DOWN",   0x51, PRESS},
    [0x52] =    {'0',   "NUMPAD_0/INSERT",      0x52, PRESS},
    [0x53] =    {'.',   "NUMPAD_PERIOD/DELETE", 0x53, PRESS},
    // Functions
    [0x3B] =    {0,     "F1",                   0x3B, PRESS},
    [0x3C] =    {0,     "F2",                   0x3C, PRESS},
    [0x3D] =    {0,     "F3",                   0x3D, PRESS},
    [0x3E] =    {0,     "F4",                   0x3E, PRESS},
    [0x3F] =    {0,     "F5",                   0x3F, PRESS},
    [0x40] =    {0,     "F6",                   0x40, PRESS},
    [0x41] =    {0,     "F7",                   0x41, PRESS},
    [0x42] =    {0,     "F8",                   0x42, PRESS},
    [0x43] =    {0,     "F9",                   0x43, PRESS},
    [0x44] =    {0,     "F10",                  0x44, PRESS},
    [0x57] =    {0,     "F11",                  0x57, PRESS},
    [0x58] =    {0,     "F12",                  0x58, PRESS},
    // Specials
    [0x0E] =    {0,     "BACKSPACE",            0x0E, PRESS},
    [0x1C] =    {0,     "ENTER",                0x1C, PRESS},
    [0x01] =    {0,     "ESCAPE",               0x01, PRESS},
    [0x5B] =    {0,     "LEFT_SUPER",           0x5B, PRESS},
    [0x5C] =    {0,     "RIGHT_SUPER",          0x5C, PRESS},
    [0x5D] =    {0,     "MENU",                 0x5D, PRESS},
};