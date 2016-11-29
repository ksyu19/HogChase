// put prototypes for public functions, explain what it does
// put your names here, date

// ADC.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0

// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init89(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion

void ADC_In89(uint32_t data[2]);
