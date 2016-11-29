// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include <math.h>
#include <stdlib.h>
#include "Graphics.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds


// *************************** Images ***************************
//apple tile (16x16 px) "A"
const unsigned short apple[] = {
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x0000, 0x4D84, 0x4D84, 0x0000, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x0000, 0x0000, 0x2A0C, 0x2A0C, 0x0000, 0x0000, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x2A0C, 0x2A0C, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x2A0C, 0x2A0C, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x0000, 0x0000, 0x0000, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,
 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84, 0x4D84,

};

// *************************** Capture image dimensions out of BMP**********

/*===================================================================================================================================
VECTOR FUNCTIONS: These are so that we can later move our sprites when we interface with the joystick.
=================================================================================================================================*/
typedef struct 
{
	float x, y; 
	
}vector2;
vector2 addVect(vector2 v1, vector2 v2)
{
	vector2 v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	return v3;
}
vector2 subVect(vector2 v1, vector2 v2)
{
	vector2 v3;
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	return v3;
}
void initVect (vector2 *vect, float x, float y)
{
	vect->x = x;
	vect->y = y;
}

float magnitude (vector2 *v)
{
	return (sqrt(v->x*v->x + v->y*v->y));
}	

/*=====================================================
ENTITY
=====================================================*/

const short maxFuel =  100;
typedef enum { PIG, WOLF, NONE } entityType;
typedef enum direction {NORTH, EAST, SOUTH, WEST} Dir;

typedef struct
{
	vector2 position;
	Dir direction;
	float speed;
	short fuel;	//display as "energy"
	float radius;
	entityType type;
}entity;

/*======================================================================
LEVEL STUFF
=======================================================================*/
typedef enum {
	O,//open path
	C,//barrier
	A,//apple
	H,//hole
	P,//pig
	W//wolf
}mapIcons;
const short row = 32;
const short col = 40;

