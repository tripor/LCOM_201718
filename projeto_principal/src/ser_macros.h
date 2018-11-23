#ifndef _SER_MACROS_H_
#define _SER_MACROS_H_
/** @defgroup ser_macros ser_macros
 * @{
 *
 * macros relativos ao funcionamento das comunicacoes
 */
#define COM1				4
#define COM2				3

#define BIT(n)              (0x01<<(n))

#define SER_RX_INT			BIT(2)
#define SER_TX_INT			BIT(1)
#define SER_RX_ERR			0x6 //0110

#define SER_EN_R			BIT(0)
#define SER_EN_T			BIT(1)
#define SER_EN_RL			BIT(2)

#define SER_IIR				2
#define SER_IER	 			1
#define SER_RBR	 			0
#define SER_THR	 			0
#define SER_LCR	 			3
#define SER_LSR	 			5
#define SER_DLL	 			0
#define SER_DLM	 			1


#define SER_INT_PEND		BIT(0)
#define INT_ID				0xE

#define LCR_8BIT			0x3 //0011
#define LCR_1_STOP			0
#define LCR_ODD				BIT(3)
#define LCR_DLAB			BIT(7)
#define LCR_UN_DLAB			0

#define SERIAL_PORT			0x3F8

#define LSB					0xFF
#define MSB					0xFF00

/**@}*/
#endif
