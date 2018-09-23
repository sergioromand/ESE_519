//ESE 519 Lab 2 Part 2.2


#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

volatile unsigned int diff, edge1, edge2, overflows, pulseDone; 
long double num_ticks; 


ISR(TIMER1_COMPA_vect)
{ 
	if((PORTB & 0x02) == 2) {
		OCR1A = TCNT1 + 80;		//increase OCR1A by 5uS
	}
	else {
		TIMSK1 &= ~(1 << OCIE1A);             //disable output compare
		TIMSK1 |= (1 << ICIE1);						  //enable input capture
	}
}

ISR(TIMER1_CAPT_vect)
{
	if(!(TCCR1B & 0x40)) {
		//FALLING EDGE
		edge2 = ICR1;			//TimeStamp
		TCCR1B |= (1 << ICES1); //check for rising edge next time
		TIFR1 |= 0x20;			//clear flag
		pulseDone = 1;          //ready to measure
		TIMSK1 &= 0xDF;			//disable input capture
		TIMSK1	|=	0x02;		// enable output compare A interrupt
		OCR1A = TCNT1 + 3200; 	//wait 200 uS
	}
	else {
		//RISING EDGE
		edge1 = ICR1;			 //TimeStamp;
		TCCR1B &= ~(1 << ICES1);	//Now check for falling edge
		TIFR1 |= 0x20;			 //clear flag
	}
}

ISR(TIMER1_OVF_vect)
{
	overflows ++;				//increment the overflow counter
}

void timer1_init()
{
	TIMSK1	|=  0x01;			// enable overflow interrupt
	TIMSK1	|=	0x02;			// enable output compare A interrupt
	
	TCCR1B	|=	0x01;			// setup clock, no prescaling
	TCCR1B &= ~(1 << CS11);			
	TCCR1B &= ~(1 << CS12);

	TCCR1A &= ~(1 << COM1A1);
	TCCR1A |= (1 << COM1A0);    //toggle mode

	TCCR1B	|= 0x40;			// capture a rising edge 
	TCCR1B  |= 0x80; 			// enable noise canceling
	
	OCR1A	=	3200;			// 1/16MHz = .06us   --> 5us/.06us = 83.333  ---->  for sending 5us pulse
	
	TCNT1	=	0;				// initialize counter

	overflows = 0;				// new number of overflows

	pulseDone = 0;				// we haven't captured a pulse

	sei();						//enable interrupts

	
}

int main(void)
{
	DDRB	|=	0x02;			// PB1 output 
	DDRB	&=  0xFE;			// PB0 input 
	uart_init();
	timer1_init();

	
	while(1) {
		if(pulseDone) {
			if(edge2 < edge1) {
				overflows--;
				diff = edge1 - edge2;
			}else {
				diff = edge2 - edge1;
			}
			num_ticks = overflows * 65535u + (long)diff;
			printf("Pulse width: %u\n", num_ticks);
			//printf("Edge 1: %u, Edge 2: %u, Overflows: %u\n", edge1, edge2, overflows);
			overflows = 0;
			pulseDone = 0;
		}
	}
	

}