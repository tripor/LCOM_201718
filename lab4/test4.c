#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "macros.h"
#include "test4.h"
#include "help.h"

static unsigned long counter=0;
typedef enum {INIT,DRAW,COMP} stat_t;
typedef enum {RDOW,RUP,MOVE} ev_type_t;

void get_event(ev_type_t *tp, unsigned long packet[]) {
	if ((*tp) == RUP && (packet[0] & GET_BIT1)) {
		(*tp) = RDOW;
	} else if (!(packet[0] & GET_BIT1)) {
		(*tp) = RUP;
	} else if ((*tp) == RDOW) {
		if (((!(packet[0] & GET_BIT4)) && (!(packet[0] & GET_BIT5))) || ((packet[0] & GET_BIT4) && (packet[0] & GET_BIT5))) {
			if (packet[1] > 0 || packet[2] > 0) {
				(*tp) = MOVE;
			}
		}
	} else if ((*tp) == MOVE) {
		if (packet[1] == 0 && packet[2] == 0) {
			(*tp) = RDOW;
		}
		if(((packet[0] & GET_BIT4) || (packet[0] & GET_BIT5)) && (!((packet[0] & GET_BIT4) && (packet[0] & GET_BIT5))))
		{
			(*tp) = RDOW;
		}
	}
}

void stat_modifier(stat_t *st, ev_type_t *tp, unsigned long packet[], long *x,
		long *y, long lenght,short *i) {
	get_event(tp, packet);
	switch (*st) {
	case INIT:
		if ((*tp) == RDOW)
			(*st) = DRAW;
		break;
	case DRAW:
		if ((*tp) == RUP)
			(*st) = INIT;
		else if ((*tp) == MOVE) {
			if ((packet[0] & GET_BIT4) && (packet[0] & GET_BIT5)) {
				if ((*i) == 1) {
					(*x) = 0;
					(*y) = 0;
				} else {
					(*x) += (packet[1] ^ 0xff) + 1;
					(*y) += (packet[2] ^ 0xff) + 1;
				}
			} else {
				if ((*i) == 2) {
					(*x) = 0;
					(*y) = 0;
				} else {
					(*x) += packet[1];
					(*y) += packet[2];
				}
			}
			long meu = *x;
			if (meu >= lenght && (*y) > 0 && (*i) == 1)
				(*st) = COMP;
			else if (meu >= (lenght * -1) && (*y) > 0 && (*i) == 2)
				(*st) = COMP;
		} else if ((*tp) == RDOW) {
			(*x) = 0;
			(*y) = 0;
		}
		break;
	case COMP:
		break;
	}
}

void display_mouse(unsigned long packet[])
{
	printf("B1=0x%x ",packet[0]);
	printf("B2=0x%x ",packet[1]);
	printf("B3=0x%x ",packet[2]);
	printf("LB=%d ",packet[0] & GET_BIT0);
	printf("MB=%d ",(packet[0] & GET_BIT2)>>2);
	printf("RB=%d ",(packet[0] & GET_BIT1)>>1);
	printf("XOV=%d ",(packet[0] & GET_BIT6)>>6);
	printf("YOV=%d ",(packet[0] & GET_BIT7)>>6);
	if (packet[0] & GET_BIT4)
		printf("X=-%d ", (packet[1] ^ 0xff)+1);
	else
		printf("X=%d ", packet[1]);
	if (packet[0] & GET_BIT5)
		printf("Y=-%d ", (packet[2]^0xff)+1);
	else
		printf("Y=%d ", packet[2]);
	printf("\n");
}
void timer_handler()
{
	counter++;
}
int set_remote_mode()
{
	if (mouse_write_cmd(ENABLE_DATA) == 1)//Porque , depois de testes o programa só lê os packets bem se antes de fazer disable for feito enable
			return 1;
	if (mouse_write_cmd(DISABLE_DATA) == 1)
		return 1;
	if (mouse_write_cmd(SET_REMOTE_MODE) == 1)
		return 1;
	return 0;
}
int set_stream_mode()
{
	if (mouse_write_cmd(SET_STREAM_MODE) == 1)
		return 1;
	return 0;
}
int disable_mouse()
{
	write_cmd_byte(ENA_IN_KBD |DISABLE_KBD);
	return 0;
}
int enable_mouse()
{
	write_cmd_byte(ENA_IN_MOUSE | ENA_IN_KBD );
	return 0;
}
int mouse_test_packet(unsigned short cnt){
	int ipc_status, r, irq_set_mouse, hook_id;
	message msg;
	int i=0;
	unsigned counter=1;
	unsigned long packet[3];
	hook_id = mouse_subscribe(); //Faz subscribe aos interrupts do teclado.
	if (hook_id != 1)
		irq_set_mouse = BIT(hook_id); //por a 1 a posicao definidada por hook
	else //Caso ocorra um erro.
	{
		printf("Ocorreu um erro no mouse_subscribe");
		return 1;
	}
	mouse_write_cmd(SET_STREAM_MODE);
	mouse_write_cmd(ENABLE_DATA);
	while (counter<=cnt) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive falho com: %d", r);
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					packet[i]=mouse_get_code();
					if (packet[0] & BIT3) {
						i++;
						if (i == 3) {
							display_mouse(packet);
							counter++;
							i = 0;
						}
					}
				}
				break;
			default: 	//não faz nada
				break;
			}
		} else {
			//não faz nada
		}
	}
	mouse_write_cmd(DISABLE_DATA);
	if (mouse_unsubscribe() != 0) //Fazer unsub
	{
		printf("Ocorreu um erro no mouse_unsubscribe.\n");
		return 1;
	}
	return 1;
}

