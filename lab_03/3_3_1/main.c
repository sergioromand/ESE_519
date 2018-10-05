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
	
	//lcd initialization
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	clear_buffer(buff);
	
	read_screen_init();
	
	init_play();
draw_game_borders();
write_buffer(buff);
	
	while (1)
	{	
		
		//  working prototype 10.04.18	
		touch = standby_mode();
		if (touch){
			move_paddle();	
			write_buffer(buff);
			}	
	
		
		/*draw_game_borders();
		write_buffer(buff);
		
		touch = standby_mode();
		if (touch){
			//	determine "A side touch" or "B side touch" 
			//		update next "y" for draw_paddleA or next "y" for draw_paddleB
		}
		if (ball contact){
			//	check for ball contact with wall or paddle
			//		update next "x" and "y" for ball 
		}
		if (goal){
			//	check for goal
			//		if goal then increment appropriate score and init play
			//		if someone's score is 10 then celebrate winner and reset score
		}
			//			move_paddle();
			//			move_ball();
			//			write_buffer(buff);
		}*/	
		
	}
}

