//ESE 519 Lab 2 Part 0.2 

#include <avr/io.h>
#include "util/delay.h"

int main(void) 
{
	
	//make PB0 an input, PB 5 is an output
	DDRB = 0x20;
	//turn on PB0 
	PORTB = 0x01;
	while (1) 
	{
		if(!(PINB &= 0x01)) {
			PORTB |= 0x20;
		}
		else {
			PORTB &= 0xDF;
		}
		PORTB |= 0x01;
	}
}