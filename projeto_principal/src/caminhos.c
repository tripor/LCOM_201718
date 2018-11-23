#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <string.h>

#include "lowProgram.h"
#include "macros.h"
#include "bitmap.h"


void preencher(imagem **board,int posX,int posY,imagem *nada)
{
	int i,j;
	if(posX>0 && posY>0) //para não preencher fora do ecra, não preciso de verificar na direita e em baixo porque o mapa não vai até ao fim do ecra
	{
		for(i=0;i<TAMANHO_BALAO_ALTURA;i++)
		{
			for(j=0;j<TAMANHO_BALAO_LARGURA;j++)
			{
				board[posX+ j + (posY+i)*H_RES]=nada;
			}
		}
	}
}

void criar_caminho1(caminho *inicio,imagem **board)
{
	short posY=300,posX=-20;
	int x,indice=1;
	imagem *nada=malloc(sizeof(imagem));
	nada->range=0;
	caminho* antes=inicio;
	inicio->posX=posX;
	inicio->posY=posY;
	inicio->indice=indice;
	indice++;
	for (x = 0; x < 120; x++) {//direita
		caminho* proximo=malloc(sizeof(caminho));
		proximo->posX=posX;
		proximo->posY=posY;
		proximo->next=NULL;
		proximo->indice=indice;
		antes->next=proximo;
		antes=proximo;
		preencher(board,posX,posY,nada);
		posX++;
		indice++;
	}
	for (x = 0; x < 225; x++) {//cima
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY--;
		indice++;
	}
	for (x = 0; x < 665; x++) {//direita
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX++;
		indice++;
	}
	for (x = 0; x < 190; x++) {//baixo
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY++;
		indice++;
	}
	for (x = 0; x < 35; x++) {//esquerda
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX--;
		indice++;
	}
	for (x = 0; x < 145; x++) {//cima
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY--;
		indice++;
	}
	for (x = 0; x < 600; x++) {//esquerda
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX--;
		indice++;
	}
	for (x = 0; x < 390; x++) {//baixo
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY++;
		indice++;
	}
	for (x = 0; x < 600; x++) {//direita
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX++;
		indice++;
	}
	for (x = 0; x < 145; x++) {//cima
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY--;
		indice++;
	}
	for (x = 0; x < 35; x++) {//direita
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX++;
		indice++;
	}
	for (x = 0; x < 190; x++) {//baixo
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY++;
		indice++;
	}
	for (x = 0; x < 670; x++) {//esquerda
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX--;
		indice++;
	}
	for (x = 0; x < 210; x++) {//cima
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posY--;
		indice++;
	}
	for (x = 0; x < 130; x++) {//esquerda
		caminho* proximo = malloc(sizeof(caminho));
		proximo->posX = posX;
		proximo->posY = posY;
		proximo->next = NULL;
		proximo->indice=indice;
		antes->next = proximo;
		antes = proximo;
		preencher(board,posX,posY,nada);
		posX--;
		indice++;
	}
}
