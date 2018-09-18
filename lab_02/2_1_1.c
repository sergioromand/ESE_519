//ESE 519 Lab 2 Part 1.1

#include <avr/interrupt.h> 
#include <avr/io.h> 
#include <stdio.h> 
#include <stdlib.h> 



ISR(TIMER1_CAPT_vect) {
	TIFR1 |= 0x20; //clear the flag
	if(!(PINB & 0x01)) {
		//LOW- Now we want a rising edge and to write a high to PB1
		PORTB |= 0x20;
		TCCR1B |= 0x40;
	}
	else {
		//High- Now we want a rising edge and to write a low to PB0
		PORTB &= 0xDF;
		TCCR1B &= 0xBF;
	}
}

int main(void) {
	DDRB |= 0x20; //this makes PB1 output
	DDRB &= 0xFE; //make PB0 input
	PORTB |= 0x01; //write a high to Port B
	TIMSK1 |= 0x20; //enable the interrupt on ICP1 (PB0)
	TCCR1B |= 0x01; //enable the clock, no prescale needed
	TCCR1B &= 0xF9; //make bits 1 and 2 == 0
	TCCR1B &= 0xBF; //the first input capture we care about is a falling edge
	TCCR1B |= 0x80; //noise cancellation
	
	sei();

	while(1);
}