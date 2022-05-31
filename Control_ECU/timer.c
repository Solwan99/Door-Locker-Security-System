
#include "timer.h"

uint8 g_tick;
uint8 g_flag = 0;
uint8 g_sec ;

void timer0_init_CTC_mode(void) {
	SREG  |= (1<<7);
	TCNT0 = 0; //timer initial value
	OCR0 = 250; //compare value
	//enable compare interrupt
	/* Configure timer0 control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. No need for OC0 in this example so COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	SET_BIT(TIMSK, OCIE0);
	SET_BIT(TCCR0, FOC0);
	SET_BIT(TCCR0, WGM01);
	SET_BIT(TCCR0, CS02);
	SET_BIT(TCCR0, CS00);
}

ISR(TIMER0_COMP_vect) {
	g_tick++;
	if(g_tick == TICKS_TO_COUNT_SEC){
		g_sec ++ ;
		g_tick =0 ;
	}
}

void Count_sec(uint8 no_of_sec) {

	timer0_init_CTC_mode();
	while (g_sec < no_of_sec){

	}
	g_sec =0 ;
}



