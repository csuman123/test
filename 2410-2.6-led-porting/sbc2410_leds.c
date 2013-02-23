#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <linux/delay.h>

#include <asm/hardware.h>

#define DEVICE_NAME	"leds"
#define LED_MAJOR 231

static unsigned long led_table [] = {
	GPIO_B7,
	GPIO_B8,
	GPIO_B9,
	GPIO_B10,
};

static int sbc2410_leds_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
	case 0:
	case 1:
		if (arg > 4) {
			return -EINVAL;
		}
		write_gpio_bit(led_table[arg], !cmd);
	default:
		return -EINVAL;
	}
}
static struct file_operations sbc2410_leds_fops = {
	owner:	THIS_MODULE,
	ioctl:  sbc2410_leds_ioctl,
};

static devfs_handle_t devfs_handle;
static int __init sbc2410_leds_init(void)
{
	int ret;
	int i;

	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &sbc2410_leds_fops);
	if (ret < 0) {
	  printk(DEVICE_NAME " can't register major number\n");
	  return ret;
	}
	devfs_handle = devfs_register(NULL, DEVICE_NAME, DEVFS_FL_DEFAULT,
				LED_MAJOR, 0, S_IFCHR | S_IRUSR | S_IWUSR, &sbc2410_leds_fops, NULL);
	for (i = 0; i < 8; i++) {
		set_gpio_ctrl (led_table[i] | GPIO_PULLUP_EN | GPIO_MODE_OUT);
		write_gpio_bit(led_table[i], 1);
	}

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit sbc2410_leds_exit(void)
{
	devfs_unregister(devfs_handle);
	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
}

module_init(sbc2410_leds_init);
module_exit(sbc2410_leds_exit);
