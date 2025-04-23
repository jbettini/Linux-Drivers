#pragma once

#define PRESS 1
#define RELEASE 0


typedef struct keyboard_key {
	uint16_t        value;
    char            *name;
	uint16_t        keycode;
    bool            status;
}   keyboard_key_t;

static keyboard_key_t default_key_table[256];

static keyboard_key_t press_keys[] = {

    // letters
    {'a', "A", 0x1E, PRESS},
    {'b', "B", 0x30, PRESS},
    {'c', "C", 0x2E, PRESS},
    {'d', "D", 0x20, PRESS},
    {'e', "E", 0x12, PRESS},
    {'f', "F", 0x21, PRESS},
    {'g', "G", 0x22, PRESS},
    {'h', "H", 0x23, PRESS},
    {'i', "I", 0x17, PRESS},
    {'j', "J", 0x24, PRESS},
    {'k', "K", 0x25, PRESS},
    {'l', "L", 0x26, PRESS},
    {'m', "M", 0x32, PRESS},
    {'n', "N", 0x31, PRESS},
    {'o', "O", 0x18, PRESS},
    {'p', "P", 0x19, PRESS},
    {'q', "Q", 0x10, PRESS},
    {'r', "R", 0x13, PRESS},
    {'s', "S", 0x1F, PRESS},
    {'t', "T", 0x14, PRESS},
    {'u', "U", 0x16, PRESS},
    {'v', "V", 0x2F, PRESS},
    {'w', "W", 0x11, PRESS},
    {'x', "X", 0x2D, PRESS},
    {'y', "Y", 0x15, PRESS},
    {'z', "Z", 0x2C, PRESS},
    // top keysPRESS
    {'1', "1", 0x02, PRESS},
    {'2', "2", 0x03, PRESS},
    {'3', "3", 0x04, PRESS},
    {'4', "4", 0x05, PRESS},
    {'5', "5", 0x06, PRESS},
    {'6', "6", 0x07, PRESS},
    {'7', "7", 0x08, PRESS},
    {'8', "8", 0x09, PRESS},
    {'9', "9", 0x0A, PRESS},
    {'0', "0", 0x0B, PRESS},
    // specials
    {'-',  "MINUS",      0x0C, PRESS},
    {'=',  "EQUALS",     0x0D, PRESS},
    {'[',  "LBRACKET",   0x1A, PRESS},
    {']',  "RBRACKET",   0x1B, PRESS},
    {';',  "SEMICOLON",  0x27, PRESS},
    {'\'', "APOSTROPHE", 0x28, PRESS},
    {'`',  "BACKTICK",   0x29, PRESS}, 
    {'\\', "BACKSLASH",  0x2B, PRESS},
    {',',  "COMMA",      0x33, PRESS},
    {'.',  "PERIOD",     0x34, PRESS},
    {'/',  "SLASH",      0x35, PRESS},
    // Space and Tab
    {' ',  "SPACE", 0x39, PRESS},
    {'\t', "TAB",   0x0F, PRESS},
    // Num pad (main operators)
    {'*', "NUMPAD_ASTERISK", 0x37, PRESS},
    {'-', "NUMPAD_MINUS",    0x4A, PRESS},
    {'+', "NUMPAD_PLUS",     0x4E, PRESS},
    // Control 
    {0, "LEFT_CTRL",   0x1D, PRESS},
    {0, "LEFT_SHIFT",  0x2A, PRESS},
    {0, "RIGHT_SHIFT", 0x36, PRESS},
    {0, "LEFT_ALT",    0x38, PRESS},
    // Toggle (Lock keys)
    {0, "CAPS_LOCK",   0x3A, PRESS},
    {0, "NUM_LOCK",    0x45, PRESS},
    {0, "SCROLL_LOCK", 0x46, PRESS},
    // Nav Keys
    {'7', "NUMPAD_7/HOME",    0x47, PRESS},
    {'8', "NUMPAD_8/ARROW_UP",    0x48, PRESS},
    {'9', "NUMPAD_9/PAGE_UP",     0x49, PRESS},
    {'4', "NUMPAD_4/ARROW_LEFT",  0x4B, PRESS},
    {'5', "NUMPAD_5/CENTER",      0x4C, PRESS},
    {'6', "NUMPAD_6/ARROW_RIGHT", 0x4D, PRESS},
    {'1', "NUMPAD_1/END",         0x4F, PRESS},
    {'2', "NUMPAD_2/ARROW_DOWN",  0x50, PRESS},
    {'3', "NUMPAD_3/PAGE_DOWN",   0x51, PRESS},
    {'0', "NUMPAD_0/INSERT",      0x52, PRESS},
    {'.', "NUMPAD_PERIOD/DELETE", 0x53, PRESS},
    // Functions
    {0, "F1",  0x3B, PRESS},
    {0, "F2",  0x3C, PRESS},
    {0, "F3",  0x3D, PRESS},
    {0, "F4",  0x3E, PRESS},
    {0, "F5",  0x3F, PRESS},
    {0, "F6",  0x40, PRESS},
    {0, "F7",  0x41, PRESS},
    {0, "F8",  0x42, PRESS},
    {0, "F9",  0x43, PRESS},
    {0, "F10", 0x44, PRESS},
    {0, "F11", 0x57, PRESS},
    {0, "F12", 0x58, PRESS},
    // Specials
    {0, "BACKSPACE", 0x0E, PRESS},
    {0, "ENTER",     0x1C, PRESS},
    {0, "ESCAPE",    0x01, PRESS},
    {0, "LEFT_SUPER", 0x5B, PRESS},            // Touche Windows gauche
    {0, "RIGHT_SUPER", 0x5C, PRESS},           // Touche Windows droite
    {0, "MENU", 0x5D, PRESS},                  // Touche menu/application
};