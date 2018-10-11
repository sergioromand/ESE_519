Pong Lab 03
Authors: Michael Yates, Sergio Roman 
High Level Description: This is the code for a pong game on an LCD screen lying beneath a resistive touchscreen. There are 3 versions of the game, each 
of which is in a separate folder. 
/touch allows you to play with 2 people, each controlling their paddle by touching the screen. 
/touch_ai allows you to play against an AI opponent, where the paddle is controlled by touching the screen. 
/acceleromter allows you to play pong against an AI opponent, where the paddle is controlled by tilting the board. 

Inside each folder there are 5 files. Below is a description of each: 
Makefile- Builds the program using avrdude (for Mac Users).
uart.h- provides printf functionality to a serial monitor. Used mainly for debugging. 
lcd.c- high-level LCD driver, providing all functionality for controlling the LCD display. This includes screen initialization, setting colors, drawing sprites and shapes, and also interfacing the touchscreen and accelerometer via ADC, and the buzzer that rings when walls are hit. The file accelerometer/lcd.c has a readTilt function that the others do not, since this is the function that allows us to read the accelerometer values that the other versions do not use. 
lcd.h- header file for lcd.c
main.c- Main program that runs the game. This is the part that varies the most from folder to folder. This program works by using the functions in lcd.c. After initializign the game, at each cycle of a while loop drawing the current buffer, updating the position of the paddles (when applicable) and ball, and checking for wall contacts, paddle contacts, or goals. If the game ends and there's a winner, the program simply re-initializes. 

Running the game:
On Mac, enter the desired directory in the terminal and type "make". Make sure the device field in the makefile is the correct one (you can see wat to write there by typing "ls /dev/tty.usb*" into the terminal). 
On Windows, run normally using ATMEL Studio, making sure that you are running the desired version of main.c