/* linux/include/asm-arm/arch-s3c2410/tq2440-map.h */


#ifndef __ASM_ARCH_TQMAP_H
#define __ASM_ARCH_TQMAP_H

#define TQ_VA_IOBASE	0xF8000000

#define TQ_VA_CS8900A    TQ_VA_IOBASE	 	/* 0xF8000000 */
#define TQ_PA_CS8900A    (S3C2410_CS4 + 0x1000000)	/* 0x19000000 */
#define TQ_SZ_CS8900A    SZ_1M

#define pSMDK2410_ETH_IO        0x20000300
#define vSMDK2410_ETH_IO        0xd0000000
#define SMDK2410_ETH_IRQ        IRQ_EINT7


#endif /* __ASM_ARCH_TQMAP_H */
