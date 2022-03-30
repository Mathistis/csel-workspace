// skeleton.c
#include <linux/module.h> // needed by all modules
#include <linux/init.h>	  // needed for macros
#include <linux/kernel.h> // needed for debugging

#include <linux/moduleparam.h> // needed for module parameters

#include <linux/delay.h>
#include <linux/kthread.h>

static char *text = "dummy text";
module_param(text, charp, 0664);
static int elements = 1;
module_param(elements, int, 0);
static struct task_struct *dummy_task;
static int my_dummy_thread(void *data)
{
	
	while (!kthread_should_stop())
	{
		pr_info("Kikou ! I'm sleepy zzzzzz\n");
		ssleep(5);
	}
	return 0;
}

static int __init skeleton_init(void)
{
	pr_info("Linux module 01 skeleton loaded\n");
	pr_debug("  text: %s\n  elements: %d\n", text, elements);
	dummy_task = kthread_run(&my_dummy_thread, NULL, "Cool Thread");

	return 0;
}

static void __exit skeleton_exit(void)
{
	int err = 0;
	err = kthread_stop (dummy_task);
	if (err){
		pr_debug("Can't stop dummy thread\n");
	}
	pr_info("Linux module skeleton unloaded\n");
}

module_init(skeleton_init);
module_exit(skeleton_exit);

MODULE_AUTHOR("Raemy Mathis, Remy Macherel");
MODULE_DESCRIPTION("Module skeleton");
MODULE_LICENSE("GPL");