#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "mouse.h"
#include "mouse_macros.h"

static int hook=3;

unsigned long stat_reg();
unsigned long get_code_mouse();

mouse_data mouse_handler(unsigned long packet[], mouse_data dados)
{
	unsigned char LB, RB, MB, Xsign, Ysign, Xov, Yov;
	unsigned long deltaX, deltaY;

	//retira os dados obtidos
	LB = packet[0] & BIT(0);
	RB = (packet[0] & GET_BIT1) >> 1;
	MB = (packet[0] & GET_BIT2) >> 2;
	Xsign = (packet[0] & GET_BIT4) >> 4;
	Ysign = (packet[0] & GET_BIT5) >> 5;
	Xov = (packet[0] & GET_BIT6) >> 6;
	Yov = (packet[0] & GET_BIT7) >> 7;

	if (Xsign == 1)
		deltaX = (packet[1] ^ 0xff) + 1;
	else
		deltaX = packet[1];
	if (Ysign == 1)
		deltaY = (packet[2] ^ 0xff) + 1;
	else
		deltaY = packet[2];

	if (RB == 1) {//trata dos cliques do botão direito do rato,só existe um clique e depois o batão só esta para baixo
		if (dados.rightBotton == RIGHTCLICK) {
			dados.rightBotton = RIGHTDOWN;
		}
		if (dados.rightBotton == RIGHTUP) {
			dados.rightBotton = RIGHTCLICK;
		}
	} else if (RB == 0) {
		dados.rightBotton = RIGHTUP;
	}

	if (LB == 1) {//trata dos cliques do botão esquerdo do rato
		if (dados.leftBotton == LEFTCLICK) {
			dados.leftBotton = LEFTDOWN;
		}
		if (dados.leftBotton == LEFTUP) {
			dados.leftBotton = LEFTCLICK;
		}
	} else if (LB == 0) {
		dados.leftBotton = LEFTUP;
	}

	//trata do movimento do rato no ecra e verifica as suas posições para não sair fora do ecrã
	if (Xsign == 1) {
		if (deltaX >= dados.posX) {
			dados.posX = 0;
		} else {
			dados.posX -= deltaX;
		}
	} else if (Xsign == 0) {
		dados.posX += deltaX;
		if (dados.posX > 1024) {
			dados.posX = 1024;
		}
	}

	if (Ysign == 1) {
		dados.posY += deltaY;
		if (dados.posY > 768) {
			dados.posY = 768;
		}
	} else if (Ysign == 0) {
		if (deltaY >= dados.posY) {
			dados.posY = 0;
		} else {
			dados.posY -= deltaY;
		}

	}

	return dados;

}

unsigned long get_statreg()
{
	return stat_reg();
}

unsigned long mouse_get_code()
{
	return get_code_mouse();
}

int mouse_write_cmd(unsigned long cmd)
{
	int quantidade=0;
	while (quantidade <=10) {
		if ((get_statreg() & KBD_IBF) == 0) {
			unsigned long receber;
			sys_outb(KBC_CMD_REG, MOUSE_WRITE_BYTE);
			sys_outb(KBC_CMD_REG_ARGS, cmd);
			tickdelay(micros_to_ticks(DELAY_US));
			if (sys_inb(KBD_OUT_BUF, &receber) != 0)
				return 1;
			if (receber == ACK)//foi aceite e a função acaba
				return 0;
			else if (receber == ERROR) {//ocorreu um erro
				printf("Ocorreu um erro.\n");
				return 1;
			}
			else//é preciso reenviar o codigo, e é repetido num maximo de 10 vezes
			{
				quantidade++;
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}
	}
	return 1;
}

int mouse_subscribe(void) {
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) != 0)
		return -1;
	if (sys_irqenable(&hook) != 0)
		return -1;

	mouse_write_cmd(SET_STREAM_MODE);
	mouse_write_cmd(ENABLE_DATA);

	return 3;
}

int mouse_unsubscribe() {
	mouse_write_cmd(DISABLE_DATA);
	if (sys_irqdisable(&hook) != 0)
		return -1;
	if (sys_irqrmpolicy(&hook) != 0)
		return -1;
	return 0;
}


