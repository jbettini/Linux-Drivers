#pragma once

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include "keyboard.h"
#include "utils.h"

extern struct miscdevice my_misc;

typedef struct key_category_stats_s {
    size_t letters;
    size_t digit;
    size_t punctuation;
    size_t spaces;
    size_t numpad;
    size_t control;
    size_t function;
    size_t toggle;
    size_t special;
    size_t navigation;
    size_t total;
    size_t erased;
    keyboard_key_t most_used[5];
} key_category_stats_t;

int logger_init(void);
void logger_stats(keyboard_key_t key);
void logger_final_log(void);