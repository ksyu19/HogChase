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
void displaymenu(void){
	ST7735_SetCursor(0,0);
	ST7735_OutString("Instructions: \nNavigate the pig\nwith the joystick\nto collect all apples.\nAvoid wolves, holes, and \nrunning out of time!\n Menu: \n 1: Level 1 \n 2: Level 2 \n 3: Exit");
};
void displaylevelwin(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("You Win! \nSelect 1 to return to menu.");
};
void displayoverallwin(void){
};
void displaylose(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("Sorry, you lost :(. \nSelect 1 to return to menu.");
};
void displayexit(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("Goodbye!");
};
