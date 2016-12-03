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

// final main program
int m;
int menu(int totalScore){
	m = 0;
	displaymenu(totalScore);
	while(m == 0){
		m = Switch_In();
	}
	return m;
}

int main(void){ 
  TExaS_Init();       // Bus clock is 80 MHz 
	ST7735_InitR(INITR_REDTAB);
  ADC_Init89();    			// initialize to sample ADC0
	Switch_Init();
	int totalScore = 0;
  while(1){
		DisableInterrupts();
		ST7735_FillScreen(0);            // set screen to black
		m = menu(totalScore);
		//insert enum - for now, use int response
		int response;
		switch(m){
			case 1: 
				response = playgame(1); break;//level 1 difficulty
			case 2: 
				response = playgame(2); break;//level 2 difficulty
			case 3: 
				response = playgame(3); break;//level 3 difficulty
			case 4:
				displayInstructions(); break;//display instructions
			default: break;
		}//menu result
		DisableInterrupts();
		totalScore = response + totalScore;
		delay(100);
	}//while
}//main

//ADC DEBUGGING
/*

uint32_t ADCvalue[2];
char Pos;
char ConvertTest(uint32_t input[2]){
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
		ST7735_FillScreen(0);
		ST7735_SetCursor(0,0);
		ST7735_OutString("ON\n");
		input = Switch_In(); 
		LCD_OutDec(input);
		if(input&&(previous ==0)){ // just pressed 
			switch(input){
				case 1: ST7735_OutString("first button");break;
				case 2: ST7735_OutString("second button");break;
				case 3: ST7735_OutString("third button");break;
				case 4: ST7735_OutString("joystick button");break;
				case 0: ST7735_OutString("no button");break;
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
	uint32_t input, previous; 
	previous = 0;
	EnableInterrupts();
  while(1){
		input = Switch_In(); 
		if(input&&(previous ==0)){ // just pressed 
			switch(input){
				case 1: Sound_Flag();break;
				case 2: Sound_Chomp();break;
				case 3: Sound_Music();break;
				case 4: Music_Stop();break;
				default:break; 
			}
		} 
		for(int i = 0; i<266667; i++){
		}
		previous = input;
  }         
}   
*/
