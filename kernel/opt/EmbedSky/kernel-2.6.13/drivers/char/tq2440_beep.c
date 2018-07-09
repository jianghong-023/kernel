/*************************************

NAME:tq2440_beep.c
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
#include <linux/mm.h>
#include <asm/irq.h>
#include <asm/arch/regs-timer.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/regs-spi.h>
#include <asm/arch/map.h>
#include <asm/arch/regs-irq.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#define DEVICE_NAME	"TQ2440-Beep"
#define LED_MAJOR	232
#define S3C2440_SPCON1	(0x59000020)
#define S3C2440_SPSTA1	(0x59000024)
#define S3C2440_SPRDAT1	(0x59000034)

static int tq2440_beep_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	unsigned long temp;
	if(cmd <= 0)
	{
		temp = __raw_readl(S3C2410_GPBCON);	//GPBCON
		temp &= ~3;
		temp |= 1;
		__raw_writel(temp, S3C2410_GPBCON);

		temp = __raw_readl(S3C2410_GPBDAT);	//GPBDAT
		temp &= ~1;
		__raw_writel(temp, S3C2410_GPBDAT);
	}
	else
	{
		temp = __raw_readl(S3C2410_GPBCON);	//GPBCON
		temp &= ~3;
		temp |= 2;
		__raw_writel(temp, S3C2410_GPBCON);

		temp = __raw_readl(S3C2410_TCFG0);	//TCFG0
		temp &= ~0xff;
		temp |= 15;
		__raw_writel(temp, S3C2410_TCFG0);

		temp = __raw_readl(S3C2410_TCFG1);	//TCFG1
		temp &= ~0xf;
		temp |= 2;
		__raw_writel(temp, S3C2410_TCFG1);

		temp = (50000000/128)/cmd;
		__raw_writel(temp, S3C2440_TCNTB0);

		temp >>= 1;
		__raw_writel(temp, S3C2440_TCMPB0);

		temp = __raw_readl(S3C2410_TCON);	//TCON
		temp &= ~0x1f;
		temp |= 0xb;
		__raw_writel(temp, S3C2410_TCON);

		temp &= ~2;
		__raw_writel(temp, S3C2410_TCON);	
	}
	return 0;
}

static struct file_operations tq2440_beep_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	tq2440_beep_ioctl,
};

static int __init tq2440_beep_init(void)
{
	int ret;
	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &tq2440_beep_fops);
	if (ret < 0) {
	  printk(DEVICE_NAME " can't register major number\n");
	  return ret;
	}

	devfs_mk_cdev(MKDEV(LED_MAJOR, 0), S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP, DEVICE_NAME);
	printk(DEVICE_NAME " initialized\n");

	return 0;
}

static void __exit tq2440_beep_exit(void)
{
	devfs_remove(DEVICE_NAME);
	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
}

module_init(tq2440_beep_init);
module_exit(tq2440_beep_exit);
