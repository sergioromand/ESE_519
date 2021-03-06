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
int newA = 32; 
int newB = 32;

int contact_point;


int ball_oldX;
int ball_oldY;
int ball_dX;
int ball_dY;
int ball_newX;
int ball_newY;

int tipoff = 0;

int continue_after_goal;
int increase_speed=0;

//accelerometer version
//maxLeft = 264
//still = 318
//maxRight = 372


void init_play(int A, int B){
	

	if ( (Ascore == 3 ) || (Bscore == 3 ) ){   // if someone gets to 10,
		celebrate_winner();                    // flash the LCD and start over game
		Ascore = 0;
		Bscore = 0;
		increase_speed = 0;

	}
	
	if ( (Ascore == 0) && (Bscore == 0) ){
		;  // say "new game!!!"
	}
	lcd_grassgreen();
	
	display_score_A(Ascore);
	display_score_B(Bscore);
	
	draw_paddleA(A);
	draw_paddleB(B);
	
	ball_newX = 64;
	ball_newY = 32;
	
		ball_oldX = 64;
		ball_oldY = 32;
	
	draw_ball(ball_newX,ball_newY);
	
	continue_after_goal=6;

	paddleA_moving = 0;

	increase_speed ++;
	
	tipoff = rand()%8;
	switch (tipoff)
	{
		case 0:
			ball_dX = 1;
			ball_dY = 1;
			break;
		case 1:
			ball_dX = 1;
			ball_dY = 2;
			break;
		case 2:
			ball_dX = 1;
			ball_dY = -1;
			break;
		case 3:
			ball_dX = 1;
			ball_dY = -2;
			break;
		case 4:
			ball_dX = -1;
			ball_dY = 1;
			break;
		case 5:
			ball_dX = -1;
			ball_dY = 2;
			break;
		case 6:
			ball_dX = -1;
			ball_dY = -1;
			break;
		case 7:
			ball_dX = -1;
			ball_dY = -2;
			break;
	}
	
	newA = 32;
	newB = 32;
	
	//printf("ball_newY init  = %d \n ", ball_newY);

}

int main(void)
{
	uart_init();
	 
	//setting up the gpio for backlight
	DDRD |= 0x80;  // define PD7 as output
	DDRB |= 0x05;  // define PB0,PB2 as output



	//lcd initialization
	lcd_init();
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	clear_buffer(buff);
	
	init_play(32,32);
	
	//read_screen_init();
	
	
		//printf("ball_newY after init  = %d \n ", ball_newY);

	while (1)
	{			
		draw_game_borders();
		
		display_score_A(Ascore);
		display_score_B(Bscore);
			
		write_buffer(buff);

		// move the A paddle
		int tilt = readTilt();
		//adjust to x-y position
		int newA = ((tilt - 250) * 53) / 108;
		newA = (59 - newA) + 10;
		if		(newA<6)				{newA=6;}
		else if (newA>59)				{newA=59;}
		draw_paddleA(newA);

		
		/////////////////////////////////////////////////////////
		///////////    just move the ball ///////////////////////
		/////////////////////////////////////////////////////////		
							
				ball_oldX = ball_newX;
				ball_oldY = ball_newY;
				
				ball_newX += ball_dX;
				ball_newY += ball_dY;
				
				clear_ball(ball_oldX,ball_oldY);
				draw_ball(ball_newX,ball_newY);
				
				//printf("new B = %d ball_newY int  = %d \n ",newB, ball_newY);
				
				//printf("ball_dX = %d  ball_dY = %d  \n", ball_dX, ball_dY);
				//printf("ball_newX = %d   ball_newY = %d\n", ball_newX, ball_newY);
				
		////////////////////////////////////////////////////////////			
		if ( (ball_newY >= 60) || (ball_newY <= 4) )  // if the new posistion hits a wall, change dY
		{ 
			ball_dY = - ball_dY; 
			buzz();
		}
		////////////////////////////////////////////////////////////
		if (ball_newX <= 8)       
		{			             // the the new position is in the paddleA zone, check for contact, else score a goal
			if ( ((newA-6) <= ball_newY) && (ball_newY <= (newA+6)) && (ball_newX == 8) )
			{ 
				ball_dX = - ball_dX;
				buzz();
			
				contact_point = ( ball_newY - (newA-6) ); 
				
				//printf("contact point = %d\n", contact_point);

				switch (contact_point)
				{
				case 1:
					ball_dY = -3;
					break;
				case 2:
					ball_dY = -3;
					break;
				case 3:
					ball_dY = -2;
					break;
				case 4:
					ball_dY = -2;
					break;
				case 5:
					ball_dY = -1;
					break;
				case 6:
					ball_dY = 1;
					break;
				case 7:
					ball_dY = 1;
					break;
				case 8:
					ball_dY = 2;
					break;
				case 9:
					ball_dY = 2;
					break;
				case 10:
					ball_dY = 3;
					break;
				case 11:
					ball_dY = 3;
					break;	
				}
			}
		
			else  
			{
				continue_after_goal--;
				
				if (continue_after_goal == 0)
				{
				Bscore++;
				buzz();
				lcd_skyblue();
				clear_ball(ball_newX,ball_newY);
				_delay_ms(2000);
				init_play(32,32);
				}
			}	
		}
		////////////////////////////////////////////////////////////
		if(ball_newX >= 117 & ball_newX <= 120) 
		{
			newB = ball_newY;
			paddleB_dy = abs(old_paddleB-newB);
			if		(old_paddleB > newB)	{updownB = 1;}
			else if (old_paddleB < newB)	{updownB = 0;}	
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

		////////////////////////////////////////////////////////////
		if (ball_newX >= 121)
		{			             // the the new position is in the paddleB zone, check for contact, else score a goal
	
			if ( ((newB-6) < ball_newY) && (ball_newY < (newB+6)) && (ball_newX == 121)  )
			{
				ball_dX = - ball_dX;
				
				contact_point = ( ball_newY - (newB-6) );
				buzz();
				
				//printf("contact point = %d   newB = %d   ball_newY = %d  \n", contact_point, newB, ball_newY);
							
				switch (contact_point)
				{	
					
					case 1:
						ball_dY = -3;
						break;
					case 2:
						ball_dY = -3;
						break;
					case 3:
						ball_dY = -2;
						break;
					case 4:
						ball_dY = -2;
						break;
					case 5:
						ball_dY = -1;
						break;
					case 6:
						ball_dY = 1;
						break;
					case 7:
						ball_dY = 1;
						break;
					case 8:
						ball_dY = 2;
						break;
					case 9:
						ball_dY = 2;
						break;
					case 10:
						ball_dY = 3;
						break;
					case 11:
						ball_dY = 3;
						break;
					default:
						//printf("default");
						break;
				}
			}
			
			else
			{
				continue_after_goal--;
				if (continue_after_goal==0)
				{				
				Ascore++;
				buzz();
				lcd_skyblue();
				_delay_ms(2000);				
				clear_ball(ball_newX,ball_newY);
				init_play(32,32);
				}
			}	
		}

	}
}
