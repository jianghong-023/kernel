/*************************************

NAME:tq2440_hello.c
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

MODULE_LICENSE("GPL");                

static int __init tq2440_hello_init(void)
{

    printk("<1>\n     Hello,EmbedSky!\n");
    printk("<1>\nThis is first driver program.\n\n");

    return 0;
}

static void __exit tq2440_hello_exit(void)
{
    printk("<1>\n     Exit!\n");
    printk("<1>\nGoodbye EmbedSky!\n\n");
}                                    

module_init(tq2440_hello_init);
module_exit(tq2440_hello_exit);
