// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>
#include "Sound.h"
#include "DAC.h"
#include "tm4c123gh6pm.h"


void Sound_Init(void){
	DAC_Init();
	//initialize PF3 for heartbeat (interrupts being called)
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x20; 		//Port F
	delay = SYSCTL_RCGC2_R; 		//Delay to wait for clock to settle
	
	GPIO_PORTF_AMSEL_R &=~0x08;	//Disable analog function on PF3
	GPIO_PORTF_PCTL_R &=~0x08; 	//Enable regular GPIO on PF3
	GPIO_PORTF_DIR_R |=0x08;		//output on PF3
	GPIO_PORTF_AFSEL_R &=~0x08; //regular function on PF3
	GPIO_PORTF_DEN_R |=0x08; 		//enable digital on PF3
};
void Sound_Play(const uint8_t *pt, uint32_t count){
// write this
};
void Sound_Shoot(void){
// write this
};
void Sound_Killed(void){
// write this
};
void Sound_Explosion(void){
// write this
};

void Sound_Fastinvader1(void){
// write this
};
void Sound_Fastinvader2(void){
// write this
};
void Sound_Fastinvader3(void){
// write this
};
void Sound_Fastinvader4(void){
// write this
};
void Sound_Highpitch(void){
// write this
};
