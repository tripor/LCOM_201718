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
#include "programInicio.h"
#include "ser.h"

int main(int argc, char **argv)
{
	if (argc == 2) {
		char* endereco = add_strings(argv[1], "projeto_principal/src/images/mouse.bmp");
		Bitmap *testar=NULL;
		testar = loadBitmap(endereco);
		if(testar==NULL)
		{
			printf("Path errado. Exemplo:\"/home/lcom/\"\n");
			return 0;
		}
		guardar_path(argv[1]);
	} else if (argc == 1) {
		char mandar[100] = "/home/lcom/";
		guardar_path(mandar);
	} else {
		printf("Quantidade de argumentos invalida");
	}
	sef_startup();
	sys_enable_iop(SELF);

	int ipc_status,r,frame=0;
	message msg;

	//subscreve aos interrupts do timer
	int irq_set_timer,hook_id_timer;
	hook_id_timer=timer_subscribe();
	if (hook_id_timer == -1) {
		printf("Ocorreu um erro no kbd_subscribe");
	} else {
		irq_set_timer = BIT(hook_id_timer);
	}

	//subscreve aos interrupts do keyboard
	unsigned long scancode;
	int irq_set_kbd,hook_id_kbd;
	hook_id_kbd=kbd_subscribe();
	if (hook_id_kbd == -1) {
		printf("Ocorreu um erro no kbd_subscribe");
	} else {
		irq_set_kbd = BIT(hook_id_kbd);
	}

	//subscreve aos interrupts do mouse
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

	//subscreve aos interrupts do serial port
	int irq_set_ser,hook_id_ser;
	hook_id_ser=ser_subscribe();
	if (hook_id_ser == -1) {
		printf("Ocorreu um erro no ser_subscribe");
	} else {
		irq_set_ser = BIT(hook_id_ser);
	}

	menu estado=INICIAL;

	unsigned char *video_mem;
	video_mem=vg_init();
	unsigned char buffer[H_RES*V_RES];
	unsigned char inicial[H_RES*V_RES];

	imagem *mouse = malloc(sizeof(imagem));
	Bitmap *triste;
	triste = load_imagem("mouse");
	mouse->mapa = triste;
	mouse->before=NULL;
	mouse->next=NULL;

	menu_multi estado_multi=WAITING;

	while(estado!=SAIR)
	{
		if(estado==INICIAL)//caso seja o menu inicial
		{
			comeca_inicio();

			init_inicio(buffer,inicial);

			estado_multi=WAITING;
			estado_multi = ser_ih_inicial();
			if(estado_multi==WAITING)send_presence();

			int ajuda=1;

			while (estado == INICIAL) {
				if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
					printf("driver_receive falho com: %d", r);
				}
				if (is_ipc_notify(ipc_status)) {
					switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE:
						if (msg.NOTIFY_ARG & irq_set_timer)//interrupt timer
						{
							if(frame==1)
							{
								desenhar_rato(buffer,dados,mouse);
								update_ecra(video_mem,buffer,inicial);
								frame=0;
							}
							else
							{
								frame++;
							}
						}
						if (msg.NOTIFY_ARG & irq_set_kbd)//interrupt keyboard
						{
							kbd_handler(&scancode);
							if (scancode == ESC_BREAK_CODE) {
								estado=SAIR;
							} else {
							}
						}
						if (msg.NOTIFY_ARG & irq_set_mouse)//interrupt mouse
						{
							packet[mouse_quantidade] = mouse_get_code();
							if (packet[0] & BIT(3)) {
								mouse_quantidade++;
								if (mouse_quantidade == 3) {
									dados=mouse_handler(packet,dados);
									mouse_controlador_inicial(&dados,mouse,&estado,inicial,&ajuda);
									if(estado==JOGOMULTI && estado_multi==WAITING) estado=INICIAL;
									mouse_quantidade = 0;
								}
							}
						}
						if (msg.NOTIFY_ARG & irq_set_ser) { // interrupt do serial port
							estado_multi = ser_ih_inicial();
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


		}
		else if(estado==JOGO)//caso seja o jogo singleplayer
		{

			//inicialização
			balao_type **vazio=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));
			balao_type **colisao=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));

			imagem **board=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));
			imagem **board_empty=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));

			init(buffer,inicial,vazio,board_empty);
			memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
			memcpy(board,board_empty,sizeof(imagem*)*(H_RES*V_RES));

			comeca(board_empty);

			desenhar_objeto(buffer,colisao);

			while (estado==JOGO) {
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
								estado=update(buffer,estado);
								desenhar_objeto(buffer,colisao);
								avancar();
								avancar_projeteis(buffer,colisao,vazio);
								desenhar_rato(buffer,dados,mouse);
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
								estado=SAIR;
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

		}
		else if(estado==JOGOMULTI)//caso seja o jogo multiplayer
		{

			//inicialização
			balao_type **vazio=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));
			balao_type **colisao=(balao_type **)malloc(sizeof(balao_type*)*(H_RES*V_RES));

			imagem **board=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));
			imagem **board_empty=(imagem**)malloc(sizeof(imagem*)*(H_RES*V_RES));

			init(buffer,inicial,vazio,board_empty);
			memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
			memcpy(board,board_empty,sizeof(imagem*)*(H_RES*V_RES));

			comeca(board_empty);

			desenhar_objeto(buffer,colisao);

			menu_multi espera=LOADING;
			send_bloon('Q');

			//o programa só funciona quando os 2 jogadores estiverem prontos
			while (estado==JOGOMULTI) {
				if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
					printf("driver_receive falho com: %d", r);
				}
				if (is_ipc_notify(ipc_status)) {
					switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE:
						if ((msg.NOTIFY_ARG & irq_set_timer)&& (estado_multi == PRONTO1|| estado_multi == PRONTO2)&&(espera == LOADED)) {
							if (frame == 1) {
								estado_multi = ser_ih();
								estado = update(buffer, estado);
								desenhar_objeto(buffer, colisao);
								avancar();
								avancar_projeteis(buffer, colisao, vazio);
								desenhar_rato(buffer, dados, mouse);
								update_ecra(video_mem, buffer, inicial);
								memcpy(colisao, vazio,
										sizeof(balao_type*) * (H_RES * V_RES));
								frame = 0;
							} else {
								frame++;
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_timer)&& (espera == LOADING)) {
							if (frame == 1) {
								update_ecra(video_mem, buffer, inicial);
								frame = 0;
							} else {
								frame++;
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_kbd)&& (estado_multi == PRONTO1)&&(espera == LOADED)) {
							kbd_handler(&scancode);
							if (scancode == ESC_BREAK_CODE) {
								estado = SAIR;
							} else {
								keypressed_jogador1(scancode, &dados, mouse);
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_kbd)&& (estado_multi == PRONTO2)&&(espera == LOADED)) {
							kbd_handler(&scancode);
							if (scancode == ESC_BREAK_CODE) {
								estado = SAIR;
							} else {
								keypressed_jogador2(scancode);
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_kbd)&& (espera == LOADING)) {
							kbd_handler(&scancode);
							if (scancode == ESC_BREAK_CODE) {
								estado = SAIR;
							} else {
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_mouse)&& (estado_multi == PRONTO1)&&(espera == LOADED)) {
							packet[mouse_quantidade] = mouse_get_code();
							if (packet[0] & BIT(3)) {
								mouse_quantidade++;
								if (mouse_quantidade == 3) {
									dados = mouse_handler(packet, dados);
									mouse_controlador(&dados, mouse, board,
											board_empty);
									mouse_quantidade = 0;
								}
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_mouse)&& (estado_multi == PRONTO2)&&(espera == LOADED)) {
							packet[mouse_quantidade] = mouse_get_code();
							if (packet[0] & BIT(3)) {
								mouse_quantidade++;
								if (mouse_quantidade == 3) {
									mouse_handler(packet, dados);
									dados.posX = H_RES_REAL;
									dados.posY = V_RES;
									mouse_quantidade = 0;
								}
							}
						}
						if ((msg.NOTIFY_ARG & irq_set_mouse)&& (espera == LOADING)) {
							packet[mouse_quantidade] = mouse_get_code();
							if (packet[0] & BIT(3)) {
								mouse_quantidade++;
								if (mouse_quantidade == 3) {
									mouse_handler(packet, dados);
									dados.posX = H_RES_REAL;
									dados.posY = V_RES;
									mouse_quantidade = 0;
								}
							}
						}

						if (msg.NOTIFY_ARG & irq_set_ser) {
							if(espera==LOADING)
							{
								espera = ser_ih();
								if(espera!=LOADED)espera=LOADING;
								else {
									send_bloon('Q');
								}
							}
							else estado_multi = ser_ih();
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
		}
		//

	}

	vg_exit();

	if (ser_unsubscribe() == -1) {
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
	}
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
