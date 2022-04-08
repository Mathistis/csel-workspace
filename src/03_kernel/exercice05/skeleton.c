/* skeleton.c */
#include <linux/cdev.h>   /* needed for char device driver */
#include <linux/device.h> /* needed for sysfs handling */
#include <linux/fs.h>     /* needed for device drivers */
#include <linux/init.h>   /* needed for macros */
#include <linux/kernel.h> /* needed for debugging */
#include <linux/miscdevice.h>
#include <linux/module.h>          /* needed by all modules */
#include <linux/platform_device.h> /* needed for sysfs handling */
#include <linux/uaccess.h>         /* needed to copy data to/from user */




static struct class* myclass;
static struct device* mydevice;
static int val;
ssize_t tatooine_show(struct device* dev,
                       struct device_attribute* attr,
                       char* buf)
{
    sprintf(buf, "%d\n", val);
    return strlen(buf);
}

ssize_t tatooine_store(struct device* dev,
                        struct device_attribute* attr,
                        const char* buf,
                        size_t count)
{
    val = simple_strtol(buf, 0, 10);
    return count;
}

static DEVICE_ATTR(tatooine, 0664, tatooine_show, tatooine_store);

static int __init skeleton_init(void)
{
    int err = 0;
    pr_info("Linux module 01 skeleton loaded\n");
    myclass = class_create(THIS_MODULE, "MySuperModule");
    mydevice = device_create(myclass, NULL, 0, NULL, "MySuperDevice");
    err = device_create_file (mydevice, &dev_attr_tatooine);
    return 0;
}

static void __exit skeleton_exit(void)
{
    device_remove_file (mydevice, &dev_attr_tatooine);
    device_destroy(myclass, 0);
    class_destroy(myclass);
    pr_info("Linux module skeleton unloaded\n");
}

module_init(skeleton_init);
module_exit(skeleton_exit);

MODULE_AUTHOR("Raemy Mathis, Remy Macherel");
MODULE_DESCRIPTION("Module skeleton");
MODULE_LICENSE("GPL");