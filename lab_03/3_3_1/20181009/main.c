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
#define A 1
#define B 2

char displayChar = 1;
int touch;

int touch_x;
int touch_y;
int player_side;
int goal;
int ball_contact;
int updownA;
int updownB;
int paddleA_moving = 0;
int paddleB_moving = 0;
int paddleA_dy;
int paddleB_dy;
int old_paddleA = 32;
int old_paddleB = 32;
int newA; 
int newB;

int main(void)
{
	uart_init();
	 
	//setting up the gpio for backlight
	DDRD |= 0x80;  // define PD7 as output
	DDRB |= 0x05;  // define PB0,PB2 as output


	lcd_grassgreen();

	//lcd initialization
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	clear_buffer(buff);
	
	read_screen_init();
	
	init_play(32,32);
	
	while (1)
	{	
		
		/*  working prototype 10.04.18	
		touch = standby_mode();
		if (touch){
			move_paddle();	
			write_buffer(buff);
			}	
	*/
		
		draw_game_borders();
		write_buffer(buff);
		
		touch = standby_mode();
		if (touch)
		{
			touch_x = read_screen_x(PORTC0, PORTC2, PORTC3, PORTC1);   // x- x+ y- y+
			touch_y = read_screen_y(PORTC0, PORTC2, PORTC3, PORTC1);
			if		(touch_y<6)					{touch_y=6;}
			else if (touch_y>59)				{touch_y=59;}   // prevent paddle from trying to draw out-of-bounds
			
			if	(touch_x <= 64)	
			{ 
				newA = touch_y;
				paddleA_dy = abs(old_paddleA-touch_y); 
				paddleA_moving = 1;
				if		(old_paddleA > newA)	{updownA = 1;}
				else if (old_paddleA < newA)	{updownA = 0;}				
			}
			else if	(touch_x >= 65)	
			{ 
				newB = touch_y;
				paddleB_dy = abs(old_paddleB-touch_y);
				paddleB_moving = 1; 
				if		(old_paddleB > newB)	{updownB = 1;}
				else if (old_paddleB < newB)	{updownB = 0;}				
			}
		}       //  if we get a touch then measure x, y and determine which side of the board it is on,
				//	then calc the distance from current position to new position for corresponding paddle
				//  and set the "paddle moving" flag for the corresponding paddle 
			
		if	 (paddleA_moving==1)	
		{
			if ( (updownA==1) && (paddleA_dy>=0) )
			{
				draw_paddleA(newA+paddleA_dy);
				paddleA_dy--;
				old_paddleA = newA+paddleA_dy;
			}
			else if ( (updownA==0) && (paddleA_dy>=0) ) 
			{
				draw_paddleA(newA-paddleA_dy);
				paddleA_dy--;
				old_paddleA = newA-paddleA_dy;
			}
			
			if ( paddleA_dy < 0 )
			{
				paddleA_moving = 0;
			}			
		}	
		
		if   (paddleB_moving==1)  // paddle B
		{

			if ( (updownB==1) && (paddleB_dy>=0) )
			{
				draw_paddleB(newB+paddleB_dy);
				paddleB_dy--;
				old_paddleB = newB+paddleB_dy;
			}
			else if ( (updownB==0) && (paddleB_dy>=0) )
			{
				draw_paddleB(newB-paddleB_dy);
				paddleB_dy--;
				old_paddleB = newB-paddleB_dy;
			}
			
			if ( paddleB_dy < 0 )
			{
				paddleB_moving = 0;
			}				
		}
		
		if (ball_contact){;
			//	check for ball contact with wall or paddle
			//		update next "x" and "y" for ball 
		}
		if (goal){;
			//	check for goal
			//		if goal then increment appropriate score and init play
			//		if someone's score is 10 then celebrate winner and reset score
		}
			//			move_paddle();
			//			move_ball();
			//			write_buffer(buff);
			
		}	
		
	}


