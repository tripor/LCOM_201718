#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "program.h"
#include "ser_macros.h"
#include "ser.h"

static int hook=4;
static menu_multi estado=WAITING;
static menu_multi espera=LOADING;
static int jogador=0;

static monkey_type ler_macaco;
static int ler_posX,ler_posY;
static unsigned long ler=0;

menu_multi ser_ih_inicial() {
	unsigned long iir,c;
	sys_inb(SERIAL_PORT + SER_IIR, &iir);
	if (!(iir & SER_INT_PEND)) {
		if(estado==WAITING)
		{
			switch (iir & INT_ID) {
			case SER_RX_INT:
				sys_inb(SERIAL_PORT+SER_RBR, &c);
				if (c == 'R')//caso ele receba um R ele manda um P e diz qual é o jogador que vai ser
				{
					printf("Recebi R");
					estado = PRONTO1;
					jogador=1;
					c='P';
					sys_outb(SERIAL_PORT+SER_THR, c);
				}
				else if(c=='P')//caso ele receba um P ele manda um R e diz qual é o jogador que vai ser
				{
					printf("Recebi P");
					estado = PRONTO2;
					jogador=2;
					c='R';
					sys_outb(SERIAL_PORT+SER_THR, c);

				}
			case SER_TX_INT:
				printf("empty");

			}

		}
		else if(estado==PRONTO1)
		{
			switch (iir & INT_ID) {
			case SER_RX_INT:
				sys_inb(SERIAL_PORT+SER_RBR, &c);
				//reenviar uma mensagem até ele esta ligado ao outro jogador
				if(c=='P')
				{

					printf("Recebi P 1");
					c = 'P';
					sys_outb(SERIAL_PORT + SER_THR, c);
				}
				if (c == 'R') {

					printf("Recebi R 1");
					c = 'P';
					sys_outb(SERIAL_PORT + SER_THR, c);
					printf("connected");

				}

			case SER_TX_INT:
				printf("empty");
			}

		}
		else if(estado==PRONTO2)
		{
			switch (iir & INT_ID) {
			case SER_RX_INT:
				sys_inb(SERIAL_PORT+SER_RBR, &c);

			case SER_TX_INT:
				printf("empty");
			}

		}
	}
	return estado;

}


menu_multi ser_ih() {
	unsigned long iir,c;
	sys_inb(SERIAL_PORT + SER_IIR, &iir);
	printf("interrupt %x",iir);
	if (!(iir & SER_INT_PEND)) {
		if(estado==PRONTO1)
		{
			switch (iir & INT_ID) {
			case SER_RX_INT:
				sys_inb(SERIAL_PORT+SER_RBR, &c);
				printf("roubar");
				//espera para começar o jogo ao mesmo tempo
				if (c == 'Q' && espera == LOADING) {
					espera = LOADED;
					return LOADED;
				}
				//caso receba um balão do outro jogador
				if(c=='R')add_red();
				else if(c=='B')add_blue();
				else if(c=='G')add_green();
				else if(c=='Y')add_yellow();
				else if(c=='P')add_pink();
			case SER_TX_INT:
				printf("empty");
			}

		}
		else if(estado==PRONTO2)
		{
			switch (iir & INT_ID) {
			case SER_RX_INT:
				sys_inb(SERIAL_PORT+SER_RBR, &c);
				sys_outb(SERIAL_PORT+SER_THR, 0xff);
				printf("%c %d",c,c);
				//espera para começar o jogo ao mesmo tempo
				if (c == 'Q' && espera == LOADING) {
					espera=LOADED;
					return LOADED;
				}
				//caso receba um macaco do outro jogador
				if(ler>=1)
				{
					if(ler==1)
					{
						ler_posX=ler_posX | (c<<8);
					}
					else if(ler==2)
					{
						ler_posX=ler_posX | c;
					}
					else if(ler==3)
					{
						ler_posY=ler_posY | (c<<8);
					}
					else if(ler==4)
					{
						ler_posY=ler_posY | c;
					}
					ler++;
					if (ler == 5)
					{
						ler = 0;
						if(ler_macaco==DART)
						{
							add_dart(ler_posX,ler_posY);
						}
						else if(ler_macaco==SUPER)
						{
							add_super(ler_posX,ler_posY);
						}
						ler_posX=0;
						ler_posY=0;
					}
				}

				if(c=='D')
				{
					ler_macaco=DART;
					ler_posX=0;
					ler_posY=0;
					ler=1;
				}
				else if (c=='S')
				{
					ler_posY=0;
					ler_posX=0;
					ler_macaco=SUPER;
					ler=1;
				}
			case SER_TX_INT:
				printf("empty");

			}

		}
	}
	return estado;
}

void esperar_vazio()
{
	unsigned long iir;
	sys_inb(SERIAL_PORT + SER_LSR, &iir);
	do {
		tickdelay(micros_to_ticks(20000));
		sys_inb(SERIAL_PORT + SER_LSR, &iir);
		printf("%x",iir);
	} while (!(iir & BIT(5)));
}

void send_macaco(int posX,int posY,monkey_type tipo)
{
	printf("mandar");
	esperar_vazio();
	unsigned long c;
	if(tipo==DART)
	{
		c='D';
		sys_outb(SERIAL_PORT+SER_THR, c);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posX>>8);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posX);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posY>>8);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posY);
		esperar_vazio();
	}
	else if(tipo==SUPER)
	{
		c='S';
		sys_outb(SERIAL_PORT+SER_THR, c);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posX>>8);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posX);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posY>>8);
		esperar_vazio();
		sys_outb(SERIAL_PORT+SER_THR, posY);
		esperar_vazio();
	}
}

void send_presence()
{
	unsigned long c;
	c='R';
	sys_outb(SERIAL_PORT+SER_THR, c);
}

void send_bloon(unsigned char tipo)
{
	esperar_vazio();
	unsigned long c;
	c=tipo;
	sys_outb(SERIAL_PORT+SER_THR, c);
}

int ser_subscribe(void)
{
	if (sys_irqsetpolicy(COM1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) != 0)
		return -1;
	if (sys_irqenable(&hook) != 0)
		return -1;

	sys_outb(SERIAL_PORT+SER_IER,SER_EN_R | SER_EN_T );
	sys_outb(SERIAL_PORT+SER_LCR,LCR_8BIT | LCR_1_STOP | LCR_ODD |LCR_DLAB);
	unsigned long parte1,parte2,total;
	total=115200/115200;
	parte1=total & MSB;
	parte2=total & LSB;
	parte1=parte1>>8;
	sys_outb(SERIAL_PORT+SER_DLL,parte2);
	sys_outb(SERIAL_PORT+SER_DLM,parte1);
	sys_outb(SERIAL_PORT+SER_LCR,LCR_8BIT | LCR_1_STOP | LCR_ODD |LCR_UN_DLAB);
	unsigned long iir,c;
	sys_inb(SERIAL_PORT + SER_LSR, &iir);//limpa o buffer de receber informações
	do
	{
		sys_inb(SERIAL_PORT+SER_RBR, &c);
		sys_inb(SERIAL_PORT + SER_LSR, &iir);
	}while(iir&BIT(0));

	return 4;
}

int ser_unsubscribe() {
	if (sys_irqdisable(&hook) != 0)
		return -1;
	if (sys_irqrmpolicy(&hook) != 0)
		return -1;
	return 0;
}
