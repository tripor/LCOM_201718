#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
static int hook=0;
int counter=0;
int timer_get_conf(unsigned char timer, unsigned char *st) {
	unsigned long uu;
	switch(timer)
	{
	case 0:timer=TIMER_0;
		if(sys_outb(TIMER_CTRL,TIMER_RB_CMD |TIMER_RB_COUNT_ | TIMER_RB_SEL(0))!=0) return 1;
		break;
	case 1:timer=TIMER_1;
		if(sys_outb(TIMER_CTRL,TIMER_RB_CMD |TIMER_RB_COUNT_ | TIMER_RB_SEL(1))!=0) return 1;
		break;
	case 2:timer=TIMER_2;
		if(sys_outb(TIMER_CTRL,TIMER_RB_CMD |TIMER_RB_COUNT_ | TIMER_RB_SEL(2))!=0) return 1;
		break;
	}
	if( sys_inb(timer,&uu) == 0)
	{
		*st=(unsigned char) uu;
		return 0;
	}
	else return 1;
}

int timer_set_frequency(unsigned char timer, unsigned long freq) {
	unsigned char base,config,aux2=0x40;
	unsigned long value2; //uso um variavel auxiliar para organizar melhor os calculos
	int value;
	switch(timer)//LSB followed by MSB
	{
	case 0:base= TIMER_SEL0 | TIMER_LSB_MSB;
		timer=TIMER_0;
		break;
	case 1:base= TIMER_SEL1 | TIMER_LSB_MSB;
		timer=TIMER_1;
		break;
	case 2:base= TIMER_SEL2 | TIMER_LSB_MSB;
		timer=TIMER_2;
		break;
	}
	if(timer_get_conf(timer,& config) == 0)// obter a configurção do timer selecionado
	{
		config= config & 0x0f; //quero que os 4bit mais significativos fiquem a 0
		config= base | config; //para depois adicionar com a base para escrever
		if(sys_outb(TIMER_CTRL,(unsigned long) config)!=0) return 1; //ao fazer sys_outb vai definir o valor inicial
		value = TIMER_FREQ / freq;
		value2= value & GET_8LSB;
		if(sys_outb(timer,(unsigned long) value2)!=0)return 1; // For example, to generate a 1000 Hz tone, the timer must be programmed to operate in mode 2, with an initial value of 1193
		value2=value & GET_8MSB;
		value2= value2 >> 8;
		if(sys_outb(timer,(unsigned long) value2)!=0)return 1;//mando lsb e depois msb
	}
	else printf("Ocurreu um erro a ler as configurações do tempo.");

	return 0;
}

int timer_subscribe_int(void) {
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook)!=0)return -1;
	if(sys_irqenable(&hook)!=0)return -1;
	return 0;//valor inicial do hook que corresponde a bit order in interrupt mask;
}

int timer_unsubscribe_int() {
	if(sys_irqdisable(&hook)!=0)return 1;
	return 0;
}

void timer_int_handler() {
	counter++;
}

int timer_display_conf(unsigned char conf) {
	unsigned char aux;

	aux= conf & TIMER_GET_TYPE;  // fazer um and na conf com 0011 0000
	if(aux == TIMER_LSB)printf("Typer of Access: LSB");
	else if (aux == TIMER_MSB)printf("Typer of Access: MSB");
	else if (aux == TIMER_LSB_MSB)printf("Typer of Access: LSB followed by MSB");
	printf("\n");
	aux= conf & TIMER_GET_OP;  // fazer um and na conf com 0000 1110
	switch(aux)
	{
	case TIMER_OP_0:printf("Operating Mode: 0");
		break;
	case TIMER_OP_1:printf("Operating Mode: 1");
			break;
	case TIMER_OP_2:printf("Operating Mode: 2");
			break;
	case TIMER_OP_2_INTEL:printf("Operating Mode: 2");
		break;
	case TIMER_OP_3:printf("Operating Mode: 3");
			break;
	case TIMER_OP_3_INTEL:printf("Operating Mode: 3");
			break;
	case TIMER_OP_4:printf("Operating Mode: 4");
			break;
	case TIMER_OP_5: printf("Operating Mode: 5");
			break;
	}
	printf("\n");
	aux= conf & TIMER_GET_COUNT;  // fazer um and na conf com 0000 0001
	if(aux == TIMER_BIN)
		printf("Counting mode: Binary (16 bits)\n");
	else if (aux == TIMER_BCD) printf("Counting mode: BCD (4 decades)\n");
	return 0;
}

int timer_test_time_base(unsigned long freq) {
	printf("Mudou a frequencia do timer 0 para: %d\n",freq);
	if(timer_set_frequency(TIMER_SEL0,freq)!=0)printf("Ocurreu um erro a mudar a frequencia\n");
	return 1;
}

int timer_test_int(unsigned long time) {
	int ipc_status,r,irq_set,obter;
	message msg;
	obter=timer_subscribe_int();
	if(obter==0)irq_set=BIT(obter);
	else printf("Ocorreu um erro no subscribe\n");
	counter=0;
	while((time*60)>=counter)
	{
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive failed with: %d", r);
		}

		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					timer_int_handler();
					if((counter%60)==0)
					{
						printf("Interrupt %d\n",counter/60);
					}
				}
				break;
			default: 	//não faz nada
				break;
			}
		}
		else
		{
				//não faz nada
		}

	}
	printf("Acabou o tempo\n");
	if(timer_unsubscribe_int()!=0)printf("Ocorreu um erro no unsubscribe\n");
	return 1;
}

int timer_test_config(unsigned char timer) {
	unsigned char byte;
	if(timer_get_conf(timer,&byte) == 0)
	{
		if(timer_display_conf(byte)!=0)printf("Ocurreu um erro no display\n");
	}
	else printf("Ocurreu um erro a ler as configurações do tempo.\n");
	return 1;
}
