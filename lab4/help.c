#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "macros.h"
static int hook=0,timer_hook=2;
unsigned long get_statreg()
{
	unsigned long stat;
	sys_inb(KBD_STATUS_REG,&stat);
	return stat;
}
int mouse_subscribe(void)
{
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) != 0)
		return 1;
	if (sys_irqenable(&hook) != 0)
				return 1;
	return 0;
}
int mouse_unsubscribe()
{
	if(sys_irqdisable(&hook)!=0)return 1;
	if(sys_irqrmpolicy(&hook)!=0)return 1;
	return 0;
}
int mouse_write_cmd(unsigned long cmd)
{
	while (1) {
		if ((get_statreg() & KBD_IBF) == 0) {
			unsigned long receber;
			sys_outb(KBC_CMD_REG, MOUSE_WRITE_BYTE);
			sys_outb(KBC_CMD_REG_ARGS, cmd);
			tickdelay(micros_to_ticks(DELAY_US));
			if (sys_inb(KBD_OUT_BUF, &receber) != 0)
				return 1;
			if (receber == ACK)
				return 0;
			else if (receber == ERROR) {
				printf("Ocorreu um erro.\n");
				return 1;
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}
	}
}
unsigned long mouse_get_code()
{
	unsigned long code;
	sys_inb(KBD_OUT_BUF,&code);
	return code;
}

int timer_subscribe_int(void) {
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_hook)!=0)return 1;
	if(sys_irqenable(&timer_hook)!=0)return 1;
	return 2;//valor inicial do hook que corresponde a bit order in interrupt mask;
}

int timer_unsubscribe_int() {
	if(sys_irqdisable(&timer_hook)!=0)return 1;
	return 0;
}
unsigned long get_cmd_byte()
{
	unsigned long cmd;
	while (1) {
		if ((get_statreg() & KBD_IBF) == 0) {
			sys_outb(KBC_CMD_REG, KBC_READ_CB );
			sys_inb(KBC_CMD_REG_ARGS, &cmd);
			return cmd;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int write_cmd_byte(unsigned long cmd)
{
	while (1) {
		if ((get_statreg() & KBD_IBF) == 0) {
			sys_outb(KBC_CMD_REG, KBC_WRITE_CB);
			sys_outb(KBC_CMD_REG_ARGS, cmd | (get_cmd_byte() & GET_CMD));
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}




