// ESE 519 Lab 2 Part 2.1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr/delay.h"


void timer0_init()
{
	TCCR0A |= 0x2;				//CTC Mode
	TCCR0A &= 0xFE;
	TCCR0B &= 0xF7;

	TCCR0A	|= 0x40;			// toggle on compare match  (01......) bit 6 set
	TCCR0A  &= 0x7F;            // toggle on compare match  (01......) bit 7 set
	TCCR0B	|= 0x04;			// set prescaler to 1:256 for timer channel 0 (.....011)
	TCCR0B	&= 0xFC;			// set prescaler to 1:256
	OCR0A = 70;
	
	TCNT0	= 0;				// initialize counter
}

int main(void)
{
	DDRD	|= 0x40;			// port D pin 6 = output (bit 6)
	timer0_init();				// initialize timer
	
	while (1);
	
}