#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "data_storage.h"

int aconteceu_colisao(balao_type *balao,projetil *dardo);
void comeca(imagem **board);
void update();
void mouse_controlador(mouse_data *dados,imagem *mouse,imagem **board,imagem **board_empty);
void keypressed(unsigned long scancode, mouse_data *dados,imagem *mouse);

#endif
