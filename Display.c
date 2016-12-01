// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16
//    16-bit color, 128 wide by 160 high LCD
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground
#include <stdint.h>
#include "ST7735.h"

void displayradar(void){
};
void displaymenu(int totalScore){
	ST7735_SetCursor(0,0);
	ST7735_OutString("Instructions: \nNavigate the pig\nwith the joystick\nto collect all apples.\nAvoid wolves, holes, and \nrunning out of time!\n Menu: \n 1: Level 1 \n 2: Level 2 \n 3: Level 3");
	ST7735_OutString("\nTotal Score: ");
	LCD_OutDec(totalScore);
};
void displaylevelwin(int time, int lives, int score){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("You Win! \n\nRaw Score: ");
	LCD_OutDec(score);
	ST7735_OutString("\nTime Bonus: ");
	LCD_OutDec(time);
	ST7735_OutString("\nLives Bonus: ");
	LCD_OutDec(lives*100);
	ST7735_OutString("\n\nLevel Score: ");
	LCD_OutDec(score+time+lives*100);
	ST7735_OutString("\nPress any button\nto return to menu.");
};
void displayoverallwin(void){
};
void displaylose(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("Sorry, you lost :(. \nPress any button\nto return to menu.");
};
void displayexit(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("Goodbye!");
};
void displayLives(int lives,int time){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	if(time == 0){
		ST7735_OutString("Out of Time!\n");
	}
	ST7735_OutString("Lives Left: ");
	LCD_OutDec(lives);
	ST7735_OutString("\nPress any button\n to continue");
};
