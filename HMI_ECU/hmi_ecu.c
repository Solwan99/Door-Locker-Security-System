/*-------------------------------------------------------------------
 * [FILE NAME]:hmi_ecu.c
 *
 * [AUTHOR(S)]:	Solwan Shokry
 *
 * [DESCRIPTION]: Source file for the HMI ECU.
 ---------------------------------------------------------------------*/


/*-------------------------Includes-------------------------*/
#include "hmi_ecu.h"
/*-------------------------Global Variables-------------------------*/

/*Structure to hold the PW values of the HMI ECU*/
Password_ConfigType PassWord_Configuration_HMI;

/*-------------------------Function Definitions-------------------------*/

int main(void) {

	/*Init_Func to Initiate the system of the ECU*/
	Init_Fun();


	/*Get Password from the user and Checking it */
	Get_Password(&PassWord_Configuration_HMI);


	/*After Getting a checked PW , Send it to the Ctrl ECU*/
	SendPassword(&PassWord_Configuration_HMI);

	while (1) {

		/*Show to the user the options of the system
				 * on a LCD to choose an action to be transfered to
				 * the Ctrl ECU and be performed
				 * */
		Menu();

		/*Take the action from keypad and stored it in
				 * a variable*/
		g_Menu_Option = KeyPad_getPressedKey();
		_delay_ms(200);


		/*Condition Checking variables */
		uint8 loop_Checking_1;
		uint8 loop_Checking_2;



		if (Option_Select(g_Menu_Option) == FALSE) {
			/*Do Nothing and Return to Menu*/
		}

		else if (Option_Select(g_Menu_Option) == CHANGE_PASSWORD) {
			uint8 Attemps;
			do {
				/*Send to the ECU_2 Password change Request*/
				/*Request from the user the password and send it
				 * to Ctrl ECU */
				Change_PW_ECU1();

				uint8 check_status = UART_recieveByte();

				if (check_status == TRUE) {

					Get_Password(&PassWord_Configuration_HMI);
					/*Send The Password to ECU_2 */
					SendPassword(&PassWord_Configuration_HMI);
					_delay_ms(200);
					MSG("PW is Changed");
					loop_Checking_1 = TRUE;

				} else {
					loop_Checking_1 = FALSE;
					Attemps++;
				}

				/*Make the user to re-enter the password
				 * only Three times , after that ,
				 * Return to main menu*/
			} while (loop_Checking_1 == FALSE && Attemps < 3);

			Attemps = 0;
		}

		else if (Option_Select(g_Menu_Option) == OPEN_DOOR) {

			uint8 Attemps;

			do {
				/*Send to the ECU_2 Password change Request*/
				/*Request from the user the password and send it
				 * to Ctrl ECU */

				/*Take password and send password & open_door
				 * command to the Ctrl ECU */
				Oped_door_ECU1();

				uint8 check_status = UART_recieveByte();

				if (check_status == TRUE) {

					MSG("Door UnLocking");

					uint8 Motor_finish = UART_recieveByte();


					/*This looping to make sure the motor finish
					 * its work first in the other ECU and then
					 * Go ahead to complete the code */
					while (Motor_finish != TRUE) {

					}

					loop_Checking_2 = TRUE;

				} else {
					loop_Checking_2 = FALSE;
					Attemps++;
				}
				/*Make the user to re-enter the password
								 * only Three times , after that ,
								 * Return to main menu*/
			} while (loop_Checking_2 == FALSE && Attemps < 3);

			Attemps = 0;

		}

		else {
			/*No Code*/
		}


	}

}

/************************************************************************************
 * Function Name: Init_Fun
 * Parameters (in): void
 * Return value: void
 * Description: Initiate the system
 *            - Initiate LCD
 *            - Initiate UART
 ************************************************************************************/
void Init_Fun(void) {

	LCD_init();
	UART_init();

}

/************************************************************************************
 * Function Name: Get_Password
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Get Password from the user and make check to it
 * It make sure that password == to its checking
 * If not , FUnction asks the user to enter the Password again

 ************************************************************************************/
void Get_Password(Password_ConfigType * Password_Config) {

	uint8 Counter_1 = 0;
	uint8 Counter_2 = 0;
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_displayString("Please, Enter");
	LCD_goToRowColumn(1, 0);
	LCD_displayString("New PASSWORD");
	LCD_goToRowColumn(2, 0);

	while (Counter_1 < PASSWORD_SIZE) {

		uint8 Key = KeyPad_getPressedKey();

		if (Key >= 0 && Key <= 9) {

			Password_Config->Password[Counter_1] = Key;
			LCD_displayCharacter('*');
			_delay_ms(350);
			Counter_1++;

		}

	}
	Counter_1 = 0;

	do {

		LCD_clearScreen();
		LCD_goToRowColumn(0, 0);
		LCD_displayString("Please,Re-enter");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("PASSWORD");
		LCD_goToRowColumn(2, 0);

		while (Counter_2 < PASSWORD_SIZE) {

			uint8 Key = KeyPad_getPressedKey();

			if (Key >= 0 && Key <= 9) {

				Password_Config->checkPassword[Counter_2] = Key;
				LCD_displayCharacter('*');
				_delay_ms(350);
				Counter_2++;

			}

		}
		Counter_2 = 0;

	} while (CheckPassword(Password_Config) == FALSE);

}

