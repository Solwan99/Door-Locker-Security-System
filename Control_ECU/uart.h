/* MODULE      : UART
 * file type   : H file
 * Description : A default UART communication peripheral of ATmega16 MCU
 * Notes       : Please check your connections and if it is different,
 *               redefine ports to be fit your drive connection
 *
 */

#ifndef UART_H_
#define UART_H_

/*******************************************************************************
 *                      Included files                                         *
 *******************************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */
#define USART_BAUDRATE 9600

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(void);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
