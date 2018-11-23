#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
/** @defgroup keyboard keyboard
 * @{
 *
 * Funcoes relacionadas com o funcionamento do keyboard
 */
/**
 * @brief Permite que os interrupts do kernal sejam recebidos.Return ao valor original de hook
 *
 * @return returna o hook
 */
int kbd_subscribe(void);
/**
 * @brief Volta ao estado anterior ao subscribe.
 *
 * @return returna 0 se correu tudo bem ou 1 caso contrario
 */
int kbd_unsubscribe();
/**
 * @brief Função que é chamada quando há um interrupt do teclado.
 *
 * @param code2 para onde vai guardar o scancode
 * @return 0 se correu tudo bem ou -1 caso contrario
 */
int kbd_handler(unsigned long *code2);

/**@}*/
#endif
