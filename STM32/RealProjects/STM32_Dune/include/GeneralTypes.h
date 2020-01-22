#ifndef GENERALTYPES_HEADER
#define GENERALTYPES_HEADER

/*--------------------------------------------------------------------------------
*                           Libraries
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Global variables
*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
*                           Const and Macro
*--------------------------------------------------------------------------------*/

/* Return codes */
#define     RETURN_OK       0
#define     RETURN_NOK      1

/* True and False */
#define     FALSE           0U
#define     TRUE            1U

/* States: ON, OFF */
#define     OFF             0U
#define     ON              1U

/*--------------------------------------------------------------------------------
*                           Typedef's
*--------------------------------------------------------------------------------*/

/* Unsigned Integers types */
typedef    unsigned char      U8;
typedef    unsigned short     U16;
typedef    unsigned long      U32;
typedef    unsigned long long U64;

/* Signed Integers types */
typedef    signed char        S8;
typedef    signed short       S16;
typedef    signed long        S32;
typedef    signed long long   S64;

/* Variable size arrays */
typedef U8 ArraySize11[11];


#endif /* GENERALTYPES_HEADER */

