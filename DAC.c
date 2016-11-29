// put implementations for functions, explain how it works
// put your names here, date
// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Karena Yu
// Date Created: 8/25/2014 
// Last Modified: 10/19/2016
// Section Wed 4:30 PM    Instructor: Vijay Reddi
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
// DAC bit 3 is PA5
// DAC bit 2 is PA4
// DAC bit 1 is PA3
// DAC bit 0 is PA2
#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	//Port Initialization:
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x01; 		//Port A
	delay = SYSCTL_RCGC2_R; 		//Delay to wait for clock to settle
	
	GPIO_PORTA_AMSEL_R &=~0x3C; //Disable analog function on PA2-5
	GPIO_PORTA_PCTL_R &=~0x3C; 	//Enable regular GPIO on PA2-5
	GPIO_PORTA_DIR_R |=0x3C; 		//outputs on PA2-5
	GPIO_PORTA_AFSEL_R &=~0x3C; //regular function on PA2-5
	GPIO_PORTA_DEN_R |=0x3C; 		//enable digital on PA2-5
	GPIO_PORTA_DR8R_R |=0x3C;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTA_DATA_R = data<<2; //set pins 2-5 
}
