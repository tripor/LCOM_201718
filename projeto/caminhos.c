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
	if(posX>0 && posY>0)
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
	short posY=270,posX=-10;
	int x,indice=1;
	imagem *nada=malloc(sizeof(imagem));
	nada->range=0;
	caminho* antes=inicio;
	inicio->posX=posX;
	inicio->posY=posY;
	inicio->indice=indice;
	indice++;
	for (x = 0; x < 110; x++) {
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
	for (x = 0; x < 210; x++) {
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
	for (x = 0; x < 670; x++) {
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
	for (x = 0; x < 210; x++) {
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
	for (x = 0; x < 75; x++) {
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
	for (x = 0; x < 140; x++) {
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
	for (x = 0; x < 530; x++) {
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
	for (x = 0; x < 350; x++) {
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
	for (x = 0; x < 530; x++) {
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
	for (x = 0; x < 140; x++) {
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
	for (x = 0; x < 75; x++) {
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
	for (x = 0; x < 210; x++) {
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
	for (x = 0; x < 670; x++) {
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
	for (x = 0; x < 210; x++) {
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
	for (x = 0; x < 130; x++) {
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
