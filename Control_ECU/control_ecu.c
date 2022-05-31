/*-------------------------------------------------------------------
 * [FILE NAME]: control_ecu.c
 *
 * [AUTHOR(S)]:	Solwan Shokry
 *
 * [DESCRIPTION]: Source File for the Control ECU MCU.
 ---------------------------------------------------------------------*/


/*-------------------------Includes-------------------------*/
#include "control_ecu.h"
/*-------------------------Global Variables-------------------------*/

Password_ConfigType PassWord_Configuration_CTRL;
Password_EEPROM_ConfigType PassWord_Configuration_EEPROM;
uint8 error_counter;
/*-------------------------Function Definitions-------------------------*/
int main(void) {
	Init_Fun();
	RecievePassword(&PassWord_Configuration_CTRL);

	/*ShowPassword(&PassWord_Configuration_CTRL);*/

	SavePassword_EEPROM(&PassWord_Configuration_CTRL);
	ReadPassword_EEPROM( Begining_Address, &PassWord_Configuration_EEPROM);

	while (1) {

		uint8 rec_data = Recieve_Option_Select();

		if (rec_data == CHANGE_PW) {


			/*Get the Password entered by the user*/
			Entered_PW_ECU2(&PassWord_Configuration_EEPROM);


			/*Check on the Password , is that same to the one in EEPROM ?*/
			uint8 check_status = CheckPassword(&PassWord_Configuration_EEPROM);

			/*Send to the HMI Is that password is Right or Wrong */
			UART_sendByte(check_status);

			if (check_status == TRUE) {

				/*If the Password is right */
				RecievePassword(&PassWord_Configuration_CTRL);
				SavePassword_EEPROM(&PassWord_Configuration_CTRL);
				ReadPassword_EEPROM( Begining_Address,
						&PassWord_Configuration_EEPROM);


			}


		}

		else if (rec_data == OPEN_DOOR) {
			/*Get the Password entered by the user*/
			Entered_PW_ECU2(&PassWord_Configuration_EEPROM);

			/*Check on the Password , is that same to the one in EEPROM ?*/
			uint8 check_status = CheckPassword(&PassWord_Configuration_EEPROM);

			/*Send to the HMI Is that password is Right or Wrong */
			UART_sendByte(check_status);

			if (check_status == TRUE) {
				Control_door(OPEN_CLOSE_TIME, WAIT_TIME);

				/*Send to the HMI ECU That the Motor
				 * finished its work*/
				UART_sendByte(TRUE);

			}

		}
		else
		{
			/*No Code*/
		}
	}
}

/************************************************************************************
 * Function Name: Init_Function
 * Parameters (in): void
 * Return value: void
 * Description: Initiate the system
 *            - It enable the General Interrupt enable bit
 *            - Initiate LCD
 *            - Initiate EEPROM
 *            - Initiate UART
 *            - Set Motors pin as OUTPUT pins
 ************************************************************************************/

void Init_Fun(void) {
	SREG |= (1 << 7);
	LCD_init();
	UART_init();
	EEPROM_init();
	SET_BIT(MOTOR_DDRX, MOTOR_PIN_1);
	SET_BIT(MOTOR_DDRX, MOTOR_PIN_2);
}

/*****************************************************************************************/

/************************************************************************************
 * Function Name: ReceivePassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Read Password From the UART and Write it in a Structure member
 ************************************************************************************/

void RecievePassword(Password_ConfigType * Password_Config) {

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		Password_Config->PW[Counter] = UART_recieveByte();

	}

}
/*************************************************************************************/

/************************************************************************************
 * Function Name: ShowPassword
 * Parameters (in): Password_ConfigType * Password_Config
 * Return value: void
 * Description: Wrtie The password from the structure and Display it on LCD
 ************************************************************************************/

