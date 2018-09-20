/*
 * Lab 0222.c
 *
 * Created: 9/20/2018 1:38:14 AM
 * Author : yatesyatesyates
 */ 
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr/delay.h"

unsigned int diff, edge1, edge2, overflows; 
unsigned long pulse_width; 


ISP(TIMER1_COMPA_vect)
{ 
	DDRB	&= 0xFD;			// PIN 1 is now input
}

ISP(TIMER1_CAPT_vect)
{
	
}

volatile	int	overflows;

ISP(TIMER1_OVF_vect)
{
	overflows ++;				//increment the overflow counter
}

unsigned int pulse_width()
{
	TIFR1	|=	0x20;			// clear the input capture flag
	while (!(TIFR1 & 0x20));	// wait until the flag is set to 1 
								// due to rising edge 
	unsigned int temp = ICR1;	// save the timestamp of the edge
	TCCR1B &= 0xBF;				// capture a falling edge 
	TIFR1 |= 0x20;				// clear the input capture flag 
	while (!(TIFR1 & 0x20));	// wait until flag is set to 1 due 
								// to falling edge 
	return ICR1 - temp;			// return the diff in timestamps

}

void timer1_init()
{
	TIMSK1	|=  0x01;			// enable overflow interrupt
	TIMSK1	|=	0x02;			// enable output compare A interrupt
	
	TCCR1A	|=	0x20;			// clear OC1A/B on compare match  (..10....) bit 5 set
	TCCR1A  &=	0xEF;           // clear OC1A/B on compare match  (..10....) bit 4 clear
	
	TCCR1B	|=	0x01;			// setup clock , no prescaler
	TCCR1B	&=	0xF9;			// setup clock , no prescaler (.....001)
	
	TCCR1B	|= 0x40;			// capture a rising edge 
	
	OCR1A	=	82;				// 1/16MHz = .06us   --> 5us/.06us = 83.333  ---->  for sending 5us pulse
	
	TCNT1	=	0;				// initialize counter
	
	
}

int main(void)
{
	uart_init();
	
	DDRB	|=	0x02;			// PB1 output 
	DDRB	&=  0xFE;			// PB0 input 
	
	PORTB	|= 0x02;			// set PB1 high
	
	timer1_init();
	
	for (;;) {					// loop forever 
		while (!(PINB & 0x01)); // while the pin is low, do nothing 
		overflows = 0;			// start counting from 0 
		while (PINB & 0x01);	// while the pin is high, do nothing 
		
		printf("Key released: %u\n", overflows); }

	
}