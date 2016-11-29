// Switch.c
// Runs on LM4F120/TM4C123
// Provide functions
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16
#include <stdint.h>
#include "tm4c123gh6pm.h"
//PE0 will be Level 1 and Pause/Play
//PE1 will be Level 2 and Quit
//PE2 will be Level 3 and Smoke?

// **************Switch_Init*********************
// Initialize switch inputs, called once 
// Input: none 
// Output: none
void Switch_Init(void){
	//Port Initialization:
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x10; 		//Port E
	delay = SYSCTL_RCGC2_R; 		//Delay to wait for clock to settle
	
	GPIO_PORTE_AMSEL_R &=~0x07;	//Disable analog function on PE0,1,2
	GPIO_PORTE_PCTL_R &=~0x07; 	//Enable regular GPIO on PE0,1,2
	GPIO_PORTE_DIR_R &=~0x07;		//inputs on PE0,1,2
	GPIO_PORTE_AFSEL_R &=~0x07; //regular function on PE0,1,2
	GPIO_PORTE_PDR_R |=0x07; 		//enable internal pull down resistors on PE0,1,2 (inputs connected to switches)
	//GPIO_PORTE_DEN_R |=0x0F;
	GPIO_PORTE_DEN_R |=0x07; 		//enable digital on PE0,1,2
}
// **************Switch_In*********************
// Input from switch inputs 
// Input: none 
// Output: 0 to 2 depending on input
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Switch_In(void){
	uint32_t input = GPIO_PORTE_DATA_R&0x07; //obtain pins 0-2 of port E
	//assume that user is unable to press all three keys simultaneously and the first to be pressed wins
  return input;
}
