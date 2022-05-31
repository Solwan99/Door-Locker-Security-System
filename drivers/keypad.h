/*-------------------------------------------------------------------
 * [Module Name]: KEYPAD
 *
 * [FILE NAME]: keypad.h
 *
 * [AUTHOR(S)]:	Solwan Shokry
 *
 * [DESCRIPTION]: Header File for Keypad Module interfaced with ATmega16 MCU
 ---------------------------------------------------------------------*/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/*-------------------------Includes-------------------------*/
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*-------------------------Definitions And Configurations-------------------------*/

/* Keypad configurations for number of rows and columns */
#define N_col 4
#define N_row 4

/* Keypad Port Configurations */
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN  PINA
#define KEYPAD_PORT_DIR DDRA

/*-------------------------Function Declarations-------------------------*/

/*-------------------------------------------------------------------
 * [Function NAME]:	KeyPad_getPressedKey
 *
 * [DESCRIPTION]:	Function responsible for getting the pressed keypad key
 *
 * [Arguments]:
 *
 * [in]:
 * [out]:
 * [in/out]:
 * [Returns]:	unsigned integer with a value equivalent to the pressed key-number value
 ---------------------------------------------------------------------*/
uint8 KeyPad_getPressedKey(void);


#endif /* KEYPAD_H_ */