//Loads level data hard coded
void loadLevel(short level,mapIcons map[][col])
{
	if (level == 1)
	{
		mapIcons mapArray[row][col] = 
		{
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,H,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,A,O,O,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,A,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,A,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,H,O,C},
			{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,O,O,O,C,C},
			{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,A,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,A,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,H,O,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,A,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,W,O,P,O,W,O,O,O,O,O,O,O,O,O,A,O,W,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C} 
			};
			
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
	}
	else if (level == 2)
		{
		mapIcons mapArray[row][col] =  
		{
		
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,H,C,C,C,O,O,O,O,O,O,O,O,W,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,H,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,O,O,C,C,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,A,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,H,C,C,C,C,C,C,C,C,C,C,C},
			{C,O,O,C,C,C,A,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C},
			{C,O,O,C,C,C,O,C,C,C,O,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,O,O,O,O,O,O,C,C,C,O,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,C,C,C,O,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,A,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
			{C,C,H,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,O,O,O,C,C},
			{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
			{C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,H,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,A,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,A,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,H,O,O,O,O,O,O,O,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,A,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,W,O,P,O,W,O,W,O,O,O,O,O,A,O,O,O,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C} 
		}; 
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
		}
	else if (level == 3)
		{		
		mapIcons mapArray[row][col] = 
		{
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
			{C,C,A,C,C,C,O,O,O,O,O,O,O,O,W,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,W,C,C,C,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,O,O,C,C,C,C,C,C,C,O,O,C,C,A,O,C,C,C,C,C,O,O,O,O,O,O,A,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
			{C,C,O,O,O,O,H,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,O,C,C,O,C,C,H,C},
			{C,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,O,O,C,C,C,O,C,C,C,O,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,H,O,O,O,O,O,C,C,C,O,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,A,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,C,C,C,O,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,O,O,O,O,O,A,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
			{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,H,C,C,C,C,C,O,O,O,C,C},
			{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
			{C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
			{C,C,O,C,O,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,C,C,O,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,H,O,O,O,C,C,C,O,C,O,O,O,O,O,O,C,C,O,C},
			{C,C,C,C,O,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,A,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
			{C,C,O,O,O,O,O,C,O,O,O,O,O,O,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,C,O,C,C,C,C,O,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
			{C,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
			{C,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
			{C,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
			{C,A,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,O,C,C,O,C},
			{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,O,C,C,A,C},
			{C,C,O,O,O,O,O,H,O,O,C,C,C,O,C,C,O,O,O,W,O,P,O,W,O,W,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
			{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C} 
		}; 
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
		}
};
/*=====================================================================
GAME
=====================================================================*/

typedef struct
{
	//float deltaTime; //Store time since last game update
	short level; //Which level currently on
	mapIcons levelData[row][col]; //Level data
	
	//Entities
	entity enemy [5]; //Array of enemies
	entity player; //Player
	short enemyCount;
	int score;
	short lives;
	
} game;
//------------------------------------------
void initializeEntities(game* G)
{
	G->enemyCount = 0;
	int i=0;
	int j=0;
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (G->levelData[i][j]== P)
				{
					G->player.position.x = j*16 + 8;
					G->player.position.y = i*16 + 8;
					G->player.radius = 4;					//4 is just a placeholder
					G->player.fuel = maxFuel;
					G->player.type = PIG; //DO NOT CLEAR OUT THE PLAYER!
				}
				else if (G->levelData[i][j]== W)
				{
					G->enemy[G->enemyCount].position.x = j*16 + 8;
					G->enemy[G->enemyCount].position.y = i*16 + 8;
					G->enemy[G->enemyCount].radius = 4;					//4 is just a placeholder
					G->enemy[G->enemyCount].fuel = maxFuel;     //Enemies don't lose fuel.
					G->enemy[G->enemyCount].type = WOLF;
					G->levelData[i][j]= O;
					G->enemyCount++;
				}
			
			}
		}
		for (i = G->enemyCount; i < 5; i++)		//The entities that no longer exist must be "invisible"
		{
			G->player.position.x = -1;
			G->player.position.y = -1;
			G->player.radius = 0;					//4 is just a placeholder
			G->player.fuel = 0;
			G->player.type = NONE;
		}
};
//--------------------------------------------------------------------------

void beginGame(game* G)
{
		initializeEntities(G);
		G->score = 0;
		G->lives = 3;
		loadLevel (G->level,G->levelData);
};
 
//------------------------------------------------------------------------------------------------------------------------------------
short nextPosIsWall(entity* ent, mapIcons levelData[row][col])
{
	vector2 nextPos = ent->position;
	if (ent->direction == NORTH)
	{
		nextPos.y -= ent->speed;
	}
	else 	if (ent->direction == SOUTH)
	{
		nextPos.y += ent->speed;
	}
	else 	if (ent->direction == EAST)
	{
		nextPos.x += ent->speed;
	}
	else 	if (ent->direction == WEST)
	{
		nextPos.x -= ent->speed;
	}
	if (levelData[(int)(nextPos.y/16)][(int)(nextPos.x/16)] == C)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
};
//------------------------------------------------------------------------------------------------------------------------------------
void moveEntity (game *G, entity *ent)
{
	while (nextPosIsWall(&G->player , G->levelData) == 1)
	{
		if (ent->direction == NORTH)
		{
			ent->direction = WEST;
		}
		else if (ent->direction == SOUTH)
		{
			ent->direction = EAST;
		}
		else if (ent->direction == WEST)
		{
			ent->direction = SOUTH;
		}
		else if (ent->direction == EAST)
		{
			ent->direction = NORTH;
		}
	}
	vector2 nextPos = G->player.position;
	if (ent->direction == NORTH)
	{
		nextPos.y -= G->player.speed;
	}
	else 	if (ent->direction == SOUTH)
	{
		nextPos.y += G->player.speed;
	}
	else 	if (ent->direction == EAST)
	{
		nextPos.x += G->player.speed;
	}
	else 	if (ent->direction == WEST)
	{
		nextPos.x -= G->player.speed;
	}
	G->player.position = nextPos;
	
}
//------------------------------------------------------------------------------------------------------------------------------------
void respawn (game *G)
{
	int i, j;   // i is y, j is x
	for (i = 0; i < row; i++)
	{
			for (j = 0; j < col; j++)
		{
			if(G->levelData[i][j]== P)
			{
				G->player.position.x = j*16 + 8;
				G->player.position.y = i*16 + 8;					
				G->player.fuel = maxFuel;
			}
		}
	}		
}

/*===================================================================================================================================
PLAYER: This is where the character info is bundled and vectors can be dealt with.
=====================================================================================================================================*/
typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
}player;



/*======================================================================
COLLISION DETECTION
======================================================================*/
int collideWithHole(entity* ent, mapIcons levelData[row][col])
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	return 0;
};

//-----------------------------------------------
int collideWithApple(entity* ent, mapIcons levelData[row][col])
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	return 0;
};
//---------------------------------------------------------
short entitiesColliding(entity* ent1, entity* ent2)
{
	vector2 distance;
		distance.x = abs(ent1->position.x -  ent2->position.x);
		distance.y = abs(ent1->position.y -  ent2->position.y);
		
	if (magnitude(&distance) < ent1->radius + ent2->radius)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//---------------------------------------------------------
short collideWithWolf (game *G)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			if (entitiesColliding(&G->player, &G->enemy[i])== 1)
			{
				return 1;
			}
		}
		
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
void handleCollisions(game *G)
{
	if (collideWithApple(&G->player, G->levelData))
	{
		G->levelData[(int)(G->player.position.y/16)][(int)(G->player.position.x/16)] = O;
		G->score = G->score+50;
	}
	else if (collideWithWolf(G)||(collideWithHole(&G->player,  G->levelData)))
	{
		G->lives--;
		if (G->lives > 0 )
		{
		respawn (G);
		}
		else
		{
			//This is game over.
		}
	}
}


//-------------------------------------------
void clearFrame()
{
	ST7735_FillScreen(0x0000);
}
//-------------------------------------------
int entityIsInView(entity* ent, vector2 camPos)
{
	int minX = camPos.x - 16*4 - 8 ; //need to figure this # out
	int maxX = camPos.x + 16*5 + 8; 
	int minY = camPos.y - 16*3 - 8;
	int maxY = camPos.y + 16*4 + 8; //Need to find min and max x and y on the SCREEN (some math)
	
	if (ent->position.x < minX || ent->position.x > maxX)
	{
		return 0;
	}
	else if (ent->position.y < minY || ent->position.y > maxY)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
//-------------------------------------------
void renderEntity(entity* ent, vector2 camPos)
{
	if (entityIsInView(ent, camPos))
	{
		if ( ent->type == PIG )
		{
			//Draw player Bitmap at player position
		}
		else if ( ent->type == WOLF )
		{
			//Draw enemy Bitmap at enemy position
		}
		else
		{
			//Draw nothing
		}
	}
}	
//----------------------------------------------------------------------------------------------
void renderMap (mapIcons visMap[][10])
{
	//visMap :map icon array of all tiles that are currently visible
	//visMap[0][0] is the top left tile on the screen.
	//visMap[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (visMap[i][j] == C)
			{
				//draw Wall bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
				ST7735_DrawBitmap(j*16+1,i*16+1,wall,16,16);
			}
			else if (visMap[i][j] == A)
			{
				//draw apple bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
				ST7735_DrawBitmap(j*16+1,i*16+1,apple,16,16);
			}
			else if (visMap[i][j] == H)
			{
				ST7735_DrawBitmap(j*16+1,i*16+1,hole,16,16);
			}
			else 
			{
				//draw air ("grass")
				ST7735_DrawBitmap(j*16+1,i*16+1,grass,16,16);
			}
			//NOTE: Entities are not dealt with in this function.
		}
	}
	
};

//-------------------------------------------
void renderFrame(game* G)
{
	vector2 camPos;
	
	mapIcons visMap [8][10];
	//Set camera based on player position
	short camI = G->player.position.y / 16;
	short camJ = G->player.position.x / 16;
	
	camPos.x = camJ + 8;
	camPos.y = camI + 8;
	
	//Render tiles around player
	int i, j, x = 0, y = 0;
	for (i = camI-3; i <= camI+4; i++)
	{
		for (j = camJ - 4; j <= (camJ + 5); j++)
		{
			if(i >= 0 && i < row && j >= 0 && j < col)
			{
				visMap[x][y] = G->levelData[i][j];
			}
			else
			{
				visMap[x][y] = O;
			}	
			y++;
		}
		x++;
	}
	renderMap (visMap);
	
	//Render Enemies
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			renderEntity(&G->enemy[i], camPos);
		}
	}
	
	//Render player
	renderEntity(&G->player, camPos);
	
	clearFrame();
	

}
//-------------------------------------------
void updateGame (game *G)
{
	int i;
	//TO DO: Process player input.
	moveEntity (G, &G->player); 
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			moveEntity (G, &G->enemy[i]);
		}
	}
};


/*=====================================================================
INT MAIN
======================================================================*/
int main()
{
	ST7735_InitR(INITR_REDTAB);
	ST7735_DrawBitmap(1,16,apple,16,16);
	game RallyX;
	TExaS_Init();  // set system clock to 80 MHz
  //Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black


  Delay100ms(50);              // delay 5 sec at 80 MHz
	
	beginGame(&RallyX);  //GETTING A HARD FAULT AROUND HERE
	//Game running
	while(1)
	{
		//updateGame(&RallyX);				//TO DO LATER!!!!!!!!!!!!!!!
		//handleCollisions(&RallyX);
		//renderFrame(&RallyX);
	}
}

// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
