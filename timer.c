/*-------------------------------------------------------------------
 * [FILE NAME]:
 *
 * [AUTHOR(S)]:
 *
 * [DATE CREATED]:
 *
 * [DESCRIPTION]:
 ---------------------------------------------------------------------*/


/*-------------------------Includes-------------------------*/
#include "timer.h"
/*-------------------------Global Variables-------------------------*/
static  void(*g_Timer0_CallBack_ptr)(void)=NULL;
static  void(*g_Timer1_CallBack_ptr)(void)=NULL;
static  void(*g_Timer2_CallBack_ptr)(void)=NULL;

/*-------------------------Function Definitions-------------------------*/
void TIMER_Init(const Timer_Configuration_Struct* Timer_Config_ptr )
{
	/*Configuring The Timer According to the selected Timer whether it's working with:
	 * -	8-bits Timer-Counter0
	 * -	16-bits Timer_Counter1
	 * - 	8-bits Timer-Counter2
	 * */
	switch(Timer_Config_ptr->TimerNumber)
	{
		/*If working with the 8-bits Timer-Counter 0*/
		case Timer0:
			/*Clear all bits of the control register*/
			Timer_Counter0_Control_Register = 0x00 ;

			/*Initialize the counter value register with the start of count value*/
			Timer_Counter0_Value_Register = ( (Timer_Config_ptr->Timer_InitialCountValue) & 0xFF);

			/*Configuring The Timer According to the selected mode of operation whether it's working in:
			 * -Normal Mode
			 * -CTC Mode
			 * -PWM (Phase correct & fast PWM )
			 * */
			switch(Timer_Config_ptr->TimerMode)
			{
				case TimerNormal:
					/*Configure Wave Generation Mode Selection Bits To work in Over flow mode:
					 * WGM01=0			WGM00=0*/
					CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit0);
					CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit1);

					/*ٍSet The force output compare bit FOC0 to work in overflow mode*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Force_Output_Compare_Bit0);

					/*Set the Timer overflow enable TOIE0 bit in the interrupt mask register TIMSK */
					SET_BIT(Timer_Counter0_Interrupt_Mask_Register,Timer_Counter0_Overflow_Interrupt_Enable_Bit);
					break;

				case phaseCorrect_PWM:
					/*Configure Wave Generation Mode Selection Bits To work in phase correct PWM mode:
					 * WGM01=0			WGM00=1*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit0);

					/*ٍSet The force output compare bit FOC0 to work in overflow mode*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Force_Output_Compare_Bit0);

					/*Set the Duty Cycle Value in 8-bit Timer-Counter Output Compare Register*/
					Timer_Counter0_Output_Compare_Register = (Timer_Config_ptr->Timer_PWM_DutyCycle) & 0xFF;

					/*Configure Compare Output Mode Selection Bits To work in Phase Correct PWM mode:
					 *Normal Mode:
					 *COM01=0			COM00=0
					 *Non Inverting Mode:
					 *COM01=1			COM00=0
					 *Inverting Mode:
					 *COM01=1			COM00=1*/
					if( (Timer_Config_ptr->Timer_PWM_Mode) == PWM_Normal)
						{
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}
					else if( (Timer_Config_ptr->Timer_PWM_Mode) == NonInverting)
						{
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}
					else if( (Timer_Config_ptr->Timer_PWM_Mode) == Inverting)
						{
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}

					/*Set the Timer output compare match interrupt enable TOIE0 bit in the interrupt mask register TIMSK */
					SET_BIT(Timer_Counter0_Interrupt_Mask_Register,Timer_Counter0_Output_Compare_Interrupt_Flag);
					break;

				case CTC:
					/*Configure Wave Generation Mode Selection Bits To work in Fast PWM mode:
					 * WGM01=1			WGM00=0*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit1);

					/*Set the Output compare Value in 8-bit Timer-Counter Output Compare Register*/
					Timer_Counter0_Output_Compare_Register = (Timer_Config_ptr->Timer_CompareValue) & 0xFF;

					/*ٍSet The force output compare bit FOC0 to work in overflow mode*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Force_Output_Compare_Bit0);

					/*Configure Compare Output Mode Selection Bits To work in  CTC mode:
					 *Normal Mode:
					 *COM01=0			COM00=0
					 *Toggle On compare match Mode:
					 *COM01=0			COM00=1
					 *Clear On compare match Mode:
					 *COM01=1			COM00=0
					 *Set On compare match Mode:
					 *COM01=1			COM00=1*/
					if((Timer_Config_ptr->Timer_CTC_Mode) == CTC_Normal)
					{
						CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
						CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
					}
					else if((Timer_Config_ptr->Timer_CTC_Mode) == Toggle_CM)
					{
						SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
						CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
					}
					else if((Timer_Config_ptr->Timer_CTC_Mode) == Clear_CM)
					{
						CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
						SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
					}
					else if((Timer_Config_ptr->Timer_CTC_Mode) ==Set_CM)
					{
						SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
						SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
					}

					/*Set the Timer output compare match interrupt enable TOIE0 bit in the interrupt mask register TIMSK */
					SET_BIT(Timer_Counter0_Interrupt_Mask_Register,Timer_Counter0_Output_Compare_Interrupt_Flag);
					break;

				case fast_PWM:

					/*Configure Wave Generation Mode Selection Bits To work in Fast PWM mode:
					 * WGM01=1			WGM00=1*/
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit0);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Waveform_Generation_Mode_Bit1);

					/*Set the Duty Cycle Value in 8-bit Timer-Counter Output Compare Register*/
					Timer_Counter0_Output_Compare_Register = (Timer_Config_ptr->Timer_PWM_DutyCycle) & 0xFF;

					/*Configure Compare Output Mode Selection Bits To work in Fast PWM mode:
					 *Normal Mode:
					 *COM01=0			COM00=0
					 *Non Inverting Mode:
					 *COM01=1			COM00=0
					 *Inverting Mode:
					 *COM01=1			COM00=1*/
					if( (Timer_Config_ptr->Timer_PWM_Mode) == PWM_Normal)
						{
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}
					else if( (Timer_Config_ptr->Timer_PWM_Mode) == NonInverting)
						{
							CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}
					else if( (Timer_Config_ptr->Timer_PWM_Mode) == Inverting)
						{
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit0);
							SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Compare_Match_Output_Mode_Bit1);
						}
					break;
				default:
					break;
			}

			switch(Timer_Config_ptr->Timer_Clock_Mode)
			{
				/*Case No clock :
				 * CS00=0		CS01=0 		CS02=0*/
				case NoClock:
					CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
					CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
					break;

				/*Case F_CPU :
				 * CS02=0		CS01=0 		CS00=0*/
				case FCPU:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					break;

				/*Case F_CPU/8 :
				 * CS02=0		CS01=1 		CS00=0*/
				case FCPU_8:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
					break;

				/*Case F_CPU/64 :
				 * CS02=0		CS01=1 		CS00=1*/
				case FCPU_64:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
					break;

				/*Case F_CPU/256 :
				 * CS02=1		CS01=0 		CS00=0*/
				case FCPU_256:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
					break;

				/*Case F_CPU/1024 :
				 * CS02=1		CS01=0 		CS00=1*/
				case FCPU_1024:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
					break;

				/*Case External Clock Source, Clock on falling edge :
				 * CS02=1		CS01=1 		CS00=0*/
				case External_falling:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
					break;

				/*Case External Clock Source, Clock on rising edge :
				* CS02=1		CS01=1 		CS00=1*/
				case External_rising:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
					break;

				/*Default Clock Selection Option: F_CPU*/
				default:
					SET_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
					break;
			}
			break;

			case Timer1:
			/*Initially clear all bits of the Timer-Counter control registers*/
			Timer_Counter1_Control_Register_A= 0x00;
			Timer_Counter1_Control_Register_B= 0X00;

			/*Store the Timer Counter's 16-bit initial value in it's corresponding registers*/
			Timer_Counter1_Value_Register = ( Timer_Counter1_Value_Register & 0x0000 ) | ( Timer_Config_ptr->Timer_InitialCountValue );

			if(Timer_Config_ptr->Channel_Number == A)
			{
				Timer_Counter1_Output_Compare_Register_Channel_A=Timer_Config_ptr->Timer_PWM_DutyCycle;
			}
			else if(Timer_Config_ptr->Channel_Number == B)
			{
				Timer_Counter1_Output_Compare_Register_Channel_B=Timer_Config_ptr->Timer_PWM_DutyCycle;
			}

			switch(Timer_Config_ptr->TimerMode)
			{
				case TimerNormal:
					/*Configure the timer-counter to work in overflow mode:

					 *Control Register A:
					 * COM1A1=0 	COM1A0=0	COM1B1=0	COM1B0=0	WGM11=0 	WGM10=0 	FOC1A=1		FOC1B=1*/
					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
					SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
					SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
					CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);


					/* Control Register B:
					 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
					CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
					CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
					CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
					CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);


					/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
					SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Over_Flow_Interrupt_Enable_Bit);

					switch(Timer_Config_ptr->Timer_Clock_Mode)
					{
						/*Case No clock :
						 * CS00=0		CS01=0 		CS02=0*/
						case NoClock:
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
							break;

						/*Case F_CPU :
						 * CS02=0		CS01=0 		CS00=1*/
						case FCPU:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							break;

						/*Case F_CPU/8 :
						 * CS02=0		CS01=1 		CS00=0*/
						case FCPU_8:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
							break;

						/*Case F_CPU/64 :
						 * CS02=0		CS01=1 		CS00=1*/
						case FCPU_64:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
							break;

						/*Case F_CPU/256 :
						 * CS02=1		CS01=0 		CS00=0*/
						case FCPU_256:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
							break;

						/*Case F_CPU/1024 :
						 * CS02=1		CS01=0 		CS00=1*/
						case FCPU_1024:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
							break;

						/*Case External Clock Source, Clock on falling edge :
						 * CS02=1		CS01=1 		CS00=0*/
						case External_falling:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
							break;

						/*Case External Clock Source, Clock on rising edge :
						* CS02=1		CS01=1 		CS00=1*/
						case External_rising:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
							break;

						/*Default Clock Selection Option: F_CPU*/
						default:
							SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
							break;
					}
					break;

				case phaseCorrect_PWM:
					switch(Timer_Config_ptr->Timer1_PWM_Resolution)
					{
					case _8bits:
						if( (Timer_Config_ptr->Timer_PWM_Mode) == PWM_Normal)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == NonInverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);

						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == Inverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						switch(Timer_Config_ptr->Timer_Clock_Mode)
						{
							/*Case No clock :
							 * CS00=0		CS01=0 		CS02=0*/
							case NoClock:
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU :
							 * CS02=0		CS01=0 		CS00=1*/
							case FCPU:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;

							/*Case F_CPU/8 :
							 * CS02=0		CS01=1 		CS00=0*/
							case FCPU_8:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/64 :
							 * CS02=0		CS01=1 		CS00=1*/
							case FCPU_64:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/256 :
							 * CS02=1		CS01=0 		CS00=0*/
							case FCPU_256:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU/1024 :
							 * CS02=1		CS01=0 		CS00=1*/
							case FCPU_1024:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on falling edge :
							 * CS02=1		CS01=1 		CS00=0*/
							case External_falling:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on rising edge :
							* CS02=1		CS01=1 		CS00=1*/
							case External_rising:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Default Clock Selection Option: F_CPU*/
							default:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;
						}
						break;

					case _9bits:
						if( (Timer_Config_ptr->Timer_PWM_Mode) == PWM_Normal)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == NonInverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);

						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == Inverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						switch(Timer_Config_ptr->Timer_Clock_Mode)
						{
							/*Case No clock :
							 * CS00=0		CS01=0 		CS02=0*/
							case NoClock:
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU :
							 * CS02=0		CS01=0 		CS00=1*/
							case FCPU:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;

							/*Case F_CPU/8 :
							 * CS02=0		CS01=1 		CS00=0*/
							case FCPU_8:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/64 :
							 * CS02=0		CS01=1 		CS00=1*/
							case FCPU_64:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/256 :
							 * CS02=1		CS01=0 		CS00=0*/
							case FCPU_256:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU/1024 :
							 * CS02=1		CS01=0 		CS00=1*/
							case FCPU_1024:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on falling edge :
							 * CS02=1		CS01=1 		CS00=0*/
							case External_falling:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on rising edge :
							* CS02=1		CS01=1 		CS00=1*/
							case External_rising:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Default Clock Selection Option: F_CPU*/
							default:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;
						}
						break;

					case _10bits:
						if( (Timer_Config_ptr->Timer_PWM_Mode) == PWM_Normal)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == NonInverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							CLEAR_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);

						}
						else if( (Timer_Config_ptr->Timer_PWM_Mode) == Inverting)
						{
							if(Timer_Config_ptr->Channel_Number == A)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_A_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_A);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_A_Match_Interrupt_Enable_Bit);
							}
							else if(Timer_Config_ptr->Channel_Number == B)
							{
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Compare_Match_Output_Mode_Channel_B_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Force_Output_Compare_Channel_B);

								/*Enable the Timer-Counter OverFlow Interrupt by setting the corresponding bit in timer interrupt mask register*/
								SET_BIT(Timer_Counter1_Interrupt_Mask_Register ,Timer_Counter1_Output_Compare_B_Match_Interrupt_Enable_Bit);
							}

							/* Control Register B:
							 * ICNC1=0		ICES1=0		WGM13=0 	WGM12=0*/
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Noise_Canceler_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Input_Capture_Edge_Select_Bit);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit3);
							CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Waveform_Generation_Mode_Bit2);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit1);
							SET_BIT(Timer_Counter1_Control_Register_A,Timer_Counter1_Waveform_Generation_Mode_Bit0);
						}
						switch(Timer_Config_ptr->Timer_Clock_Mode)
						{
							/*Case No clock :
							 * CS00=0		CS01=0 		CS02=0*/
							case NoClock:
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU :
							 * CS02=0		CS01=0 		CS00=1*/
							case FCPU:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;

							/*Case F_CPU/8 :
							 * CS02=0		CS01=1 		CS00=0*/
							case FCPU_8:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/64 :
							 * CS02=0		CS01=1 		CS00=1*/
							case FCPU_64:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								break;

							/*Case F_CPU/256 :
							 * CS02=1		CS01=0 		CS00=0*/
							case FCPU_256:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case F_CPU/1024 :
							 * CS02=1		CS01=0 		CS00=1*/
							case FCPU_1024:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on falling edge :
							 * CS02=1		CS01=1 		CS00=0*/
							case External_falling:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Case External Clock Source, Clock on rising edge :
							* CS02=1		CS01=1 		CS00=1*/
							case External_rising:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
								break;

							/*Default Clock Selection Option: F_CPU*/
							default:
								SET_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
								break;
						}
						break;
					}
					break;
				case CTC:
					break;
				case fast_PWM:
					break;
				case phaseFrequencyCorrect_PWM:
					break;
				default:
					break;
			}

			break;
		case Timer2:
			break;
	}
}
/*==========================Interrupt Service Routines========================*/


