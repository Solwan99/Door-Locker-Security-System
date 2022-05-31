/*-------------------------------------------------------------------
 * [Module Name]: TIMER
 *
 * [FILE NAME]: timer.h
 *
 * [AUTHOR(S)]:	Solwan Shokry
 *
 * [DESCRIPTION]: Header File for Timer Module driver of ATmega16 MCU
 ---------------------------------------------------------------------*/


#ifndef TIMER_H_
#define TIMER_H_


/*-------------------------Includes-------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
/*-------------------------Definitions And Configurations-------------------------*/


/*============================Timer0 Registers Definitions========================*/

#define Timer_Counter0_Control_Register 							TCCR0

/*Control Register Distinguished bits definitions */
#define Timer_Counter0_Force_Output_Compare_Bit0 					FOC0	/*7*/
#define Timer_Counter0_Waveform_Generation_Mode_Bit0 				WGM00	/*6*/
#define Timer_Counter0_Compare_Match_Output_Mode_Bit1 				COM01	/*5*/
#define Timer_Counter0_Compare_Match_Output_Mode_Bit0 				COM00	/*4*/
#define Timer_Counter0_Waveform_Generation_Mode_Bit1 				WGM01	/*3*/
#define Timer_Counter0_Clock_Select_Bit2 							CS02	/*2*/
#define Timer_Counter0_Clock_Select_Bit1 							CS01	/*1*/
#define Timer_Counter0_Clock_Select_Bit0 							CS00	/*0*/

#define Timer_Counter0_Value_Register  								TCNT0
#define Timer_Counter0_Output_Compare_Register  					OCR0

#define Timer_Counter0_Interrupt_Mask_Register  					TIMSK

/*Interrupt Mask Register Distinguished bits definitions */
#define Timer_Counter0_Output_Compare_Match_Interrupt_Enable_Bit	OCIE0	/*1*/
#define Timer_Counter0_Overflow_Interrupt_Enable_Bit				TOIE0	/*0*/


#define Timer_Counter0_Interrupt_Flag_Register 						TIFR

/*Interrupt Flag Register Distinguished bits definitions */
#define Timer_Counter0_Output_Compare_Interrupt_Flag 				OCF0	/*1*/
#define Timer_Counter0_Overflow_Interrupt_Flag 						TOV0	/*0*/

#define Timer_Counter0_1_Special_Function_IO_Register  				SFIOR

/*Special Function IO Register Distinguished bits definitions */
#define Timer_Counter0_1_Prescaler_Reset_Bit						PSR10	/*0*/



/*============================Timer1 Registers Definitions========================*/

#define Timer_Counter1_Control_Register_A 							TCCR1A

/*Control Register Channel A Distinguished bits definitions */
#define Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1 	COM1A1	/*7*/
#define Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0 	COM1A0	/*6*/
#define Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1 	COM1B1	/*5*/
#define Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0 	COM1B0	/*4*/
#define Timer_Counter1_Force_Output_Compare_Channel_A 				FOC1A	/*3*/
#define Timer_Counter1_Force_Output_Compare_Channel_B 				FOC1B	/*2*/
#define Timer_Counter1_Waveform_Generation_Mode_Bit1 				WGM11	/*1*/
#define Timer_Counter1_Waveform_Generation_Mode_Bit0 				WGM10	/*0*/


#define Timer_Counter1_Control_Register_B 							TCCR1B

/*Control Register Channel B Distinguished bits definitions */
#define Timer_Counter1_Input_Capture_Noise_Canceler_Bit				ICNC1	/*7*/
#define Timer_Counter1_Input_Capture_Edge_Select_Bit				ICES1	/*6*/
#define Timer_Counter1_Waveform_Generation_Mode_Bit3 				WGM13	/*4*/
#define Timer_Counter1_Waveform_Generation_Mode_Bit2 				WGM12	/*3*/
#define Timer_Counter1_Clock_Select_Bit2 							CS12	/*2*/
#define Timer_Counter1_Clock_Select_Bit1 							CS11	/*1*/
#define Timer_Counter1_Clock_Select_Bit0 							CS10	/*0*/

