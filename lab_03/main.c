#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"

#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001

char displayChar = 1;


int main(void)
{
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
		drawchar(buff,0,0,displayChar);
		
		clearpixel(buff,11,15);
		clearpixel(buff,12,16);
		clearpixel(buff,13,17);
		clearpixel(buff,14,18);

		write_buffer(buff);
		
		//invert_screen();		

		_delay_ms(5000);
		
		setpixel(buff,11,15,1);
		setpixel(buff,12,16,1);
		setpixel(buff,13,17,1);
		setpixel(buff,14,18,1);
		write_buffer(buff);
		
		//uninvert_screen();
		
		_delay_ms(5000);
		
		fillrect(buff,25,20,5,5,1);
		
		drawrect(buff,37,20,6,6,1);
		
		drawline_hor(buff,50,65,3,1);
		drawline_hor(buff,65,50,5,1);
		
		drawline_vert(buff,30,50,5,1);
		drawline_vert(buff,31,51,7,1);
		//displayChar++;
		_delay_ms(5000);
		
		/*black_screen();
		_delay_ms(5000);
		unblack_screen();
		_delay_ms(5000);*/
		
		drawline(buff,30,30,30,50,1);
		drawline(buff,30,30,50,30,1);
		drawline(buff,30,30,50,50,1);
		
		unsigned char *meh = "meh";
		
		drawstring(buff,75,1,meh);

		drawcircle(buff,75,40,10,1);
		
		fillcircle(buff,100,50,6,1);
	}
}