/*========================================================================================
 * 								Timer 0
 * =======================================================================================*/
ISR(TIMER0_OVF_vect){

	if (g_Timer0_CallBack_ptr != NULL)
	{
		(*g_Timer0_CallBack_ptr)();
	}
}



ISR(TIMER0_COMP_vect){
	if (g_Timer0_CallBack_ptr != NULL)
	{
		(*g_Timer0_CallBack_ptr)();
	}

}



/*========================================================================================
 * 										Timer 1
 * =======================================================================================*/

ISR(TIMER1_OVF_vect){
	if (g_Timer1_CallBack_ptr != NULL)
	{
		(*g_Timer1_CallBack_ptr)();
	}
}

ISR(TIMER1_COMPA_vect){
	if (g_Timer1_CallBack_ptr != NULL)
	{
		(*g_Timer1_CallBack_ptr)();
	}
}

ISR(TIMER1_COMPB_vect){
	if (g_Timer1_CallBack_ptr != NULL)
	{
		(*g_Timer1_CallBack_ptr)();
	}
}



/*========================================================================================
 * 										Timer 2
 * =======================================================================================*/
ISR(TIMER2_OVF_vect){

	if (g_Timer2_CallBack_ptr != NULL)
	{
		(*g_Timer2_CallBack_ptr)();
	}
}