#define Timer_Counter1_Value_Register		  						TCNT1
#define Timer_Counter1_Value_Register_High  						TCNT1H	/*MSB Value Register	Byte1*/
#define Timer_Counter1_Value_Register_Low  							TCNT1L	/*LSB Value Register	Byte0*/

#define Timer_Counter1_Output_Compare_Register_Channel_A			OCR1A
#define Timer_Counter1_Output_Compare_Register_Channel_A_High		OCR1AH
#define Timer_Counter1_Output_Compare_Register_Channel_A_Low		OCR1AL

#define Timer_Counter1_Output_Compare_Register_Channel_B			OCR1B
#define Timer_Counter1_Output_Compare_Register_Channel_B_High		OCR1BH
#define Timer_Counter1_Output_Compare_Register_Channel_B_Low		OCR1BL

#define Timer_Counter1_Input_Capture_Register_High					ICR1H
#define Timer_Counter1_Input_Capture_Register_Low					ICR1L

#define Timer_Counter1_Interrupt_Mask_Register  					TIMSK

/*Interrupt Mask Register Distinguished bits definitions */
#define Timer_Counter1_Input_Capture_Interrupt_Enable_Bit			TICIE1	/*5*/
#define Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit	OCIE1A 	/*4*/
#define Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit	OCIE1B 	/*3*/
#define Timer_Counter1_Over_Flow_Interrupt_Enable_Bit				TOIE1	/*2*/


#define Timer_Counter1_Interrupt_Flag_Register 						TIFR

/*Interrupt Flag Register Distinguished bits definitions */
#define Timer_Counter1_Input_Capture_Flag							ICF1	/*5*/
#define Timer_Counter1_Output_Compare_A_Match_Interrupt_Flag		OCF1A	/*4*/
#define Timer_Counter1_Output_Compare_B_Match_Interrupt_Flag		OCF1B	/*3*/
#define Timer_Counter1_Over_Flow_Interrupt_Flag						TOV1	/*2*/


/*============================Timer2 Registers Definitions========================*/

#define Timer_Counter2_Control_Register 							TCCR2

/*Control Register Distinguished bits definitions */
#define Timer_Counter2_Force_Output_Compare_Bit0 					FOC2	/*7*/
#define Timer_Counter2_Waveform_Generation_Mode_Bit0 				WGM20	/*6*/
#define Timer_Counter2_Compare_Match_Output_Mode_Bit1 				COM21	/*5*/
#define Timer_Counter2_Compare_Match_Output_Mode_Bit0 				COM20	/*4*/
#define Timer_Counter2_Waveform_Generation_Mode_Bit1 				WGM21	/*3*/
#define Timer_Counter2_Clock_Select_Bit2 							CS22	/*2*/
#define Timer_Counter2_Clock_Select_Bit1 							CS21	/*1*/
#define Timer_Counter2_Clock_Select_Bit0 							CS20	/*0*/

#define Timer_Counter2_Value_Register  								TCNT2
#define Timer_Counter2_Output_Compare_Register  					OCR2

#define Timer_Counter2_Asynchronous_Status_Register 				ASSR

/*Asynchronous Status Register Distinguished bits definitions*/
#define Timer_Counter2_Asynchronous_Mode_Flag						AS2		/*3*/
#define Timer_Counter2_Update_Busy_Flag								TCN2UB	/*2*/
#define Timer_Counter2_Output_Compare_Register_Update_Busy_Flag		OCR2UB	/*1*/
#define Timer_Counter2_Output_Control_Register_Update_Busy_Flag		TCR2UB	/*0*/


#define Timer_Counter2_Interrupt_Mask_Register  					TIMSK

