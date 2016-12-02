// Switch.c
// Runs on LM4F120/TM4C123
// Provide functions
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16
#include <stdint.h>
#include "tm4c123gh6pm.h"
//PE0 will be Level 1 and Resume
//PE1 will be Level 2 and Quit
//PE2 will be Level 3
//PE3 will be Instructions and Smoke?

// **************Switch_Init*********************
// Initialize switch inputs, called once 
// Input: none 
// Output: none
void Switch_Init(void){
	//Port Initialization:
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x10;	//Port E
	delay = SYSCTL_RCGCGPIO_R; 		//Delay to wait for clock to settle
	GPIO_PORTE_AMSEL_R &=~0x0F;	//Disable analog function on PE0,1,2,3
	GPIO_PORTE_PCTL_R &=~0x0F; 	//Enable regular GPIO on PE0,1,2,3
	GPIO_PORTE_DIR_R &=~0x0F;		//inputs on PE0,1,2,3
	GPIO_PORTE_AFSEL_R &=~0x0F; //regular function on PE0,1,2,3
	GPIO_PORTE_PDR_R |=0x07; 		//enable internal pull down resistors on PE0,1,2 (inputs connected to switches)
	GPIO_PORTE_PUR_R |=0x08; 		//enable internal pull down resistors on PE3 (inputs connected to joystick)
	GPIO_PORTE_DEN_R |=0x0F; 		//enable digital on PE0,1,2,3
}
// **************Switch_In*********************
// Input from switch inputs 
// Input: none 
// Output: 0 to 2 depending on input
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Switch_In(void){
	uint32_t input = GPIO_PORTE_DATA_R&0x0F; //obtain pins 0-2 of port E
	switch(input){
		case 9:
			input = 1; break;
		case 10:
			input = 2; break;
		case 12:
			input = 3; break;
		case 0:
			input = 4; break;
		case 8:
			input = 0; break;
	}
  return input;
}
