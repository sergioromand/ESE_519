//ESE 519 Lab 2 Part 1.2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

void timer0_init()
{
	TIMSK0	|= 0x02;			// enable Timer/Counter Overflow Interrupt  (...01)
	TCCR0A	|= 0x40;			// toggle on compare match  (01......) bit 6 set
	TCCR0A  &= 0x7F;            // toggle on compare match  (01......) bit 7 set
	TCCR0A &= 0xF8;             //set to normal mode
	TCCR0B	|= 0x04;			// set prescaler to 1:256 for timer channel 0 (.....011)
	TCCR0B	&= 0xFC;			// set prescaler to 1:256
	TCNT0	= 0;				// initialize counter
	OCR0A = 71;                 //set timer compare
	sei();
}


ISR (TIMER0_COMPA_vect)
{
	OCR0A += 71;				//update compare match
}

int main(void)
{
	
	DDRD	|= 0x40;			// port D pin 6 = output (bit 6)
	
	timer0_init();				// initialize timer
	while (1) 
    {
		
    }
}