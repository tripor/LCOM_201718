#ifndef _LOWPROGRAM_H_
#define _LOWPROGRAM_H_

#include "data_storage.h"

void avancar();
void avancar_projeteis(unsigned char buffer[], balao_type **colisao,balao_type **vazio);
void delete_monkey(imagem* deleted);
void delete_balao(balao_type* deleted);
void delete_projetil(projetil* deleted);
void add_balao(Bitmap* nom, unsigned long hp, unsigned long speed,caminho *inicio);
void add_monkey(Bitmap* nom, Bitmap* tipo,int posX, int posY,unsigned long range,unsigned long speed_lancar,unsigned long pierce,unsigned long aa);
void add_projetil(Bitmap* nom , int posX, int posY, unsigned long pierce,long speedX, long speedY);
Bitmap* load_imagem(char* tipo_objeto);
void update_ecra(unsigned char *video_mem, unsigned char buffer[],unsigned char inicial[]);
int testar_board(int posX,int posY,imagem **board,Bitmap* nom);
void reescrever_board(imagem **board,imagem **board_empty);
void desenhar_objeto(unsigned char buffer[], balao_type **colisao);
void desenhar_overlay(unsigned char buffer[], imagem *novo);
void desenhar_rato(unsigned char buffer[], mouse_data dados, imagem *novo);
void unselect_monkey();
void init(unsigned char buffer[], unsigned char inicial[],balao_type **colisao,imagem **board);

#endif
