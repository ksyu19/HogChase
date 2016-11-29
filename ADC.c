// put implementations for functions, explain how it works
// put your names here, date
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
/*void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x10; //activate clock for Port E
	while ((SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x30; 	//make PE4, PE5 input
	GPIO_PORTE_AFSEL_R |= 0x30; //enable alternate function on PE4, PE5
	GPIO_PORTE_DEN_R &= ~0x30; 	//diable digital I/O on PE4, PE5
	GPIO_PORTE_AMSEL_R |= 0x30; //enable analog function on PE4, PE5

	SYSCTL_RCGCADC_R |= 0x01; 	//activate ADC0
	while ((SYSCTL_RCGCADC_R&0x01)==0){}; //delay to let ADC clock initialization finish
	ADC0_PC_R = 0x01;						//configure 125K as max possible sampling rate
	ADC0_SSPRI_R = 0x0123;			//Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;		//disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;			//seq3 is a software trigger
	ADC0_SSMUX3_R = 0x0089;         // 12) set channels for SS2
	//ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+8; //clear SS3 field and set channel Ain8 (PE5) 
	//ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+9; //clear SS3 field and set channel Ain9 (PE4)
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
*/
void ADC_Init89(void){ 
  volatile uint32_t delay;                         
//  SYSCTL_RCGC0_R |= 0x00010000; // 1) activate ADC0 (legacy code)
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x30;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x30;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x30;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
  GPIO_PORTE_AMSEL_R |= 0x30;     // 6) enable analog functionality on PE4 PE5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R |= 0x0089;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}

//------------ADC_In89------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: two 12-bit result of ADC conversions
// Samples ADC8 and ADC9 
// 125k max sampling
// software trigger, busy-wait sampling
// data returned by reference
// data[0] is ADC8 (PE5) 0 to 4095
// data[1] is ADC9 (PE4) 0 to 4095
void ADC_In89(uint32_t data[2]){ 
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}
