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
// static DEFINE_MUTEX(proc_mutex);

pte_t *va2pte(struct mm_struct *mm, unsigned long addr);

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    // char *input;
    // unsigned long va = 0;

    // input = kmalloc(count + 1, GFP_KERNEL);
    // if (!input)
    //     return -ENOMEM;

    // if (copy_from_user(input, buffer, count)) {
    //     kfree(input);
    //     return -EFAULT;
    // }

    // input[count] = '\0';

    // if (kstrtoul(input, 0, &va) != 0) {
    //     kfree(input);
    //     return -EINVAL;
    // }

    // // Perform translation and measure latency
    // ktime_t start_time = ktime_get();
    // // Perform translation logic here with the provided virtual address 'va'

    // pte_t *ptep = va2pte(current->mm, va);
    // unsigned long paddr = 0;
    // unsigned long page_addr = 0;
    // unsigned long page_offset = 0;
    // page_addr = pte_val(*ptep) & PAGE_MASK;
    // page_offset = va & ~PAGE_MASK;
    // paddr = page_addr | page_offset;

    // ktime_t end_time = ktime_get();
    // ktime_t latency = ktime_sub(end_time, start_time);

    // // Log latency to kernel log
    // printk(KERN_INFO LOG_PREFIX "Translation latency for VA 0x%lx: %lld ns\n", va, latency);
    // kfree(input);
    return count;
}

static const struct file_operations proc_fops = {
    .write = proc_write,
    .owner = THIS_MODULE,
};

static int __init ko5204_init(void) {
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &proc_fops);
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

pte_t *va2pte(struct mm_struct *mm, unsigned long addr)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep;

    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd))) {
        printk("Coperd,invalid pgd [%p]\n", pgd);
        goto out;
    }

    p4d = p4d_offset(pgd, addr);
    if (p4d_none(*p4d) || unlikely(p4d_bad(*p4d))) {
        printk("Coperd, invalid p4d [%p]", p4d);
        goto out;
    }

    pud = pud_offset(p4d, addr);
    if (pud_none(*pud) || unlikely(pud_bad(*pud))) {
        printk("Coperd, invalid pud [%p]", pud);
        goto out;
    }

    pmd = pmd_offset(pud, addr);
    /* Coperd: TODO: sometimes pmd_bad() is true */
    if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd))) {
        printk("Coperd, invalid pmd [%p]", pmd);
        goto out;
    }

    ptep = pte_offset_map(pmd, addr);
    //ptep = pte_offset_map_lock(mm, pmd, addr, &ptl);
    if (!ptep) {
        printk("Coperd,%d,pt_offset_map() failed\n", __LINE__);
        goto out;
    }

    return ptep;

out:
    return NULL;
}

module_init(ko5204_init);
module_exit(ko5204_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tripp Spano");
MODULE_DESCRIPTION("Kernel Module for /proc/5204 communication");
