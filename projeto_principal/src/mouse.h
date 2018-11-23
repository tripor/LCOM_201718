#ifndef _MOUSE_H_
#define _MOUSE_H_
/** @defgroup mouse mouse
 * @{
 *
 * funcoes relaticas ao mouse
 */
#include "data_storage.h"
/**
 * @brief trata da informação recolhida do rato
 *
 * @param packet informacao relativa ao rato
 * @param dados dados retalivos ao rato
 * @return returna os dados relativos ao rato
 */
mouse_data mouse_handler(unsigned long packet[], mouse_data dados);
/**
 * @brief obtem o stat reg
 *
 * @return returna o stat reg
 */
unsigned long get_statreg();
/**
 * @brief obtem um packet do rato
 *
 * @return returna um codigo do rato
 */
unsigned long mouse_get_code();
/**
 * @brief escreve um comando, é usada esta função depois do subscribe
 *
 * @param cmd comando que quero escrever
 * @return return 1 se ocorrer um erro ou 0 caso tenha corrido tudo bem
 */
int mouse_write_cmd(unsigned long cmd);
/**
 * @brief faz subscribe aos interrupts do mouse
 *
 * @return returna o valor do hook ou -1 se algo correr de errado
 */
int mouse_subscribe(void);
/**
 * @brief faz unsubscribe aos interrupts do mouse
 *
 * @return returna 0 se correr tudo bem ou 1 caso contrario
 */
int mouse_unsubscribe();

/**@}*/
#endif
