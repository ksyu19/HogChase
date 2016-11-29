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
// DAC bit 3 is PB3
// DAC bit 2 is PB2
// DAC bit 1 is PB1
// DAC bit 0 is PB0
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
	SYSCTL_RCGC2_R |= 0x02; 		//Port B
	delay = SYSCTL_RCGC2_R; 		//Delay to wait for clock to settle
	
	GPIO_PORTB_AMSEL_R &=~0x0F; //Disable analog function on PB1-3
	GPIO_PORTB_PCTL_R &=~0x0F; 	//Enable regular GPIO on PB0-3
	GPIO_PORTB_DIR_R |=0x0F; 		//outputs on PB0-3
	GPIO_PORTB_AFSEL_R &=~0x0F; //regular function on PB0-3
	GPIO_PORTB_DEN_R |=0x0F; 		//enable digital on PB0-3
	GPIO_PORTB_DR8R_R |=0x0F;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data<<2; //set pins 2-5 
}
