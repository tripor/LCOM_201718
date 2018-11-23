#ifndef _MACROS_H_
#define _MACROS_H_
/** @defgroup macros macros
 * @{
 *
 * dados importantes relativos ao funcionamento do programa
 */
#define BIT(n)                (0x01<<(n))

#define ESC_BREAK_CODE        0x81


#define Q_MAKE_CODE           0x10
#define W_MAKE_CODE           0x11
#define E_MAKE_CODE           0x12
#define R_MAKE_CODE           0x13
#define T_MAKE_CODE           0x14
#define Y_MAKE_CODE           0x15
#define U_MAKE_CODE           0x16
#define I_MAKE_CODE           0x17
#define O_MAKE_CODE           0x18
#define P_MAKE_CODE           0x19
#define DELETE_MAKE_CODE      0x0e

#define BRANCO				  0Xff
#define PRETO				  0X00

#define KBD_OBF               BIT(0) //0x01

#define H_RES				  2048
#define H_RES_REAL			  1024
#define V_RES				  768
#define BITS_PER_PIXEL		  8

#define TAMANHO_BALAO_LARGURA 32
#define TAMANHO_BALAO_ALTURA  45

#define LIMITE_DIREITA		  1790
#define LIMITE_ESQUERDA		  0
#define LIMITE_CIMA			  0
#define LIMITE_BAIXO		  665

#define QUADRADOS_VERTICAL	  10
#define QUADRADOS_HORIZONTAL  10
#define QUADRADOS_ESPACO_VERTICAL	  64
#define QUADRADOS_ESPACO_HORIZONTAL	  88


#define STARTING_MONEY		  3000
#define STARTING_VIDA		  150

#define PRECO_DART			  250
#define PIERCE_DART			  1
#define AA_DART				  30
#define VELO_DART			  50
#define RANGE_DART			  150

#define PRECO_SUPER			  2500
#define PIERCE_SUPER		  1
#define AA_SUPER			  1
#define VELO_SUPER			  50
#define RANGE_SUPER			  300

/**@}*/


#endif
