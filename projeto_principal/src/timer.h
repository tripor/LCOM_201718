#ifndef _TIMER_H_
#define _TIMER_H_
/** @defgroup lmlib lmlib
 * @{
 *
 * funcoes relativas ao timer
 */
/**
 * @brief subescreve aos interrupts do timer
 *
 * @return returna o valor do hook ou -1 caso aconteca um erro
 */
int timer_subscribe(void);
/**
 * @brief unsubscreve aos interrupts do time
 *
 * @return returna 0 caso aconteça tudo bem ou -1 caso contrario
 */
int timer_unsubscribe();

/**@}*/
#endif
