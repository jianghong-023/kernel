/*************************************

NAME:tq2440_gpf.c
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
#include <asm/hardware.h>

#define DEVICE_NAME	"TQ2440_GPF"
#define GPF_MAJOR	235

static unsigned long gpf_table [] = {
	S3C2410_GPF1,			//EINT1
	S3C2410_GPF4,			//EINT4
	S3C2410_GPF5,			//EINT5
	S3C2410_GPG0,			//EINT8
};

static unsigned int gpf_cfg_table [] = {
	S3C2410_GPF1_OUTP,		//EINT1
	S3C2410_GPF4_OUTP,		//EINT4
	S3C2410_GPF5_OUTP,		//EINT5
	S3C2410_GPG0_OUTP,		//EINT8
};

static int tq2440_gpf_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	switch(cmd) {
	case 0:
	case 1:
		if (arg > 4) {
			return -EINVAL;
		}
		s3c2410_gpio_setpin(gpf_table[arg], cmd);
		printk("s3c2410_gpio_%d=%d\n",arg, cmd);
		return 0;
	default:
		return -EINVAL;
	}
}

static struct file_operations tq2440_gpf_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	tq2440_gpf_ioctl,
};

static int __init tq2440_gpf_init(void)
{
	int ret;
	int i;

	ret = register_chrdev(GPF_MAJOR, DEVICE_NAME, &tq2440_gpf_fops);
	if (ret < 0) {
	  printk(DEVICE_NAME " can't register major number\n");
	  return ret;
	}

	devfs_mk_cdev(MKDEV(GPF_MAJOR, 0), S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP, DEVICE_NAME);
	
	for (i = 0; i < 4; i++) {
		s3c2410_gpio_cfgpin(gpf_table[i], gpf_cfg_table[i]);
		s3c2410_gpio_setpin(gpf_table[i], 1);
	}

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit tq2440_gpf_exit(void)
{
	devfs_remove(DEVICE_NAME);
	unregister_chrdev(GPF_MAJOR, DEVICE_NAME);
}

module_init(tq2440_gpf_init);
module_exit(tq2440_gpf_exit);
