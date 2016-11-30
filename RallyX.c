// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)
#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "PlayGame.h"
#include "Display.h"
#include "Sound.h"
#include "Switch.h"
#include "Timer0.h"
#include "DAC.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm
int32_t TxCounter = 0;


void PortF_Init(void)
	{unsigned long volatile delay;
  // Intialize PortF for hearbeat
	SYSCTL_RCGCGPIO_R |=0x20; //turn on Port F clock
	delay = SYSCTL_RCGCGPIO_R; 	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; //unlock GPIO Port F
	GPIO_PORTF_CR_R |= 0x0E; //allow changes to PF1,2,3
	GPIO_PORTF_AMSEL_R &=~0x0E; //Disable analog function on PF1,2,3
	GPIO_PORTF_PCTL_R &=~0x0E; 	//Enable regular GPIO on PF1,2,3
	GPIO_PORTF_DIR_R |=0x0E; 		//output on PF1,2,3
	GPIO_PORTF_AFSEL_R &=~0x0E; //regular input and output on PF1,2,3
	GPIO_PORTF_DEN_R |=0x0E; 		//enable digital on PF1,2,3
}

// final main program
int menu(){
	int m;
	//display stuff
	//wait for user input
	//return choice
	return m;
}

int main1(void){ 
  TExaS_Init();       // Bus clock is 80 MHz 
	ST7735_InitR(INITR_REDTAB);
  //ADC_Init();    			// initialize to sample ADC0
  //PortF_Init();
  //LCD_OutFix(0);
  //ST7735_OutString("     cm");
	//SysTick_Init();
	//IO_Init();
  //EnableInterrupts();
	int m;
  while(1){
		m = menu();
		//insert enum - for now, use int response
		int response;
		displaymenu();
		switch(m){
			case 1: response = playgame(1); break;//level 1 difficulty
			case 2: response = playgame(2); break;//level 2 difficulty
			case 3: response = playgame(3); break;//level 3 difficulty
			case 4: displayexit(); break;
			default: break;
		}
		switch(response){
			case -1: break;//didn't win or lose
			case 0: displaylose(); break;//lose
			default: displaylevelwin(); break;//display level win (anything above = number of points earned)
		}
	}
}//main

//ADC DEBUGGING
/*

uint32_t ADCvalue[2];
char Pos;
char ConvertTest(uint32_t input[2]){//CALIBRATE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//data[0];//North/South (vert - PE5)
  //data[1];//East/West (horz - PE4)
	char output = 0;
	if (4096>input[0]&&input[0]>=3500){
		output = 'N';
	}
	else if (500>=input[0]&&input[0]>0){
		output = 'S';
	}
	else if (500>=input[1]&&input[1]>0){
		output = 'E';
	}
	else if (4096>input[1]&&input[1]>=3500){
		output = 'W';
	}
  return output;
}
int main(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init89();         // turn on ADC, set channel to 1
  while(1){  
		ADC_In89(ADCvalue);  // sample 12-bit channel 1
    Pos = ConvertTest(ADCvalue); 
    ST7735_SetCursor(0,0);
		ST7735_OutString(&Pos);
    //LCD_OutDec(ADCvalue[0]); ST7735_OutString("vert "); //print vertical
    //ST7735_SetCursor(6,0);
		//LCD_OutDec(ADCvalue[1]); ST7735_OutString("horiz ");  //print horizontal
		for(int i = 0; i<266667; i++){
		}
  }
}   
*/
//SWITCH DEBUGGING
/*
int mainSwitch(void){
	TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
	Switch_Init();
	uint32_t input, previous; 
	previous = 0;
	while(1){
		ST7735_SetCursor(0,0);
		ST7735_OutString("ON");
		input = Switch_In(); 
		if(input&&(previous ==0)){ // just pressed 
			switch(input){
				case 1: ST7735_OutString("first button");break;
				case 2: ST7735_OutString("second button");break;
				case 4: ST7735_OutString("third button");break;
				default:break; 
			}
		} 
		previous = input;
		for(int i = 0; i<266667; i++){
		}
	}
}
*/

//Timer0 DEBUGGING
/*
int statusTest;
void Timer0Test (void){
	statusTest = 1;
}
int mainTimer(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
	Timer0_Init(Timer0Test, 80000000/11025);
	statusTest = 0;
	int i = 0;
	EnableInterrupts();
  while(1){
		if (statusTest == 1){
			DisableInterrupts();
			statusTest = 0;
			ST7735_SetCursor(0,0);
			ST7735_OutString("ON");
			LCD_OutDec(i);
			i= (i+1)%500;
			EnableInterrupts();
		}
  }         
}   
*/
/*
//SOUND DEBUGGING
int statusTest;
int main(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
	Switch_Init();
  Sound_Init();
	statusTest = 0;
	int i = 0;
	uint32_t input, previous; 
	previous = 0;
	EnableInterrupts();
  while(1){
		input = Switch_In(); 
		if(input&&(previous ==0)){ // just pressed 
			switch(input){
				case 1: Sound_Flag();break;
				case 2: Sound_Chomp();break;
				//case 4: Sound_Round_Clear();break;
				default:break; 
			}
		} 
		for(int i = 0; i<266667; i++){
		}
		previous = input;
  }         
}   
*/
