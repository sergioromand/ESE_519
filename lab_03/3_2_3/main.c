/*
 * 3_2_3.c
 *
 * Created: 9/28/2018 6:30:44 PM
 * Author : yatesyatesyates
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"

#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001

char displayChar = 1;


int main(void)
{
	 uart_init();
	 
	//setting up the gpio for backlight
	DDRD |= 0x80;
	PORTD &= ~0x80;
	PORTD |= 0x00;
	
	DDRB |= 0x05;
	PORTB &= ~0x05;
	PORTB |= 0x00;
	
	//lcd initialisation
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	_delay_ms(10000);
	clear_buffer(buff);
	
	while (1)
	{
		write_buffer(buff);
		read_screen_init();

		int x = read_screen_x(PORTC0, PORTC2, PORTC3, PORTC1);   // x- x+ y- y+
		//int y = read_screen_y(PORTC0, PORTC2, PORTC3, PORTC1);
		
		printf("x = %u\n V", x);
		
		//printf("y = %u\n V", y);
	}
}

