#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <asm/arch/irq.h>

extern __inline__ int irq_canonicalize(int irq)
{  
  return irq; 
}

#endif  /* _ASM_IRQ_H */


