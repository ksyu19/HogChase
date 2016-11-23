// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Student names: Karena Yu and Alex Smith
// Last modification date: 11-16-16

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x10; //activate clock for Port E
	while ((SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x04; 	//make PE2 input
	GPIO_PORTE_AFSEL_R |= 0x04; //enable alternate function on PE2
	GPIO_PORTE_DEN_R &= ~0x04; 	//diable digital I/O on PE2
	GPIO_PORTE_AMSEL_R |= 0x04; //enable analog function on PE2

	SYSCTL_RCGCADC_R |= 0x01; 	//activate ADC0
	while ((SYSCTL_RCGCADC_R&0x01)==0){}; //delay to let ADC clock initialization finish
	ADC0_PC_R = 0x01;						//configure 125K as max possible sampling rate
	ADC0_SSPRI_R = 0x0123;			//Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;		//disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;			//seq3 is a software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; //clear SS3 field and set channel Ain1 (PE2)
	ADC0_SSCTL3_R = 0x0006;			//no TS0 (bit 4)(clear = no temperature reading, yes measure analog voltage on ADC input pin),
															//no D0 (bit 1) (clear = single ended analog input (no differential sampling))
															//yes IE0 (bit 3)(set so INR3 bit is set on ADC conversion)
															//yes END0 (bit 2)(using sequencer 3 = only once sample, so END0 is set (this sample is end of the sequence)
	ADC0_IM_R &= ~0x0008;				//disable SS3 interrupts (ADC0_IM_R has interrupt arm bits)
	ADC0_ACTSS_R |= 0x0008;			//enable sample sequencer 3
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion, 0 to 3.3 V maps to 0 to 4095 
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;						//initiates SS3
	while((ADC0_RIS_R&0x08)==0){};	//wait for conversion to be done (bit 3, INR3, is set when conversion is complete)
	result = ADC0_SSFIFO3_R&0xFFF;	//read 12-bit result
	ADC0_ISC_R = 0x0008;						//acknowledge completion (clear INR3 and IN3 bits (the second is cleared when INR3 or MASK3 are cleared)) (ADC0_ISC_R has interrupt trigger bits)
  return result;
}



// put implementations for functions, explain how it works
// put your names here, date
// put implementations for functions, explain how it works
// put your names here, date

/*==================================================================================================================================
GLOBALS:
====================================================================================================================================*/
const int MAXFUEL = 100;

/*===================================================================================================================================
VECTOR FUNCTIONS:
These are so that we can later move our sprites when we interface with the joystick.
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


/*===================================================================================================================================
PLAYER:
These probably shouldn't be in the ADC.c file. We might just need another module for logic updates.
=====================================================================================================================================*/
typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
	short alive; //1 if alive, 0 if dead
	unsigned short fuel; //to check for scoring and life purposes
	
}player;

void initPlayer (player *player1)
{
	player1->alive = 1;
	player1->fuel = MAXFUEL;
	
}
