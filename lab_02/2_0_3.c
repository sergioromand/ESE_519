//ESE 519 Lab 2 Part 0.3

#include <avr/interrupt.h> 
#include <avr/io.h> 
#include <stdio.h> 
#include <stdlib.h> 


unsigned int half_period=1000;

ISR(TIMER0_COMPA_VECT) {
	OCR0A += half_period;
}

int main(void) {
	DDRD |= 0x40;
	TCCR0A |= 0x40;
	TCCR0B |= 0x01;
	TIMSK0 = 0x01;

	OCR0A += TCNT0 + half_period;
	sei();

	while(1) {

	}
}