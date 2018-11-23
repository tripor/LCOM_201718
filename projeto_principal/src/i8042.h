#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard
 */
#define BIT(n)                (0x01<<(n))

#define KBD_IRQ               1
#define TIMER0_IRQ            0

#define KBD_STATUS_REG        0x64

#define KBC_CMD_REG           0x64
#define KBC_CMD_REG_ARGS      0x60

#define KBC_READ_CB           0X20
#define KBC_WRITE_CB          0X60

#define KBD_IN_BUF            0x60
#define KBD_OUT_BUF           0x60
#define KBD_CMD_BUF           0x64

#define KBD_OBF               BIT(0) //0x01
#define KBD_IBF               BIT(1)

#define KBD_PAR_ERR           BIT(7)
#define KBD_TO_ERR            BIT(6)

#define BREAK_CODE            BIT(7)
#define ESC_BREAK_CODE        0x81

#define GET_CMD               0xCC //1100 1100
#define DISABLE_KBD           BIT(4)
#define DISABLE_MOUSE         BIT(5)
#define ENA_IN_MOUSE          BIT(1)
#define ENA_IN_KBD            BIT(0)

#define DELAY_US              20000

/**@}*/

#endif