/************************************************************************************
 * Function Name: CheckPassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Check The Password and its Re-typing

 ************************************************************************************/
uint8 CheckPassword(Password_ConfigType * Password_Config) {

	uint8 state = TRUE;

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {

		if (Password_Config->Password[Counter] != Password_Config->checkPassword[Counter]) {

			state = FALSE;
		} else {
			/*No Code*/
		}

	}

	return state;
}

/************************************************************************************
 * Function Name: SendPassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Send the Password by the UART

 ************************************************************************************/

void SendPassword(Password_ConfigType * Password_Config) {

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		UART_sendByte(Password_Config->Password[Counter]);

	}

}


/************************************************************************************
 * Function Name: Menu
 * Parameters (in): void
 * Return value: void
 * Description: Display on the LCD The Options which the user will choose one of them
 * Function is only for displaying , It Takes NO ACTION

 ************************************************************************************/
void Menu(void) {

	LCD_clearScreen();
	LCD_displayString("(+):Change PW");
	LCD_goToRowColumn(Second_ROW, First_Char);
	LCD_displayString("(=):OpenDoor");

}

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
void Change_PW_ECU1(void) {
	UART_sendByte(CHANGE_PASSWORD);
	uint8 Required_PW[PASSWORD_SIZE];
	uint8 Counter_1 = 0;
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_displayString("Please, Enter");
	LCD_goToRowColumn(1, 0);
	LCD_displayString("OLD PASSWORD");
	LCD_goToRowColumn(2, 0);

	while (Counter_1 < PASSWORD_SIZE) {

		uint8 Key = KeyPad_getPressedKey();

		if (Key >= 0 && Key <= 9) {

			Required_PW[Counter_1] = Key;
			LCD_displayCharacter('*');
			_delay_ms(350);
			Counter_1++;
		}
	}
	Counter_1 = 0;

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		UART_sendByte(Required_PW[Counter]);

	}

}

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
uint8 Option_Select(uint8 data) {

	if (data == '+') {
		return CHANGE_PASSWORD;

	} else if (data == '=') {
		return OPEN_DOOR;
	}

	else {
		return FALSE;
	}
}

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
void Oped_door_ECU1(void) {
	UART_sendByte(OPEN_DOOR);
	uint8 Required_PW[PASSWORD_SIZE];
	uint8 Counter_1 = 0;
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_displayString("Please, Enter");
	LCD_goToRowColumn(1, 0);
	LCD_displayString("The PASSWORD");
	LCD_goToRowColumn(2, 0);

	while (Counter_1 < PASSWORD_SIZE) {

		uint8 Key = KeyPad_getPressedKey();

		if (Key >= 0 && Key <= 9) {

			Required_PW[Counter_1] = Key;
			LCD_displayCharacter('*');
			_delay_ms(350);
			Counter_1++;
		}
	}
	Counter_1 = 0;

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		UART_sendByte(Required_PW[Counter]);

	}

}

/************************************************************************************
 * Function Name: MSG
 * Parameters (in): uint8 *Str
 * Return value: void
 * Description: Prints a string on the connected LCD
 *  ************************************************************************************/


void MSG(uint8 *Str) {
	LCD_clearScreen();
	LCD_displayString(Str);
	_delay_ms(600);
}


/************************************************************************************
 * Function Name: Require_Password
 * Parameters (in): void
 * Return value: void
 * Description:Take a Password from the user This Password is not the new password ,
 *this password will be sent to the Ctrl ECU to Be checked and compared by System Password

 *  ************************************************************************************/
void Require_Password(void) {

	uint8 Required_PW[PASSWORD_SIZE];
	uint8 Counter_1 = 0;
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_displayString("Please, Enter");
	LCD_goToRowColumn(1, 0);
	LCD_displayString("The PASSWORD");
	LCD_goToRowColumn(2, 0);

	while (Counter_1 < PASSWORD_SIZE) {

		uint8 Key = KeyPad_getPressedKey();

		if (Key >= 0 && Key <= 9) {

			Required_PW[Counter_1] = Key;
			LCD_displayCharacter('*');
			_delay_ms(350);
			Counter_1++;
		}
	}
	Counter_1 = 0;

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		UART_sendByte(Required_PW[Counter]);

	}

}
