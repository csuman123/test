#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
//#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/gpio-cfg.h>

#define DEVICE_NAME "2410_leds_For2.6"

/*
cmd:bitMap:高电平熄灭，低电平点亮
arg:0-3:LED0-LED3
*/
static long sbc2410_leds_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		unsigned tmp;
	case 0:
	case 1:
		if (arg > 4) {
			return -EINVAL;
		}
		tmp = readl(S3C2410_GPBDAT);
		tmp &= ~(1 << (7 + arg));
		tmp |= ( (!cmd) << (7 + arg) );
		writel(tmp, S3C2410_GPBDAT);
		//printk (DEVICE_NAME": %d %d\n", arg, cmd);
		return 0;
	default:
		return -EINVAL;
	}
}

static struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= sbc2410_leds_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	{
		unsigned tmp;
		tmp = readl(S3C2410_GPBCON);
		tmp = (tmp & ~(0xffU<<14))|(0x55U<<14);/*初始化为输出*/
		writel(tmp, S3C2410_GPBCON);
		
		tmp = readl(S3C2410_GPBDAT);/*初始化为高电平熄灭*/
		tmp |= (0xF << 7);/*4LED：GPIOB7-10*/
		writel(tmp, S3C2410_GPBDAT);
	}

	ret = misc_register(&misc);

	printk (DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex tanghu");
