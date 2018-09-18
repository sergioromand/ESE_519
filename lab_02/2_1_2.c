//ESE 519 Lab 2 Part 1.2

/*
 * 
 * 16MHz / 64 prescalar --> 250KHz --> T = .004ms
 * Time to overflow 8bit clock = T * 256 = 1.024 ms
 *  
 * 440Hz --> T = 2.272727ms --> half_period = 1.1363636ms
 *  
 * after one overflow we need .1123636ms more to toggle
 * .1123636ms / .004ms = 28 more ticks
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

volatile uint8_t total_overflows;

void timer0_init()
{
	TIMSK0	|= 0x01;			// enable Timer/Counter Overflow Interrupt  (...01)
	TCCR0A	|= 0x40;			// toggle on compare match  (01......) bit 6 set
	TCCR0B	|= 0x03;			// set prescaler to 1:64 for timer channel 0 (.....011)
	TCNT0	= 0;				// initialize counter
	TIMSK0 &= 0xFD;				//  kill T/C Compare Interrupt

	sei();
}

//*********************

ISR (TIMER0_OVF_vect)
{
	total_overflows = 1;		// count 1 overflow
}

//*********************
ISR (TIMER0_COMPA_vect)
{
	  total_overflows = 0;		// reset overflow to 0;
	  TCNT0 = 0;				// restart timer counter
	  TIFR1 |= 0x20;			// clear T/C Overflow Interrupt Flag
}

//*********************
//*********************
int main(void)
{
	
	DDRD	|= 0x40;			// port D pin 6 = output (bit 6)
	
	timer0_init();				// initialize timer
	
	OCR0A = 28;
	
	while (1) 
    {
		TIMSK0 &= 0xFD;			//  kill T/C Compare Interrupt
		
		if (total_overflows >= 1)
		{
			TIMSK0 |= 0x02;		// enable Timer/Counter Compare Interrupt (...10);	
		}
		
		while(1);
    }
}