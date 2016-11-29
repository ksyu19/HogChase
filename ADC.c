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
// PE5 will be vert
// PE4 will be horz
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x10; //activate clock for Port E
	while ((SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x14; 	//make PE2 and PE4 input
	GPIO_PORTE_AFSEL_R |= 0x14; //enable alternate function on PE2 and PE4
	GPIO_PORTE_DEN_R &= ~0x14; 	//diable digital I/O on PE2 and PE4
	GPIO_PORTE_AMSEL_R |= 0x14; //enable analog function on PE2 and PE4

	SYSCTL_RCGCADC_R |= 0x01; 	//activate ADC0
	while ((SYSCTL_RCGCADC_R&0x01)==0){}; //delay to let ADC clock initialization finish
	ADC0_PC_R = 0x01;						//configure 125K as max possible sampling rate
	ADC0_SSPRI_R = 0x0123;			//Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;		//disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;			//seq3 is a software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+8; //clear SS3 field and set channel Ain8 (PE5) 
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+9; //clear SS3 field and set channel Ain9 (PE4)
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
// Samples ADC8 and ADC9 
// 125k max sampling
// software trigger, busy-wait sampling
// data returned by reference
// data[0] is ADC8 (PE5) 0 to 4095 - vertical
// data[1] is ADC9 (PE4) 0 to 4095 - horizontal
void ADC_In(uint32_t data[2]){  
	ADC0_PSSI_R = 0x0008;						//initiates SS3
	while((ADC0_RIS_R&0x08)==0){};	//wait for conversion to be done (bit 3, INR3, is set when conversion is complete)
	data[1] = ADC0_SSFIFO3_R&0xFFF;	//read first 12-bit result
	data[0] = ADC0_SSFIFO3_R&0xFFF;	//read second 12-bit result
	ADC0_ISC_R = 0x0008;						//acknowledge completion (clear INR3 and IN3 bits (the second is cleared when INR3 or MASK3 are cleared)) (ADC0_ISC_R has interrupt trigger bits)
}
