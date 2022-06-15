#include <linux/cdev.h>   /* needed for char device driver */
#include <linux/device.h> /* needed for sysfs handling */
#include <linux/fs.h>     /* needed for device drivers */
#include <linux/init.h>   /* needed for macros */
#include <linux/kernel.h> /* needed for debugging */
#include <linux/miscdevice.h>
#include <linux/module.h>          /* needed by all modules */
#include <linux/platform_device.h> /* needed for sysfs handling */
#include <linux/uaccess.h>         /* needed to copy data to/from user */
#include "controller.h"

static int is_manu = MODE_AUTO;
static int freq_manual = 1;

ssize_t sysfs_show_is_manu(struct device *dev,
                           struct device_attribute *attr,
                           char *buf)
{
    sprintf(buf, "%d\n", is_manu);
    return strlen(buf);
}
ssize_t sysfs_store_is_manu(struct device *dev,
                            struct device_attribute *attr,
                            const char *buf,
                            size_t count)
{
    is_manu = simple_strtol(buf, 0, 10) == 1 ? MODE_MANUAL : MODE_AUTO;

    return count;
}

ssize_t sysfs_show_freq(struct device *dev,
                        struct device_attribute *attr,
                        char *buf)
{
    sprintf(buf, "%d\n", freq_manual);
    return strlen(buf);
}
ssize_t sysfs_store_freq(struct device *dev,
                         struct device_attribute *attr,
                         const char *buf,
                         size_t count)
{
    sscanf(buf,
           "%d",
           &freq_manual);
    return count;
}

static struct class *sysfs_class;
static struct device *sysfs_device;

DEVICE_ATTR(is_manu, 0664, sysfs_show_is_manu, sysfs_store_is_manu);
DEVICE_ATTR(freq, 0664, sysfs_show_freq, sysfs_store_freq);

static int __init skeleton_init(void)
{
    int status = 0;

    sysfs_class = class_create(THIS_MODULE, "led_class");
    sysfs_device = device_create(sysfs_class, NULL, 0, NULL, "led_device");
    if (status == 0)
        status = device_create_file(sysfs_device, &dev_attr_is_manu);
    if (status == 0)
        status = device_create_file(sysfs_device, &dev_attr_freq);
    if (status == 0)
        status = init_controller(&is_manu, &freq_manual);
    set_mode(MODE_AUTO);
    pr_info("Linux module skeleton loaded\n");
    return 0;
}

static void __exit skeleton_exit(void)
{
    stop_controller();
    device_remove_file(sysfs_device, &dev_attr_is_manu);
    device_remove_file(sysfs_device, &dev_attr_freq);
    device_destroy(sysfs_class, 0);
    class_destroy(sysfs_class);

    pr_info("Linux module skeleton unloaded\n");
}

module_init(skeleton_init);
module_exit(skeleton_exit);

MODULE_AUTHOR("Remy Macherel & Mathis Raemy");
MODULE_DESCRIPTION("Module skeleton");
MODULE_LICENSE("GPL");