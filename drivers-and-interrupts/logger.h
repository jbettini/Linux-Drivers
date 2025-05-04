#pragma once

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include "keyboard.h"
extern struct miscdevice my_misc;

int logger_init(void);