int mouse_test_async(unsigned short idle_time){
	int ipc_status, r, irq_set_mouse, irq_set_timer, hook_id,timer_hook_id;
	message msg;
	int i = 0;
	unsigned long packet[3];


	timer_hook_id=timer_subscribe_int();
	hook_id = mouse_subscribe(); //Faz subscribe aos interrupts do teclado.
	if (hook_id != 1 && timer_hook_id != 1) {
		irq_set_mouse = BIT(hook_id); //por a 1 a posicao definidada por hook
		irq_set_timer = BIT(timer_hook_id);
	} else //Caso ocorra um erro.
	{
		printf("Ocorreu um erro no subscribe");
		return 1;
	}

	mouse_write_cmd(SET_STREAM_MODE);
	mouse_write_cmd(ENABLE_DATA);

	while (counter<=idle_time*60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive falho com: %d", r);
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					counter=0;
					packet[i] = mouse_get_code();
					if (packet[0] & BIT3) {
						i++;
						if (i == 3) {
							display_mouse(packet);
							i = 0;
						}
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_handler();
				}
				break;
			default: 	//não faz nada
				break;
			}
		} else {
			//não faz nada
		}
	}
	printf("Passaram os %d segundos\n",idle_time);
	mouse_write_cmd(DISABLE_DATA);
	if (timer_unsubscribe_int() != 0) //Fazer unsub
			{
		printf("Ocorreu um erro no timer_unsubscribe.\n");
		return 1;
	}
	if (mouse_unsubscribe() != 0) //Fazer unsub
			{
		printf("Ocorreu um erro no mouse_unsubscribe.\n");
		return 1;
	}
	return 1;
}

int mouse_test_remote(unsigned long period, unsigned short cnt){
	int i=0;
	unsigned long packet[3],code;
	disable_mouse();
	if (set_remote_mode() == 1) {
		printf("Ocorreu um erro a fazer set up ao remote mode\n");
		return 1;
	}
	while(i<=cnt)
	{
		mouse_write_cmd(READ_DATA);
		if(get_statreg() & (KBD_OBF | AUX))
		{
			sys_inb(KBD_OUT_BUF,&packet[0]);
			if (packet[0] & BIT3) {
				sys_inb(KBD_OUT_BUF, &packet[1]);
				sys_inb(KBD_OUT_BUF, &packet[2]);
				display_mouse(packet);
				i++;
			}
			while(get_statreg() & KBD_OBF)
			{
				sys_inb(KBD_OUT_BUF,&code);
			}

		}
		tickdelay(micros_to_ticks(period*1000));//67 -> 1 segundo
	}
	printf("A quantidade de imprecoes acabou e o programa vai terminar.\n");
	if (set_stream_mode() == 1) {
		printf("Ocorreu um erro a fazer set up ao stream mode\n");
		return 1;
	}
	enable_mouse();
	return 1;
}

int mouse_test_gesture(long length){//alterei de short para long para fazer distancias maiores
	int ipc_status, r, irq_set_mouse, hook_id;
	message msg;
	int i = 0;
	unsigned long packet[3];
	long x=0,y=0;
	short sentido;
	if(length > 0) sentido =1;
	else sentido =2;
	stat_t st=INIT;
	ev_type_t tp=RUP;

	hook_id = mouse_subscribe(); //Faz subscribe aos interrupts do teclado.
	if (hook_id != 1)
		irq_set_mouse = BIT(hook_id); //por a 1 a posicao definidada por hook
	else //Caso ocorra um erro.
	{
		printf("Ocorreu um erro no mouse_subscribe");
		return 1;
	}
	mouse_write_cmd(SET_STREAM_MODE);
	mouse_write_cmd(ENABLE_DATA);
	while (st!=COMP) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive falho com: %d", r);
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					packet[i] = mouse_get_code();
					if (packet[0] & BIT3) {
						i++;
						if (i == 3) {
							display_mouse(packet);
							stat_modifier(&st,&tp,packet,&x,&y,length,&sentido);
							i = 0;
						}
					}
				}
				break;
			default: 	//não faz nada
				break;
			}
		} else {
			//não faz nada
		}
	}
	printf("O programa terminou\n");
	mouse_write_cmd(DISABLE_DATA);
	if (mouse_unsubscribe() != 0) //Fazer unsub
			{
		printf("Ocorreu um erro no mouse_unsubscribe.\n");
		return 1;
	}
	return 0;
}
