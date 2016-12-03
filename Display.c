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
#include "PlayGame.h"
#include "Switch.h"

void displayradar(void){
};
void displaymenu(int totalScore){
	ST7735_SetCursor(0,0);
	ST7735_OutString("******HOGCHASE******");
	ST7735_OutString("\n\nMenu:\n 1: Level 1\n 2: Level 2\n 3: Level 3\n Joystick: Info");
	ST7735_OutString("\n\nTotal Score: ");
	LCD_OutDec(totalScore);
};
void displayInstructions(){
	int s = 0;//instruction screens
	int input = 0;
	//Screen 0:
	char first[] = "Your house has been\nblown down by the\nbig, bad wolf!\n\n<1:Menu, 2:Next>";
	//Screen 1:
	char second[] = "Navigate the pig\nwith the joystick to\ncollect materials\nfor a new house.\n\nDuring play, press\nany button to pause.\n\n<1:Back, 2:Next>";
	//Screen 2:
	char third[] = "You have 3 lives to\ncomplete each level.\nLevels are completed\nonce you obtain 5\nitems. Running into \nwolves and holes\nwill cost a life.\nYou will also lose a\nlife every 1 min.\n\n<1:Back, 2:Next>";
	//Screen 3:
	char fourth[] = "Points:\nStraw (Level 1) =\n25 points\nTwigs (Level 2) =\n50 points\nBrick (Level 3) =\n75 points\nTime Bonus =\n1 point/sec left\nLives Bonus =\n60 points/life left\n\n<1:Back, 2:Menu>";
	typedef struct infoFSM{
		char *output;
		int next[2];
	}info;
	info Screens[4] = {
		{first,{-1,1}}, //0
		{second,{0,2}},//1
		{third,{1,3}}, //2
		{fourth,{2,4}} //3
	};
	while (s!=-1&&s!=4){
		//output
		ST7735_FillScreen(0); 
		ST7735_SetCursor(0,0);
		ST7735_OutString(Screens[s].output);
		//input
		while(input == 0){input = Switch_In();}
		delayTouch();
		if(input == 1||input == 2){
			input = input - 1;//adjust for button numbering vs indices mismatch
			s = Screens[s].next[input];
		}
		input = 0;
		delay(100);//prevent bounce
	}
};

void displaylevelwin(int time, int lives, int score){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("You Win! \n\nRaw Score: ");
	LCD_OutDec(score);
	ST7735_OutString("\nTime Bonus: ");
	LCD_OutDec(time);
	ST7735_OutString("\nLives Bonus: ");
	LCD_OutDec(lives*60);
	ST7735_OutString("\n\nLevel Score: ");
	LCD_OutDec(score+time+lives*60);
	ST7735_OutString("\n\nPress any button\nto return to menu.");
};
void displayoverallwin(void){
};
void displaylose(void){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("Sorry, you lost :(.\n\nPress any button\nto return to menu.");
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
	ST7735_OutString("\n\nPress any button\nto continue");
};
void displaypause(int time, int lives, int score){
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString("PAUSED\nLives Left: ");
	LCD_OutDec(lives);
	ST7735_OutString("\nTime Left (sec): ");
	LCD_OutDec(time/30);
	ST7735_OutString("\nCurrent Score: ");
	LCD_OutDec(score);
	ST7735_OutString("\n\nPress:\n 1: Resume Game\n 2: Quit\n    (Lose progress)");
};

