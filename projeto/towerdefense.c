#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "mouse.h"
#include "timer.h"
#include "keyboard.h"
#include "macros.h"
#include "data_storage.h"
#include "vb.h"
#include "program.h"
#include "lowProgram.h"
#include "bitmap.h"


int main(int argc, char **argv)
{
	sef_startup();

	int ipc_status,r,frame=0;
	message msg;

	int irq_set_timer,hook_id_timer;
	hook_id_timer=timer_subscribe();
	if (hook_id_timer == -1) {
		printf("Ocorreu um erro no kbd_subscribe");
	} else {
		irq_set_timer = BIT(hook_id_timer);
	}

	unsigned long scancode;
	int i=0;
	int irq_set_kbd,hook_id_kbd;
	hook_id_kbd=kbd_subscribe();
	if (hook_id_kbd == -1) {
		printf("Ocorreu um erro no kbd_subscribe");
	} else {
		irq_set_kbd = BIT(hook_id_kbd);
	}

	mouse_data dados;
	dados.posX=H_RES_REAL/2;
	dados.posY=V_RES/2;
	dados.rightBotton=RIGHTUP;
	dados.leftBotton=LEFTUP;
	dados.tipo=NADA;
	dados.select=NAO;
	dados.selected_monkey=NULL;
	unsigned long packet[3];
	int mouse_quantidade=0;
	int irq_set_mouse, hook_id_mouse;
	hook_id_mouse = mouse_subscribe();
	if (hook_id_mouse == -1) {
		printf("Ocorreu um erro no kbd_subscribe");
	} else {
		irq_set_mouse = BIT(hook_id_mouse);
	}

	unsigned char *video_mem;
	video_mem=vg_init();
	unsigned char buffer[H_RES*V_RES];
	unsigned char inicial[H_RES*V_RES];
	balao_type **vazio=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));
	balao_type **colisao=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));

	imagem **board=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));
	imagem **board_empty=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));

	init(buffer,inicial,vazio,board_empty);
	memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
	memcpy(board,board_empty,sizeof(imagem*)*(H_RES*V_RES));

	imagem *mouse = malloc(sizeof(imagem));
	Bitmap *triste;
	triste = load_imagem("mouse");
	mouse->mapa = triste;
	mouse->before=NULL;
	mouse->next=NULL;


	comeca(board_empty);

	desenhar_objeto(buffer,colisao);

	while (i == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive falho com: %d", r);
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer)
				{
					if(frame==1)
					{
						update();
						desenhar_objeto(buffer,colisao);
						desenhar_rato(buffer,dados,mouse);
						avancar();
						avancar_projeteis(buffer,colisao,vazio);
						update_ecra(video_mem,buffer,inicial);
						memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
						frame=0;
					}
					else
					{
						frame++;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_kbd)
				{
					kbd_handler(&scancode);
					if (scancode == ESC_BREAK_CODE) {
						i = 1;
						//deleteBitmap(mouse->mapa);
						//free(mouse);
					} else {
						keypressed(scancode, &dados,mouse);
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					packet[mouse_quantidade] = mouse_get_code();
					if (packet[0] & BIT(3)) {
						mouse_quantidade++;
						if (mouse_quantidade == 3) {
							dados=mouse_handler(packet,dados);
							mouse_controlador(&dados,mouse,board,board_empty);
							mouse_quantidade = 0;
						}
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	while (get_statreg() & KBD_OBF) {
		mouse_get_code();
	}

	vg_exit();

	if (mouse_unsubscribe() == -1) {
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
	}
	if (kbd_unsubscribe() == -1) {
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
	}
	if (timer_unsubscribe() == -1) {
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
	}

	return 0;
}
