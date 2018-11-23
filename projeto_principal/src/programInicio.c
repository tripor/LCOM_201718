#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#include "programInicio.h"
#include "lowProgram.h"
#include "macros.h"


static Bitmap* loadedImages;
static menu_inicial estado_inicial;


void comeca_inicio()
{
	loadedImages = malloc(sizeof(Bitmap) * 2);
	loadedImages[0] = *(load_imagem("inicio"));
	loadedImages[1] = *(load_imagem("inicio_select"));
	loadedImages[2] = *(load_imagem("inicio_menu"));
}

void mouse_controlador_inicial(mouse_data *dados,imagem *mouse,menu *estado,unsigned char inicial[],int *ajuda)
{

	if(dados->rightBotton == RIGHTCLICK)
	{

	}
	if(dados->leftBotton == LEFTCLICK && *ajuda==1)//caso menu inicial
	{
		if(dados->posX>=380 && dados->posX<=615 && dados->posY>=515 && dados->posY<=625)
		{
			*ajuda=2;
			imagem *ajuda = malloc(sizeof(imagem));
			ajuda->mapa = &loadedImages[2];
			ajuda->posX = 110;
			ajuda->posY = 50;
			desenhar_imagem(inicial,ajuda);
			free(ajuda);
		}
	}
	else if(dados->leftBotton == LEFTCLICK && *ajuda==2)//caso sub menu
	{
		if(dados->posX>=323 && dados->posX<=660 && dados->posY>=320 && dados->posY<=390)
		{
			*estado=JOGO;
		}
		else if(dados->posX>=400 && dados->posX<=605 && dados->posY>=417 && dados->posY<=608)
		{
			*estado=JOGOMULTI;
		}
	}

	//pequena animaçao para quando o rato fica por cima do botão
	if (dados->posX >= 380 && dados->posX <= 615 && dados->posY >= 515 && dados->posY <= 625 && *ajuda==1) {
		if(estado_inicial==UN)
		{
			imagem *ajuda = malloc(sizeof(imagem));
			ajuda->mapa = &loadedImages[1];
			ajuda->posX = 0;
			ajuda->posY = 0;
			desenhar_imagem(inicial, ajuda);
			free(ajuda);
			estado_inicial=SEL;
		}
	}
	else if(estado_inicial==SEL && *ajuda==1)
	{
		imagem *ajuda = malloc(sizeof(imagem));
		ajuda->mapa = &loadedImages[0];
		ajuda->posX = 0;
		ajuda->posY = 0;
		desenhar_imagem(inicial, ajuda);
		free(ajuda);
		estado_inicial=UN;
	}
}

void init_inicio(unsigned char buffer[],unsigned char inicial[])
{
	int x;
	for (x = 0; x < V_RES * H_RES; x++) {
		buffer[x] = 0;
		inicial[x] = 0xff;
	}
	imagem *ajuda = malloc(sizeof(imagem));
	ajuda->mapa = &loadedImages[0];
	ajuda->posX = 0;
	ajuda->posY = 0;
	desenhar_imagem(inicial,ajuda);
	free(ajuda);
	memcpy(buffer,inicial,V_RES * H_RES);
	estado_inicial=UN;

}
