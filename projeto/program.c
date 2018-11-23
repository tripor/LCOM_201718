#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "lowProgram.h"
#include "bitmap.h"
#include "program.h"
#include "macros.h"
#include "data_storage.h"
#include "caminhos.h"

static Bitmap* loadedImages;
static Bitmap* loadedBaloes;
static int* velocidades;
static caminho* inicio;

int aconteceu_colisao(balao_type *balao,projetil *dardo)
{
	if(dardo->pierce>balao->hp)
	{
		delete_balao(balao);
		dardo->pierce-=balao->hp;
		return 1;
	}
	else if(dardo->pierce<balao->hp)
	{
		balao->hp-=dardo->pierce;
		balao->speed=velocidades[balao->hp];
		balao->mapa=&(loadedBaloes[balao->hp]);
		return 2;
	}
	else
	{
		delete_balao(balao);
		return 2;
	}
	//add_monkey(&loadedImages[0],0,0);
	return 2;// 2 para apagar o projetil
}

void carregar_imagens()
{
	loadedImages = malloc(sizeof(Bitmap) * 5);
	loadedImages[0] = *(load_imagem("super"));
	loadedImages[1] = *(load_imagem("ball"));
	loadedImages[2] = *(load_imagem("red"));
	loadedImages[3] = *(load_imagem("blue"));
	loadedImages[4] = *(load_imagem("mouse"));

	loadedBaloes = malloc(sizeof(Bitmap) * 6);
	loadedBaloes[1]=*(load_imagem("red"));
	loadedBaloes[2]=*(load_imagem("blue"));
	loadedBaloes[3]=*(load_imagem("green"));
	loadedBaloes[4]=*(load_imagem("yellow"));
	loadedBaloes[5]=*(load_imagem("pink"));

	velocidades=malloc(sizeof(int)*6);
	velocidades[1]=1;
	velocidades[2]=2;
	velocidades[3]=3;
	velocidades[4]=5;
	velocidades[5]=7;
}

void comeca(imagem **board)
{
	inicio=malloc(sizeof(caminho));
	inicio->next=NULL;
	criar_caminho1(inicio,board);
	carregar_imagens();
}

void update()
{



}

void mouse_controlador(mouse_data *dados,imagem *mouse,imagem **board,imagem **board_empty)
{
	if(dados->rightBotton == RIGHTCLICK)
	{

	}
	if(dados->leftBotton == LEFTCLICK)
	{
		if(dados->tipo == SUPER)
		{
			reescrever_board(board,board_empty);
			if(testar_board( dados->posX,dados->posY,board,&loadedImages[0]))
			{
				add_monkey(&loadedImages[0],&loadedImages[4], dados->posX, dados->posY,300,35,1,1);
			}
			reescrever_board(board,board_empty);
			mouse->mapa=&loadedImages[4];
			dados->tipo=NADA;
		}
		if(dados->tipo == NADA)
		{
			if(board[dados->posX+dados->posY*H_RES] != NULL)
			{
				unselect_monkey();
				board[dados->posX+dados->posY*H_RES]->select=SIM;
				dados->select=SIM;
				dados->selected_monkey=board[dados->posX+dados->posY*H_RES];
			}
			else
			{
				unselect_monkey();
				dados->select=NAO;
				dados->selected_monkey=NULL;
			}
		}
	}
}

void keypressed(unsigned long scancode, mouse_data *dados,imagem *mouse)
{
	printf("%x",scancode);
	if (scancode == Q_MAKE_CODE) {
		mouse->mapa=&loadedImages[0];
		mouse->range=300;
		dados->tipo=SUPER;
	} else if (scancode == W_MAKE_CODE) {
		add_balao(&loadedBaloes[1], 1, 1, inicio);
	} else if (scancode == E_MAKE_CODE) {
		add_balao(&loadedBaloes[5], 5, 7, inicio);
	} else if (scancode == R_MAKE_CODE) {
		//add_monkey(&loadedImages[3],&loadedImages[4], dados.posX, dados.posY);
	} else if (scancode == T_MAKE_CODE) {
		//add_projetil(&loadedImages[4], dados.posX, dados.posY,1,2,10);
	} else if (scancode == DELETE_MAKE_CODE){
		if(dados->select==SIM)
		{
			delete_monkey(dados->selected_monkey);
		}
	}

}
