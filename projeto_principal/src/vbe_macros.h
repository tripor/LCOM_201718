#ifndef __MACROS_H
#define __MACROS_H
/** @defgroup vbe_macros vbe_macros
 * @{
 *
 * macros relativos ao funcionamento do modo grafico
 */
#define BIT(n)                           (0x0001<<(n))

//FUNCTIONS
#define FUNCTION_RETURN_CONTROLLER_INFO  0x00
#define FUNCTION_RETURN_MODE_INFO        0x01
#define FUNCTION_SET_VBE_MODE            0x02
#define FUNCTION_SET_TEXT_MODE           0x03

//OPERATING MODES
#define MODE_101						 0x101
#define MODE_103						 0x103
#define MODE_105						 0x105
#define MODE_117                         0x117
#define H_RES_105						 2048
#define V_RES_105						 768

#define MODE_107						 0x107

//OUTRAS COISAS
#define INT_NU							 0x10
#define VBE_CALL						 0x4F

/**@}*/
#endif /* __MACROS_H */
