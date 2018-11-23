#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"

static int hook=1;

int timer_subscribe(void) {
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook)!=0)return -1;
	if(sys_irqenable(&hook)!=0)return -1;
	return 1;//valor inicial do hook que corresponde a bit order in interrupt mask;
}

int timer_unsubscribe() {
	if(sys_irqdisable(&hook)!=0)return -1;
	return 0;
}
