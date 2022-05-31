/*-------------------------------------------------------------------
 * [FILE NAME]:hmi_ecu.h
 *
 * [AUTHOR(S)]:	Solwan Shokry
 *
 * [DESCRIPTION]: Header file for the HMI ECU.
 ---------------------------------------------------------------------*/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

/*-------------------------Includes-------------------------*/

#include "common_macros.h"
#include "micro_config.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"


/*-------------------------Definitions And Configurations-------------------------*/
#define PASSWORD_SIZE		5

#define CHANGE_PASSWORD  	0x07
#define OPEN_DOOR  			0x10
#define OK_STATUS  			0x08
#define ALERT_ENTRY   		0x20
#define NO_OREDR       		0x30

#define OPEN_CLOSE_TIME 	5
#define WAIT_TIME 			5

/*-------------------------Extern Module shared global variables-------------------------*/
uint8 g_Menu_Option;
/*-------------------------ENUMS-------------------------*/

/*-------------------------Structures and unions-------------------------*/


/*-------------------------------------------------------------------
 * [Structure NAME]:
 *
 * [Structure DESCRIPTION]:
 *
 * [Members]:
 ---------------------------------------------------------------------*/
typedef struct {

	uint8 Password[PASSWORD_SIZE];
	uint8 checkPassword[PASSWORD_SIZE];

}Password_ConfigType;

/*-------------------------FUNCTION-LIKE MACROS-------------------------*/

/*-------------------------Function Declarations-------------------------*/

/************************************************************************************
 * Function Name: Get_Password
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Get Password from the user and make check to it
 * It make sure that password == to its checking
 * If not , FUnction asks the user to enter the Password again

 ************************************************************************************/
void Get_Password(Password_ConfigType * Password_Config);
/*-------------------------------------------------------------------
 * [Function NAME]:
 *
 * [DESCRIPTION]:
 *
 * [Arguments]:
 *
 * [in]:
 * [out]:
 * [in/out]:
 * [Returns]:
 ---------------------------------------------------------------------*/
void Init_Fun(void);

/************************************************************************************
 * Function Name: CheckPassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Check The Password and its Re-typing

 ************************************************************************************/
uint8 CheckPassword(Password_ConfigType * Password_Config);


/************************************************************************************
 * Function Name: SendPassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Send the Password by the UART

 ************************************************************************************/
void SendPassword(Password_ConfigType * Password_Config);

/************************************************************************************
 * Function Name: Menu
 * Parameters (in): void
 * Return value: void
 * Description: Display on the LCD The Options which the user will choose one of them
 * Function is only for displaying , It Takes NO ACTION

 ************************************************************************************/
void Menu(void);

/************************************************************************************
 * Function Name: Change_PW_ECU1
 * Parameters (in): void
 * Return value: void
 * Description:  1) Sends a Command for the other ECU That It will change the Password
 *               2) Take a Password from the user
 *
 *               This Password is not the new password , this password will be sent to the
 *               Ctrl ECU to Be checked and compared by System Password

 *  ************************************************************************************/
void Change_PW_ECU1(void);

/************************************************************************************
 * Function Name: Option_Select
 * Parameters (in): uint8 data
 * Return value: uint8
 * Description:  It takes a value from the keypad and return corresponding command
 * to be used by the other ECU
 *
 * Mainly this function is used after Menu() Function
 *
 *  ************************************************************************************/

uint8 Option_Select(uint8 data);

/************************************************************************************
 * Function Name: Oped_door_ECU1
 * Parameters (in): void
 * Return value: void
 * Description:  1) Sends a Command for the other ECU That It will (Want only) to open the door
 *               2) Take a Password from the user
 *
 *               This Password is not the new password , this password will be sent to the
 *               Ctrl ECU to Be checked and compared by System Password

 *  ************************************************************************************/
void Oped_door_ECU1(void);
/************************************************************************************
 * Function Name: MSG
 * Parameters (in): uint8 *Str
 * Return value: void
 * Description: Prints a string on the connected LCD
 *  ************************************************************************************/
void MSG (uint8 *Str);

/************************************************************************************
 * Function Name: Require_Password
 * Parameters (in): void
 * Return value: void
 * Description:Take a Password from the user This Password is not the new password ,
 *this password will be sent to the Ctrl ECU to Be checked and compared by System Password

 *  ************************************************************************************/
void Require_Password(void) ;
#endif /* HMI_ECU_H_ */