/*Interrupt Mask Register Distinguished bits definitions */
#define Timer_Counter2_Output_Compare_Match_Interrupt_Enable_Bit	OCIE2	/*7*/
#define Timer_Counter2_Overflow_Interrupt_Enable_Bit				TOIE2	/*6*/


#define Timer_Counter2_Interrupt_Flag_Register 						TIFR

/*Interrupt Flag Register Distinguished bits definitions */
#define Timer_Counter2_Output_Compare_Interrupt_Flag 				OCF2	/*7*/
#define Timer_Counter2_Overflow_Interrupt_Flag 						TOV2	/*6*/

/*Special Function IO Register Distinguished bits definitions */
#define Timer_Counter2_Prescaler_Reset_Bit							PSR12	/*1*/

/*Definition of NULL value to be assigned to the NULL Pointer as a pointer to void with a value equivalent to ZERO */
#define NULL ( (void*) (0) )



/*-------------------------Extern Module shared global variables-------------------------*/


extern volatile uint8 g_TIMER_VALUE;

/*-------------------------ENUMS-------------------------*/


/*Enumeration user Type defined to distinguish The Timer module to deal with during run time from the three supported timers*/
typedef enum{
	Timer0,Timer1,Timer2
}Timer_Number;

typedef enum{
	TimerNormal,phaseCorrect_PWM,CTC,fast_PWM,phaseFrequencyCorrect_PWM
}Timer_OperationMode;

typedef enum{
	CTC_Normal,Toggle_CM,Clear_CM,Set_CM,CTC_ICR
}CTC_Mode;

typedef enum{
	PWM_Normal,Reserved,NonInverting,Inverting
}PWM_Mode;

typedef enum{
	_8bits,_9bits,_10bits
}PWM_Mode_Resolution;

typedef enum{
	OCR,ICR
}Compare_Register;

typedef enum{
	A,B
}Timer1_Channel_Number;

typedef enum{
	NoClock,FCPU,FCPU_8,FCPU_64,FCPU_256,FCPU_1024,External_falling,External_rising
}Timer_ClockSelect;


/*-------------------------Structures and unions-------------------------*/


/*-------------------------------------------------------------------
 * [Structure NAME]: Timer_Configuration_Struct
 *
 * [Structure DESCRIPTION]:	Structure to configure the Timer Module Dynamically
 *
 * [Members]:
 * 				TimerNumber: Enum to configure the Timer module number to operate on during run-time.
 * 				TimerMode: Enum to configure the Timer operation Mode.
 * 				Timer_CTC_Mode: Enum to configure the Timer CTC operation Mode.
 * 				Timer_PWM_Mode: Enum to configure the Timer PWM operation Mode.
 * 				Timer_Clock_Mode: Enum to configure the Timer Clock Pre-scaler selection Mode.
 ---------------------------------------------------------------------*/

typedef struct{

	Timer_Number TimerNumber;
	Timer_OperationMode TimerMode;
	CTC_Mode Timer_CTC_Mode;
	PWM_Mode Timer_PWM_Mode;
	PWM_Mode_Resolution Timer1_PWM_Resolution;
	Compare_Register Timer1_Compare_Register;
	Timer1_Channel_Number Channel_Number;
	Timer_ClockSelect Timer_Clock_Mode;
	uint16 Timer_InitialCountValue;
	uint16 Timer_CompareValue;
	uint16 Timer_PWM_DutyCycle;
}Timer_Configuration_Struct;




/*-------------------------FUNCTION-LIKE MACROS-------------------------*/

/*-------------------------Function Declarations-------------------------*/

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
void TIMER_Init(const Timer_Configuration_Struct* Timer_Config_ptr );


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
void TIMER_SetISR_CallBack( void(*Timer_CallBackFunc)(void) , Timer_Number TimerNumber);


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
void TIMER_StopCounting(Timer_Number TimerNumber);

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
void TIMER_DeInit(Timer_Number TimerNumber);





#endif /* TIMER_H_ */
