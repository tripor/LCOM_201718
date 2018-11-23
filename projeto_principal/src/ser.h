#ifndef _SER_H_
#define _SER_H_
/** @defgroup ser ser
 * @{
 *
 * funcoes relativas ao funcionamento da comunicacao via serial port
 */
#include "data_storage.h"
/**
 * @brief esperar que o buffer de envio fique vazio
 *
 */
void esperar_vazio();
/**
 * @brief mando um macaco para o outro jogador
 *
 * @param posX posicao do macaco para enviar
 * @param posY posicao do macaco para enviar
 * @param tipo tipo de macaco a enviar
 */
void send_macaco(int posX,int posY,monkey_type tipo);
/**
 * @brief manda uma presença inicial para o outro jogador
 *
 */
void send_presence();
/**
 * @brief manda um balão para o outro jogador
 *
 * @param tipo tipo de balao a enviar
 */
void send_bloon(unsigned char tipo);
/**
 * @brief função para ligar os 2 jogadores no menu inicial
 *
 * @return returna o estado do jogador
 */
menu_multi ser_ih_inicial();
/**
 * @brief comunicação no jogo
 *
 * @return returna um estado do jogo
 */
menu_multi ser_ih();
/**
 * @brief subscreve aos interrupts do serial port
 *
 * @return returna o valor de hook ou -1 caso aconteca um erro
 */
int ser_subscribe(void);
/**
 * @brief unsubscribe aos interrupts do serial port
 *
 * @return returna 0 caso não aconteca erros ou -1 caso contrario
 */
int ser_unsubscribe();

/**@}*/

#endif
