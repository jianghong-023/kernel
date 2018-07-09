/*************************************

NAME:tq2440_buttons.c
COPYRIGHT:www.embedsky.net

*************************************/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/fb.h>
#include <asm/hardware.h>

#include <linux/mm.h>
#include <asm/arch/regs-timer.h>
#include <asm/arch/map.h>
#include <asm/arch/regs-irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#define DEVICE_NAME	"TQ2440_backlight"
#define LED_MAJOR	234
#define TQ2440_LCDCON5	(0x4D000010)


static int tq2440_backlight_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	unsigned long temp;
	switch(cmd)
	{
		case 0:
			s3c2410_gpio_setpin(S3C2410_GPG4, 0);
			temp = __raw_readl(S3C2410_LCDCON5);
			temp = temp & (~(1 << 3) | (0 << 3));
			__raw_writel(temp, S3C2410_LCDCON5);
			printk(DEVICE_NAME " Turn Off!\n");
			return 0;
		case 1:
			s3c2410_gpio_setpin(S3C2410_GPG4, 1);
			temp = __raw_readl(S3C2410_LCDCON5);
			temp = temp | (1 << 3);
			__raw_writel(temp, S3C2410_LCDCON5);
			printk(DEVICE_NAME " Turn On!\n");
			return 0;
		default:
			return -EINVAL;
	}
}

static struct file_operations tq2440_backlight_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	tq2440_backlight_ioctl,
};

static int __init tq2440_backlight_init(void)
{
	int ret;

	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &tq2440_backlight_fops);
	if (ret < 0) {
	  printk(DEVICE_NAME " can't register major number\n");
	  return ret;
	}

	devfs_mk_cdev(MKDEV(LED_MAJOR, 0), S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP, DEVICE_NAME);
	
	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit tq2440_backlight_exit(void)
{
	devfs_remove(DEVICE_NAME);
	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
}

module_init(tq2440_backlight_init);
module_exit(tq2440_backlight_exit);
