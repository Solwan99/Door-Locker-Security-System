
/* MODULE      : LCD
 * file type   : H file
 * Description : Header file supports both 4 and 8 data bus mode for LCD
 * Notes       : Please check your connections and if it is different,
 *               redefine ports to be fit your drive connection
 *
 */

#ifndef _LCD_H_
#define _LCD_H_


                           /*********************************************
                                       H files Included
                           *********************************************/
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "stdlib.h"
             /*******************************************************************************
              *                      Preprocessor Macros                                    *
              *******************************************************************************/

                         /* LCD Data bits mode configuration */


#define DATA_BITS_MODE 4

/* Use higher 4 bits in the data port */
#if (DATA_BITS_MODE == 4)
#define UPPER_PORT_PINS
#endif

/* LCD HW Pins */
#define RS PD4
#define RW PD5
#define E  PD6
#define LCD_CTRL_PORT 			PORTD
#define LCD_CTRL_PORT_DIR 		DDRD

#define LCD_DATA_PORT 			PORTB
#define LCD_DATA_PORT_DIR 		DDRB

/* LCD Commands */
#define CLEAR_COMMAND 				0x01
#define FOUR_BITS_DATA_MODE 		0x02
#define TWO_LINE_LCD_Four_BIT_MODE 	0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 					0x0C
#define CURSOR_ON 					0x0E
#define SET_CURSOR_LOCATION 		0x80

/* LCD Parameters */
#define First_ROW 		0
#define Second_ROW 		1
#define Third_ROW 		2
#define Fourth_ROW 		3

#define First_Char 		0
#define Second_Char 	1
#define Third_Char 		2
#define Fourth_Char 	3
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_init(void);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(int data);

#endif /* LCD_H_ */

