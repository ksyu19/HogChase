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

void SysTick_Init(void){
		//SYSTICK Initialization
	NVIC_ST_CTRL_R = 0;                   											// disable SysTick during setup
  NVIC_ST_RELOAD_R = 2000000;  																// reload value for interrupts at 40 Hz (0.025 s)
  NVIC_ST_CURRENT_R = 0;                											// any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;  // priority 1   
  NVIC_ST_CTRL_R = 0x0007;																		// enable SysTick with core clock and interrupts
}
void SysTick_Handler(void) // every 25 ms
{
	PF1 ^= 0x02; //toggle heartbeat LED 
	//Data = ADC_In(); //sample ADC
	PF1 ^= 0x02; //toggle heartbeat LED 
	PF1 ^= 0x02; //toggle heartbeat LED 
	PF1 ^= 0x02; //toggle heartbeat LED 
}

// final main program
int menu(){
	int m;
	//display stuff
	//wait for user input
	//return choice
	return m;
}

int main(void){ 
  //TExaS_Init();       // Bus clock is 80 MHz 
	//ST7735_InitR(INITR_REDTAB);
  //ADC_Init();    			// initialize to sample ADC1
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
uint32_t Convert(uint32_t input){
	uint32_t Xtable[] = {210,760,1678,3189,4089};	//ADC results
	uint32_t Ytable[] = {300,500,1000,1500,2000};	//positions (in 0.001 cm)
	uint32_t x0,x1,y0,y1;
	uint32_t output;
	for(int i = 0;i<5; i++){
		if (Xtable[i] == input){
			output = Ytable[i];
			break;
		}
		else{
			if (Xtable[i]>input){
				if(i==0){
					x0 = 0;
					y0 = 0;
				}
				else {
					x0 = Xtable[i-1];
					y0 = Ytable[i-1];
				}
				x1 = Xtable[i];
				y1 = Ytable[i];
				output = y0+(input - x0)*(y1-y0)/(x1-x0);
				break;
			}
			else if(i==4){
				x0 = Xtable[i];
				y0 = Ytable[i];
				x1 = 4095;
				y1 = 2000;//or how many cm long it is
				output = y0+(input - x0)*(y1-y0)/(x1-x0);
			}
		}
	}//for
  return output;
}

uint32_t ADCvalue[2];
//ADCvalue[0] = 
int main(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 1
  while(1){  
		ADC_In(ADCvalue);  // sample 12-bit channel 1
    //Position = Convert(ADCvalue[0]); 
    ST7735_SetCursor(0,0);
    LCD_OutDec(ADCvalue[0]); ST7735_OutString("    "); //print vertical
    ST7735_SetCursor(6,0);
		LCD_OutDec(ADCvalue[1]); ST7735_OutString("    ");  //print horizontal
    //LCD_OutFix(Position);
		for(int i = 0; i<266667; i++){
		}
  }
}   
*/
//SOUND DEBUGGING
