#ifndef _PROGRAM_INICIO_H_
#define _PROGRAM_INICIO_H_
/** @defgroup program_inicio program_inicio
 * @{
 *
 * funcoes relativas ao menu inicial
 */
#include "data_storage.h"
/**
 * @brief funcao chamada no inicio do menu
 *
 */
void comeca_inicio();
/**
 * @brief trata da informação do rato para o menu inicial
 *
 * @param dados dados relativos ao rato
 * @param mouse imagem do rato
 * @param estado estado do programa
 * @param inicial buffer do menu inicial
 * @param ajuda distincao entre os menus iniciais
 */
void mouse_controlador_inicial(mouse_data *dados,imagem *mouse,menu *estado,unsigned char inicial[],int *ajuda);
/**
 * @brief inicial o buffer e o buffer inicial para o menu inicial
 *
 * @param buffer buffer do menu inicial
 * @param inicial buffer inicial do menu
 */
void init_inicio(unsigned char buffer[],unsigned char inicial[]);

/**@}*/

#endif
