#ifndef PORT_HEADER
#define PORT_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/
#include "GeneralTypes.h" 

/*--------------------------------------------------------------------------------
*                           Custom types
*--------------------------------------------------------------------------------*/
typedef struct __attribute__ ((aligned (1)))
{
    U8 pinPort  : 2;
    U8 pinNumber: 6;
    U8 pinMode  : 4;
    U8 pinSpeed : 4;
} PinConfig_Type;

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* Pin port */
#define PIN_PORT_A              0
#define PIN_PORT_B              1
#define PIN_PORT_C              2
#define PIN_PORT_D              3

/* Pin number */
#define PIN_NUMBER_0            0
#define PIN_NUMBER_1            1
#define PIN_NUMBER_2            2
#define PIN_NUMBER_3            3
#define PIN_NUMBER_4            4
#define PIN_NUMBER_5            5
#define PIN_NUMBER_6            6
#define PIN_NUMBER_7            7
#define PIN_NUMBER_8            8
#define PIN_NUMBER_9            9
#define PIN_NUMBER_10           10
#define PIN_NUMBER_11           11
#define PIN_NUMBER_12           12
#define PIN_NUMBER_13           13
#define PIN_NUMBER_14           14
#define PIN_NUMBER_15           15

/* Pin configuration mode */
#define PIN_MODE_NOT_USED       0
#define PIN_MODE_AIN            1
#define PIN_MODE_IN_FLOATING    2
#define PIN_MODE_IN_PULLDOWN    3
#define PIN_MODE_IN_PULLUP      4
#define PIN_MODE_OUT_OD         5
#define PIN_MODE_OUT_PP         6
#define PIN_MODE_AF_OD          7
#define PIN_MODE_AF_PP          8

/* Digital pin state */
#define PIN_STATUS_LOW          0
#define PIN_STATUS_HIGH         1

/*--------------------------------------------------------------------------------
*                           Functions prototypes
*--------------------------------------------------------------------------------*/
U8 Port_Init(void);
U8 Port_Deinit(void);

U8 Port_ReadPin(U8 port, U8 pin);
void Port_WritePin(U8 port, U8 pin, U8 status);
void Port_TogglePin(U8 port, U8 pin);

#endif /* PORT_HEADER */
 
 