void ShowPassword(Password_ConfigType * Password_Config) {
	LCD_clearScreen();
	LCD_goToRowColumn(0, 0);
	LCD_displayString("Password is");
	LCD_goToRowColumn(1, 0);
	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		LCD_intgerToString(Password_Config->PW[Counter]);

	}

	/************************************************************************************
	 * Function Name: SavePassword_EEPROM
	 * Parameters (in): Password_ConfigType * Password_Config
	 * Return value: void
	 * Description: Save Password in The EEPROM
	 ************************************************************************************/
}

void SavePassword_EEPROM(Password_ConfigType * Password_Config) {

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {

		EEPROM_writeByte(Begining_Address + Counter,
				Password_Config->PW[Counter]);
		_delay_ms(10);
	}

}




/************************************************************************************
 * Function Name: Show_EEPROM
 * Parameters (in): uint16 Address
 * Return value: void
 * Description: Show Password in The EEPROM
 ************************************************************************************/


/*
 void Show_EEPROM(uint16 Address) {
 uint8 Value;

 LCD_clearScreen();
 LCD_displayString("PW in EEPROM:");
 LCD_goToRowColumn(1, 0);

 for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++){
 EEPROM_readByte(Address + Counter ,&Value);
 LCD_intgerToString(Value);
 _delay_ms(10);

 }

 }*/


/************************************************************************************
 * Function Name: ReadPassword_EEPROM
 * Parameters (in): uint16 Address, Password_EEPROM_ConfigType * Password_Config
 * Return value: void
 * Description: Read Password from The EEPROM
 ************************************************************************************/

void ReadPassword_EEPROM(uint16 Address,
		Password_EEPROM_ConfigType * Password_Config) {
	uint8 Value;
	LCD_clearScreen();
	LCD_displayString("PW in EEPROM:");
	LCD_goToRowColumn(1, 0);

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		EEPROM_readByte(Address + Counter, &Value);
		Password_Config->PW_EEPROM[Counter] = Value;
		LCD_intgerToString(Password_Config->PW_EEPROM[Counter]);
		_delay_ms(10);

	}

}

/************************************************************************************
 * Function Name: CheckPassword
 * Parameters (in): Password_EEPROM_ConfigType * Password_Config
 * Return value: uint8
 * Description: Check Password from The EEPROM and the entered one from the user
 ************************************************************************************/

uint8 CheckPassword(Password_EEPROM_ConfigType * Password_Config) {

	uint8 state = TRUE;

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {

		if (Password_Config->Entered_PW[Counter]
				!= Password_Config->PW_EEPROM[Counter]) {

			state = FALSE;
		} else {
			/*No Code*/
		}

	}

	return state;
}
/************************************************************************************
 * Function Name: CheckPassword
 * Parameters (in): Password_EEPROM_ConfigType * Password_Config
 * Return value: void
 * Description: Enter Password to The EEPROM from The UART
 ************************************************************************************/
void Entered_PW_ECU2(Password_EEPROM_ConfigType * Password_Config) {

	for (uint8 Counter = 0; Counter < PASSWORD_SIZE; Counter++) {
		Password_Config->Entered_PW[Counter] = UART_recieveByte();

	}
}

/************************************************************************************
 * Function Name: Recieve_Option_Select
 * Parameters (in): void * Password_Config
 * Return value: uint8
 * Description: Recieve uint8 data by UART
 ************************************************************************************/
uint8 Recieve_Option_Select(void) {

	return UART_recieveByte();

}

/************************************************************************************
 * Function Name: Control_door
 * Parameters (in): uint8 open_time, uint8 wait_time
 * Return value: void
 * Description: It Open the door till certain time (open_time) and then Wait
 * for (wait_time) and then close door again with the same value of its open value
 *
 ************************************************************************************/
void Control_door(uint8 open_time, uint8 wait_time) {

	Count_sec(0);

	SET_BIT(MOTOR_PORTX, MOTOR_PIN_1);
	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_2);
	Count_sec(open_time);

	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_1);
	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_2);
	Count_sec(wait_time);

	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_1);
	SET_BIT(MOTOR_PORTX, MOTOR_PIN_2);
	Count_sec(open_time);

	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_1);
	CLEAR_BIT(MOTOR_PORTX, MOTOR_PIN_2);

}

