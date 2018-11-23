#ifndef _MACROS_H_
#define _MACROS_H_



#define BIT(n)                (0x01<<(n))

#define MOUSE_IRQ			  12
#define KBD_IRQ               1
#define TIMER0_IRQ            0

#define KBD_STATUS_REG        0x64

#define KBC_CMD_REG           0x64
#define KBC_CMD_REG_ARGS      0x60

#define KBC_READ_CB           0X20
#define KBC_WRITE_CB          0X60
#define MOUSE_WRITE_BYTE      0XD4
#define SET_STREAM_MODE       0xEA
#define SET_REMOTE_MODE       0xF0
#define ENABLE_DATA     	  0xF4
#define DISABLE_DATA     	  0xF5
#define READ_DATA             0xEB

#define KBD_IN_BUF            0x60
#define KBD_OUT_BUF           0x60
#define KBD_CMD_BUF           0x64

#define KBD_OBF               BIT(0) //0x01
#define KBD_IBF               BIT(1)
#define BIT3				  BIT(3)

#define KBD_PAR_ERR           BIT(7)
#define KBD_TO_ERR            BIT(6)
#define AUX                   BIT(5)
#define ACK					  0xFA
#define NACK				  0xFE
#define ERROR 				  0xFC

#define BREAK_CODE            BIT(7)
#define ESC_BREAK_CODE        0x81

#define GET_CMD               0xCC //1100 1100
#define DISABLE_KBD           BIT(4)
#define DISABLE_MOUSE         BIT(5)
#define ENA_IN_MOUSE          BIT(1)
#define ENA_IN_KBD            BIT(0)

#define GET_BIT0				  BIT(0)
#define GET_BIT1				  BIT(1)
#define GET_BIT2				  BIT(2)
#define GET_BIT3				  BIT(3)
#define GET_BIT4				  BIT(4)
#define GET_BIT5				  BIT(5)
#define GET_BIT6				  BIT(6)
#define GET_BIT7				  BIT(7)

#define DELAY_US              20000

#endif
