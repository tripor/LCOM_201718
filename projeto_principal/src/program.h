#ifndef _PROGRAM_H_
#define _PROGRAM_H_
/** @defgroup program program
 * @{
 *
 * funcoes de alto nivel para o funcionamento do programa
 */
#include "data_storage.h"
/**
 * @brief caso aconte�a um colis�o entre um bal�o e um dardo esta fun��o � chamada
 *
 * @param balao balao com que ocorreu a colisao
 * @param dardo dardo com que ocorreu a colisao
 * @return retorna 2 se for preciso apagar o projetil ou 1 caso contrario
 */
int aconteceu_colisao(balao_type *balao,projetil *dardo);
/**
 * @brief fun��o que carrega todas as imagens para a memoria
 *
 */
void carregar_imagens();
/**
 * @brief fun��o que � chamada no inicio do jogo
 *
 * @param board mapa com as posicoes dos macaco
 */
void comeca(imagem **board);
/**
 * @brief fun��o que orienta o macaco e o dardo segundo uma posi��o dada
 *
 * @param monkey macaco que quero orientar a posicao
 * @param pos posicao que quero que ele vire
 */
void orientar_pos(imagem *monkey,int pos);
/**
 * @brief fun��o que calcula a posi��o a orientar o macaco e o dardo. Existem 8 posi��es posiveis
 *
 * @param monkey macaco que quero orientar
 * @param speedX velocidade de lancamento horizontal
 * @param speedY velocidade de lancamento vertical
 */
void orientar_monkey(imagem *monkey,int speedX,int speedY);
/**
 * @brief fun��o chamada quando um bal�o chega ao fim do percurso
 *
 * @param quantidade quantidade de vida que o jogador vai perder
 */
void perde_vida(unsigned long quantidade);
/**
 * @brief fun��o de desenha um numero no ecra
 *
 * @param posX posicao para onde desenhar o numero
 * @param posY posicao para onde desenhar o numero
 * @param numero valor do numero que quero desenhar
 * @param buffer buffer para onde quero desenhar o numero
 */
void desenhar_numero(int posX,int posY,int numero,unsigned char buffer[]);
/**
 * @brief fun��o que � chamada a cada atualiza��o do ecra
 *
 * @param buffer buffer
 * @param estado estado do jogo
 * @return returna o estado do jogo
 */
menu update(unsigned char buffer[],menu estado);
/**
 * @brief fun��o que trata do controlo do rato
 *
 * @param dados dados relativos ao rato
 * @param mouse imagem do rato
 * @param board mapa com as posicoes dos macacos
 * @param board_empty mapa sem as posicoes dos macacos
 */
void mouse_controlador(mouse_data *dados,imagem *mouse,imagem **board,imagem **board_empty);
/**
 * @brief fun��o que � chamada quando uma tecla � clicada
 *
 * @param scancode code de qual tecla foi pressionada
 * @param dados dados relativos ao rato
 * @param mouse imagem do rato
 */
void keypressed(unsigned long scancode, mouse_data *dados,imagem *mouse);
/**
 * @brief na vers�o multiplayer para quando for clicada uma tecla pelo jogador 1
 *
 * @param scancode code de qual tecla foi pressionada
 * @param dados dados relativos ao rato
 * @param mouse imagem do rato
 */
void keypressed_jogador1(unsigned long scancode, mouse_data *dados,imagem *mouse);
/**
 * @brief na vers�o multiplayer quando for clicada uma tecla pelo jogador 2
 *
 * @param scancode code de qual tecla foi pressionada
 */
void keypressed_jogador2(unsigned long scancode);
/**
 * @brief fun��es que adicionam objetos
 *
 * @param posX posicao onde quero adicionar o objeto
 * @param posY posicao onde quero adicionar o objeto
 */
void add_dart(int posX,int posY);
/**
 * @brief fun��es que adicionam objetos
 *
 * @param posX posicao onde quero adicionar o objeto
 * @param posY posicao onde quero adicionar o objeto
 */
void add_super(int posX,int posY);
/**
 * @brief fun��es que adicionam objetos
 *
 */
void add_red();
/**
 * @brief fun��es que adicionam objetos
 *
 */
void add_blue();
/**
 * @brief fun��es que adicionam objetos
 *
 */
void add_green();
/**
 * @brief fun��es que adicionam objetos
 *
 */
void add_yellow();
/**
 * @brief fun��es que adicionam objetos
 *
 */
void add_pink();
/**@}*/
#endif
