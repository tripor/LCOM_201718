#ifndef _DATA_STORAGE_H_
#define _DATA_STORAGE_H_

#include "bitmap.h"

typedef enum {RIGHTCLICK,RIGHTDOWN,RIGHTUP} right_state;
typedef enum {LEFTCLICK,LEFTDOWN,LEFTUP} left_state;
typedef enum {NADA,DART,SUPER} monkey_type;
typedef enum {SIM,NAO} selecionado;

typedef struct li{
	short posX;
	short posY;
	unsigned long indice;
	struct li *next;
}caminho;

typedef struct im {
	int posX;
	int posY;
	unsigned long range;
	unsigned long speed_lancar;
	unsigned long pierce;
	unsigned long tempo;
	unsigned long aa;
	selecionado select;
	struct im **linha_endereco;
	struct im* next;
	struct im* before;
	Bitmap *mapa;
	Bitmap *projetil_tipo;
}imagem;

typedef struct {

	unsigned long posX;
	unsigned long posY;
	imagem *selected_monkey;
	selecionado select;
	monkey_type tipo;
	right_state rightBotton;
	left_state leftBotton;

}mouse_data;

typedef struct balao {
	int posX;
	int posY;
	unsigned long hp;
	unsigned long speed;
	struct balao ** linha_endereco;
	caminho* percurso;
	struct balao* next;
	struct balao* before;
	Bitmap *mapa;
}balao_type;
typedef struct dardo {
	int posX;
	int posY;
	unsigned long pierce;
	unsigned long speedX;
	unsigned long speedY;
	struct dardo* next;
	struct dardo* before;
	Bitmap *mapa;
}projetil;

typedef struct{
	imagem *primeiraImagem;
	imagem *ultimaImagem;
	int size;
}conjunto;

typedef struct{
	balao_type *primeiraBalao;
	balao_type *ultimaBalao;
	int size;
}conjunto_baloes;

typedef struct{
	projetil *primeiraProjeteis;
	projetil *ultimaProjeteis;
	int size;
}conjunto_projeteis;



#endif
