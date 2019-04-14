
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/string.h>

#define DEVICE_NAME "E02"
#define BUF_LEN 128

MODULE_AUTHOR("Andrea Cossio");

static int major;
static char charBuffer[BUF_LEN];
static struct cdev *myCharDevice;
static dev_t dev;
static DEFINE_SEMAPHORE(me);

static int myOpen(struct inode *inode, struct file *file) {

    printk(KERN_INFO "Opened device %s\n", DEVICE_NAME);

    if (down_interruptible(&me)) {
        printk(KERN_INFO "Couldn't hold semaphore.\n");
        return -1;
    }

    return 0;
}

static int myRelease(struct inode *inode, struct file *file) {

    printk(KERN_INFO "Released device %s\n", DEVICE_NAME);
    up(&me);
    return 0;
}

static ssize_t myRead(struct file *file, char *buf, size_t count, loff_t *off) {

    size_t len;
    ssize_t ret = -1;

    // Gather string length
    len = strlen(charBuffer);

    printk(KERN_INFO "Received request of reading %ld bytes. The current len of the buffer is %ld bytes.\n", count, len);

    // If someone wanted to read more than len
    if (*off > 0) {
        printk(KERN_INFO "Offset was set. Returning 0.\n");
        // Restore offset pointer
        *off = 0;
        return 0;
    }

    // If someone wants to read more than len
    if (count > len) {

        // Copy len bytes
        ret = copy_to_user(buf, charBuffer, len);

        printk(KERN_INFO "Copying %ld bytes... Returning %ld bytes copied. The offset has been set.\n", len, len - ret);

        // Move forward the pointer
        *off += len - ret;

        // Return the number of bytes that were actually read (successfull copy -> ret = 0)
        return len - ret;

    } else {

        // Copy count bytes
        ret = copy_to_user(buf, charBuffer, count);

        printk(KERN_INFO "Copying %ld bytes... Returning %ld bytes copied.\n", count, count - ret);

        // Return the number of bytes that were actually read (successfull copy -> ret = 0)
        return count - ret;
    }
}

static ssize_t myWrite(struct file *file, const char *buf, size_t count, loff_t *off) {

    ssize_t ret = -1;

    printk(KERN_INFO "Received request of writing %ld bytes.\n", count);

    // Check overwriting
    if (count > BUF_LEN) {
        printk(KERN_INFO "Attempted to overwrite the buffer.\n");
        return ret;
    }

    // Clear the buffer
    memset(charBuffer, 0, BUF_LEN);

    // Copy count bytes
    ret = copy_from_user(charBuffer, buf, count);

    printk(KERN_INFO "Copying %ld bytes... Returning %ld bytes copied.\n", count, count - ret);

    // Return the number of bytes that were actually written (successfull copy -> ret = 0)
    return count - ret;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = myRead,
    .write = myWrite,
    .open = myOpen,
    .release = myRelease
};

int init_module(void) {

    int ret = -1;

    printk(KERN_INFO "====    ****    ====");
    printk(KERN_INFO "Initialization of module %s started.\n", DEVICE_NAME);

    // Init device
    if ((myCharDevice = cdev_alloc()) == NULL) {
        printk(KERN_INFO "Failed to allocate the device.\n====    ****    ====\n");
        return ret;
    }
    cdev_init(myCharDevice, &fops);

    // Register device
    if ((ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME)) < 0) {
        printk(KERN_ALERT "Failed to register the device.\n====    ****    ====\n");
        return ret;
    }
    major = MAJOR(dev);
    dev = MKDEV(major, 0);
    
    // Add device
    if ((ret = cdev_add(myCharDevice, dev, 1)) < 0) {
        printk(KERN_INFO "Failed to add the device.\n====    ****    ====\n");
        return ret;
    }

    printk(KERN_INFO "Initialization of the module successfully completed.\n");
    printk(KERN_INFO "Major: %d.\n", major);
    printk(KERN_INFO "To use the device use: mknod /dev/%s c %d 0.\n", DEVICE_NAME, major);
    printk(KERN_INFO "====    ****    ====\n");

    return 0;
}

void cleanup_module(void) {

    printk(KERN_INFO "====    ****    ====");
    printk(KERN_INFO "Cleanup of module %s started.\n", DEVICE_NAME);

    cdev_del(myCharDevice);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "Cleanup of the module successfully completed. The major was: %d\n", major);
    printk(KERN_INFO "====    ****    ====\n");

    return;
}