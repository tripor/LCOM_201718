#ifndef _MACROS_H_
#define _MACROS_H_

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

#define LIMITE_DIREITA		  1750
#define LIMITE_ESQUERDA		  43
#define LIMITE_CIMA			  0
#define LIMITE_BAIXO		  640

#define QUADRADOS_VERTICAL	  10
#define QUADRADOS_HORIZONTAL  10
#define QUADRADOS_ESPACO_VERTICAL	  64
#define QUADRADOS_ESPACO_HORIZONTAL	  88



#endif
