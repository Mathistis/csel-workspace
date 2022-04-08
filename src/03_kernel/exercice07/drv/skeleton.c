/* skeleton.c */
#include <linux/cdev.h>      /* needed for char device driver */
#include <linux/fs.h>        /* needed for device drivers */
#include <linux/gpio.h>      /* needed for i/o handling */
#include <linux/init.h>      /* needed for macros */
#include <linux/interrupt.h> /* needed for interrupt handling */
#include <linux/io.h>        /* needed for mmio handling */
#include <linux/ioport.h>    /* needed for memory region handling */
#include <linux/kernel.h>    /* needed for debugging */
#include <linux/miscdevice.h>
#include <linux/module.h>  /* needed by all modules */
#include <linux/poll.h>    /* needed for polling handling */
#include <linux/sched.h>   /* needed for scheduling constants */
#include <linux/uaccess.h> /* needed to copy data to/from user */
#include <linux/wait.h>    /* needed for wating */

#define K1	0
#define K2	2
#define K3	3

static char* k1="gpio_a.0-k1";
static char* k2="gpio_a.2-k2";
static char* k3="gpio_a.3-k3";

static atomic_t pressure_btn;
DECLARE_WAIT_QUEUE_HEAD(q);


irqreturn_t gpio_interrupt(int irq, void* handle)
{
	atomic_inc(&pressure_btn);
    wake_up_interruptible(&q);
	pr_info("WARNING ! Seem that the pressus on GPIO %s is very high !\n", (char*) handle);
	return IRQ_HANDLED;
}



static ssize_t skeleton_read(struct file *f,
                             char __user *buf,
                             size_t count,
                             loff_t *off)
{
	return 0;
}

static unsigned int skeleton_poll(struct file* f, poll_table* wait)
{
    unsigned mask = 0;
    poll_wait(f, &q, wait);
    if (atomic_read(&pressure_btn) != 0) {
        mask |= POLLIN | POLLRDNORM; /* read operation */
        atomic_dec(&pressure_btn);
        pr_info("Good Morning !!\n");
    }
    return mask;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = skeleton_read,
    .poll  = skeleton_poll,
};

struct miscdevice gpio_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .fops  = &fops,
    .name  = "mymodule",
    .mode  = 0777,
};

static int __init skeleton_init(void)
{
	int err = 0;
	atomic_set(&pressure_btn, 0);
	err = misc_register(&gpio_device);
	if(!err) err = devm_request_irq(
		gpio_device.this_device,
		gpio_to_irq(K1),
		gpio_interrupt,
		IRQF_TRIGGER_FALLING | IRQF_SHARED,
		k1,
		k1);
	if(!err) err = devm_request_irq(
		gpio_device.this_device,
		gpio_to_irq(K2),
		gpio_interrupt,
		IRQF_TRIGGER_FALLING | IRQF_SHARED,
		k2,
		k2);
	if(!err) err = devm_request_irq(
		gpio_device.this_device,
		gpio_to_irq(K3),
		gpio_interrupt,
		IRQF_TRIGGER_FALLING | IRQF_SHARED,
		k3,
		k3);

	pr_info("Linux module skeleton loaded(status=%d)\n", err);
	return err;
}

static void __exit skeleton_exit(void)
{
    
	misc_deregister(&gpio_device);
    pr_info("Linux module skeleton unloaded\n");

}

module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_AUTHOR ("Raemy Mathis , Macherel Rémy");
MODULE_DESCRIPTION ("Module skeleton");
MODULE_LICENSE ("GPL");