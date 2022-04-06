/* skeleton.c */
#include <linux/cdev.h>        /* needed for char device driver */
#include <linux/fs.h>          /* needed for device drivers */
#include <linux/init.h>        /* needed for macros */
#include <linux/kernel.h>      /* needed for debugging */
#include <linux/module.h>      /* needed by all modules */
#include <linux/moduleparam.h> /* needed for module parameters */
#include <linux/slab.h>        /* needed for dynamic memory management */
#include <linux/uaccess.h>     /* needed to copy data to/from user */

#define BUFFER_SZ 10000
static dev_t skeleton_dev;
static struct cdev skeleton_cdev;

static int instances = 3;
static char** buffers = 0;

module_param(instances, int, 0);

static int skeleton_open(struct inode *i, struct file *f)
{
    pr_info("skeleton : open operation... major:%d, minor:%d\n",
            imajor(i), iminor(i));
    if ((f->f_mode & (FMODE_READ | FMODE_WRITE)) != 0)
    {
        pr_info("skeleton : opened for reading & writing...\n");
    }
    else if ((f->f_mode & FMODE_READ) != 0)
    {
        pr_info("skeleton : opened for reading...\n");
    }
    else if ((f->f_mode & FMODE_WRITE) != 0)
    {
        pr_info("skeleton : opened for writing...\n");
    }
    if(iminor(i) >= instances) return -EFAULT;
    
    f->private_data = buffers[iminor(i)];
    return 0;
}

static int skeleton_release(struct inode *i, struct file *f)
{
    pr_info("skeleton: release operation...\n");
    return 0;
}

static ssize_t skeleton_read(struct file *f,
                             char __user *buf,
                             size_t count,
                             loff_t *off)
{
    // compute remaining bytes to copy, update count and pointers
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    char *ptr = (char*)f->private_data + *off;
    if (count > remaining)
        count = remaining;
    *off += count;
    // copy required number of bytes
    if (copy_to_user(buf, ptr, count) != 0)
        count = -EFAULT;
    pr_info("skeleton: read operation... read=%ld\n", count);
    return count;
}

static ssize_t skeleton_write(struct file *f,
                              const char __user *buf,
                              size_t count,
                              loff_t *off)
{
    // compute remaining space in buffer and update pointers
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    // check if still remaining space to store additional bytes
    if (count >= remaining)
        count = -EIO;
    // store additional bytes into internal buffer
    if (count > 0)
    {
        char *ptr = (char*)f->private_data + *off;
        *off += count;
        ptr[count] = 0; // make sure string is null terminated
        if (copy_from_user(ptr, buf, count))
            count = -EFAULT;
    }
    pr_info("skeleton: write operation... written=%ld\n", count);
    return count;
}

static struct file_operations skeleton_fops = {
    .owner = THIS_MODULE,
    .open = skeleton_open,
    .read = skeleton_read,
    .write = skeleton_write,
    .release = skeleton_release,
};

static int __init skeleton_init(void)
{
    int i = 0;
    int status = alloc_chrdev_region(&skeleton_dev, 0, instances, "mymodule");
    if (instances <= 0) return -EFAULT;
    if (status == 0)
    {
        cdev_init(&skeleton_cdev, &skeleton_fops);
        skeleton_cdev.owner = THIS_MODULE;
        status = cdev_add(&skeleton_cdev, skeleton_dev, instances);
    }
    if (status == 0) {
        buffers = kzalloc(sizeof(char*) * instances, GFP_KERNEL);
        for (i = 0; i < instances; i++) buffers[i] = kzalloc(BUFFER_SZ * sizeof(char), GFP_KERNEL);
    }

    pr_info("Linux module skeleton loaded\n");
    pr_info("Use %d instance\n", instances);
    return 0;
}

static void __exit skeleton_exit(void)
{
    int i = 0;
    cdev_del(&skeleton_cdev);
    unregister_chrdev_region(skeleton_dev, instances);

    for (i = 0; i < instances; i++) kfree(buffers[i]);
    kfree(buffers);
    pr_info("Linux module skeleton unloaded\n");
}
module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_AUTHOR ("Raemy Mathis , Macherel Rémy");
MODULE_DESCRIPTION ("Module skeleton");
MODULE_LICENSE ("GPL");