ISR(TIMER2_COMP_vect){

	if (g_Timer2_CallBack_ptr != NULL)
	{
		(*g_Timer2_CallBack_ptr)();
	}
}


void TIMER_SetISR_CallBack( void(*Timer_CallBackFunc)(void) , Timer_Number TimerNumber)
{
	switch(TimerNumber)
	{
	case Timer0:
		g_Timer0_CallBack_ptr = Timer_CallBackFunc;
		break;
	case Timer1:
		g_Timer1_CallBack_ptr = Timer_CallBackFunc;
		break;
	case Timer2:
		g_Timer2_CallBack_ptr = Timer_CallBackFunc;
		break;
	default:
		break;
	}
}
void TIMER_StopCounting(Timer_Number TimerNumber)
{
	switch(TimerNumber)
	{
	case Timer0:
		CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit0);
		CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit1);
		CLEAR_BIT(Timer_Counter0_Control_Register,Timer_Counter0_Clock_Select_Bit2);
		break;
	case Timer1:
		CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit0);
		CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit1);
		CLEAR_BIT(Timer_Counter1_Control_Register_B,Timer_Counter1_Clock_Select_Bit2);
		break;
	case Timer2:
		CLEAR_BIT(Timer_Counter2_Control_Register,Timer_Counter2_Clock_Select_Bit0);
		CLEAR_BIT(Timer_Counter2_Control_Register,Timer_Counter2_Clock_Select_Bit1);
		CLEAR_BIT(Timer_Counter2_Control_Register,Timer_Counter2_Clock_Select_Bit2);
		break;
	default:
		break;
	}
}


void TIMER_DeInit(Timer_Number TimerNumber)
{
	switch(TimerNumber)
	{
	case Timer0:

		/*Clear all register in Timer0*/
		Timer_Counter0_Control_Register= 0x00;
		Timer_Counter0_Value_Register= 0x00;
		Timer_Counter0_Output_Compare_Register= 0x00;
		Timer_Counter0_Interrupt_Mask_Register= 0x00;
		break;

	case Timer1:
		/*Clear all register in Timer1*/
		Timer_Counter1_Control_Register_A= 0x00;
		Timer_Counter1_Control_Register_B= 0x00;
		Timer_Counter1_Value_Register= 0x0000;
		Timer_Counter1_Output_Compare_Register_Channel_A= 0x00;
		Timer_Counter1_Output_Compare_Register_Channel_B= 0x00;
		Timer_Counter1_Interrupt_Mask_Register= 0x00;
		break;

	case Timer2:
		/*Clear all register in Timer2*/
		Timer_Counter2_Control_Register= 0x00;
		Timer_Counter2_Value_Register= 0x00;
		Timer_Counter2_Output_Compare_Register= 0x00;
		Timer_Counter2_Interrupt_Mask_Register= 0x00;
		break;
}
}
