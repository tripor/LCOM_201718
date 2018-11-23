#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "i8042.h"



static int hook=0;

int kbd_subscribe(void)
{
	if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook)!=0)return -1;
	if(sys_irqenable(&hook)!=0)return -1;
	return 0;
}

int kbd_unsubscribe()
{
	if(sys_irqdisable(&hook)!=0)return -1;
	if(sys_irqrmpolicy(&hook)!=0)return -1;
	return 0;
}

int kbd_handler(unsigned long *code2)
{
	unsigned long stat, code;
	if (sys_inb(KBD_STATUS_REG, &stat) != 0)
		return -1;
	if (sys_inb(KBD_OUT_BUF, &code) != 0)
		return -1; //guarda o scancode em code
	if ((stat & (KBD_PAR_ERR | KBD_TO_ERR)) == 0) // ve se há algo de errado com o stat
			{
		if (sys_inb(KBD_STATUS_REG, &stat) != 0)
			return -1;
		*code2 = code;

		return 0;
	} else
		return -1;
}
