/*
 * 3_3_1.c
 *
 * Created: 10/3/2018 4:36:25 AM
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
int touch;
long old_paddle_A = 33;
long old_paddle_B = 33;

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
	clear_buffer(buff);
	
	read_screen_init();
	
	//drawline(buff,0,0,0,63,1);         //   game border
	//drawline(buff,0,63,127,63,1);
	//drawline(buff,127,63,127,0,1);
	//drawline(buff,127,0,0,0,1);
	drawline(buff,1,1,1,64,1);
	drawline(buff,1,64,128,64,1);
	drawline(buff,128,64,128,1,1);
	drawline(buff,128,1,1,1,1);
	
	for (int i = 0 ; i <= 127; i=i+2) 
		{
			setpixel(buff,64,i,1);
			setpixel(buff,65,i+1,1);
		}

	write_buffer(buff);

	while (1)
	{	
		touch = standby_mode();
		if (touch){
			long x = read_screen_x(PORTC0, PORTC2, PORTC3, PORTC1);   // x- x+ y- y+
			long y = read_screen_y(PORTC0, PORTC2, PORTC3, PORTC1);
			
			if (x<=64)  // paddle A
				{	
					clearline(buff,4,2,4,63);
					clearline(buff,5,2,5,63);
					if (y<6){
						(y=6);}
					else if (y>123){
						y=123;}
					drawline(buff,4,y+4,4,y-4,1);
					drawline(buff,5,y+4,5,y-4,1);
				}
				
			else if (x>=65)  // paddle B
				{
					clearline(buff,125,2,125,63);
					clearline(buff,124,2,124,63);
					if (y<6){
					(y=6);}
					else if (y>123){
					y=123;}					
					drawline(buff,125,y+4,125,y-4,1);
					drawline(buff,124,y+4,124,y-4,1);		
				}
				
			write_buffer(buff);
			}	

	}
}

