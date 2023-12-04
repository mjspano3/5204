#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/mm.h>



#define PROC_FILENAME "5204"
#define LOG_PREFIX "ko5204: "

static struct proc_dir_entry *proc_entry;

pte_t *va2pte(struct mm_struct *mm, unsigned long addr);

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    return count;
}

static const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .write = proc_write,
};

static int __init ko5204_init(void) {
    proc_entry = proc_create("5204", 0660, NULL, &proc_fops);
    if (!proc_entry) {
        return -ENOMEM;
    }

    printk(KERN_INFO LOG_PREFIX "Module initialized\n");
    return 0;
}

static void __exit ko5204_exit(void) {
    if (proc_entry) {
        remove_proc_entry(PROC_FILENAME, NULL);
    }
    printk(KERN_INFO LOG_PREFIX "Module exited\n");
}

module_init(ko5204_init);
module_exit(ko5204_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tripp Spano");
MODULE_DESCRIPTION("Kernel Module for /proc/5204 communication");
