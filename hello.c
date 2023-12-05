#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tripp Spano");


static struct proc_dir_entry *ent;

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
	
	return count;
}

static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.write = mywrite,
};

static int simple_init(void)
{
	ent=proc_create("cs5204",0660,NULL,&myops);
	return 0;
}

static void simple_cleanup(void)
{
	proc_remove(ent);
}

module_init(simple_init);
module_exit(simple_cleanup);
