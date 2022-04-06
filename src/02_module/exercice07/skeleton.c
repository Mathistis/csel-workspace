// skeleton.c
#include <linux/module.h> // needed by all modules
#include <linux/init.h>	  // needed for macros
#include <linux/kernel.h> // needed for debugging

#include <linux/moduleparam.h> // needed for module parameters

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/wait.h>		/* needed for waitqueues handling */

static char *text = "dummy text";
module_param(text, charp, 0664);
static int elements = 1;
module_param(elements, int, 0);
static struct task_struct *wakeup_task;
static struct task_struct *sleepy_task;
static atomic_t punch;

DECLARE_WAIT_QUEUE_HEAD(queue);

static int sleep_for_ever(void *data)
{
	while (!kthread_should_stop())
	{
		int status = wait_event_interruptible(queue, atomic_read(&punch) != 0);
		if(status == -ERESTARTSYS) {
			break;
			pr_info("Sleepy thread interrupted\n");
		}
		atomic_dec (&punch);
		pr_info("AAaaAaAaarrg !!! WHO THE F WOKE ME UP ?!!!!\n");
	}
	return 0;
}

static int wakeup(void *data)
{
	
	while (!kthread_should_stop())
	{
		pr_info("Kikou ! I'm sleepy zzzzzz\n");
		atomic_set(&punch, 1);
		wake_up_interruptible(&queue);
		ssleep(5);
	}
	return 0;
}

static int __init skeleton_init(void)
{
	pr_info("Linux module 01 skeleton loaded\n");
	pr_debug("  text: %s\n  elements: %d\n", text, elements);
	sleepy_task = kthread_run(&sleep_for_ever, NULL, "Cool Thread that sleep ");
	wakeup_task = kthread_run(&wakeup, NULL, "Cool Thread that wakeup");

	return 0;
}

static void __exit skeleton_exit(void)
{
	int err = 0;
	err = kthread_stop (sleepy_task);
	err |= kthread_stop (wakeup_task);
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