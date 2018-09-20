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
#include "util/delay.h"


volatile uint8_t total_overflows;

void timer0_init()
{
	TIMSK0	|= 0x01;			// enable Timer/Counter Overflow Interrupt  (...01)
	TCCR0A	|= 0x40;			// toggle on compare match  (01......) bit 6 set
	TCCR0A  &= 0x7F;            // toggle on compare match  (01......) bit 7 set
	TCCR0A &= 0xF8;             //set to normal mode
	TCCR0B	|= 0x03;			// set prescaler to 1:64 for timer channel 0 (.....011)
	TCNT0	= 0;				// initialize counter
	TIMSK0 &= 0xFD;				//disable output compare isr
	OCR0A = 28;                 //set timer compare
	sei();
}

//*********************

ISR (TIMER0_OVF_vect)
{
	TIMSK0 |= 0x02;				//turn on output compare isr
	TCNT0 = 0;
	
}

//*********************
ISR (TIMER0_COMPA_vect)
{
	  TIMSK0 &= 0xFD;			//disable output compare isr
	  //TCNT0 = 0;
	  OCR0A += 28;				//update compare match
}

//*********************
//*********************
int main(void)
{
	
	DDRD	|= 0x40;			// port D pin 6 = output (bit 6)
	
	timer0_init();				// initialize timer
	while (1) 
    {
		
    }
}