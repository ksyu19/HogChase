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
#include "Timer1.h"
#include "tm4c123gh6pm.h"
#include "Sound.h"
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

//house
const unsigned short storyhouse[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x53D7, 0x53D7, 0x53D7, 0x53D7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x53D7, 0x53D7, 0x53D7, 0x079F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x53D7, 0x53D7, 0x53D7, 0x53D7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x53D7, 0x53D7, 0x53D7, 0x53D7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x53D7, 0x53D7, 0x53D7, 0x53D7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x20FD, 0x20FD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20FD, 0x20FD, 0x0000,
 0x0000, 0x20FD, 0x20FD, 0x20FD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20FD, 0x20FD, 0x20FD, 0x0000,
 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0xFFFF, 0xFFFF, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

const unsigned short storywolf[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0xB596, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0xBDD7, 0xBDD7, 0xBDD7, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0xBDD7, 0xEF7D, 0xBDD7, 0xBDD7, 0xB596, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x7BEF, 0xEF3C, 0xFFFF, 0x18A2, 0xEF5D, 0x18A2, 0xBDD7, 0xFFFF, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7BEF, 0xBDF7, 0xBDD7, 0xFFFF, 0x62EB, 0xEF5D, 0x62EB, 0xBDD7, 0xFFDF, 0xC618, 0x7BEF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x7BEF, 0xEF5D, 0xBDD7, 0xFFFF, 0xFFFF, 0xC617, 0xEF5D, 0xBDD7, 0xEF5D, 0xFFFF, 0xBDD7, 0xFFFF, 0x7BEF, 0x0000, 0x0000,
 0x0000, 0xE73C, 0xFFDF, 0xEF5D, 0x0000, 0x0000, 0xEF3C, 0xEF5D, 0xEF3C, 0x0000, 0x0000, 0xEF5D, 0xBDD7, 0xD6BA, 0x0000, 0x0000,
 0x0000, 0x7BEF, 0x18E3, 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x2104, 0x7BEF, 0x0000, 0x0000,
 0x0000, 0xBDF7, 0xEF5D, 0x18A2, 0x18C3, 0xFFFF, 0xEF5D, 0xEF5D, 0xEF5D, 0xFFFF, 0x0000, 0x0000, 0xEF5D, 0xBDD7, 0x0000, 0x0000,
 0x0000, 0x7BEF, 0xFFFF, 0xEF5D, 0xEF5D, 0xEF5D, 0xF79E, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xE73C, 0xEF5D, 0x7BEF, 0x0000, 0x0000,
 0x0000, 0x7BEF, 0xBDD7, 0xEF5D, 0xEF5D, 0xBDD6, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xEF5D, 0xC5F7, 0x7BEF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7BEF, 0x20E3, 0x7BEF, 0xC5F7, 0xEF5D, 0xBDD6, 0xEF5D, 0xC5F7, 0x7BEF, 0x20E3, 0x7BEF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7BEF, 0xEF5D, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0xE73C, 0x7BEF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};

//Pig bitmap (16x16 px) "P"
const unsigned short storypig[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x4A8A, 0x4A8A, 0x4A8A, 0x62F3, 0x4A8A, 0x4A8A, 0x4A8A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x4A8A, 0x1085, 0x422D, 0x7BF7, 0x2127, 0x1085, 0x4A8A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x62D4, 0x7BF7, 0x7BF7, 0xAD5F, 0xA51E, 0xAD7F, 0x7BD6, 0x7BB7, 0x630F, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7BB8, 0xB59F, 0xAD5F, 0xAD7F, 0xB59F, 0xB59F, 0xB59F, 0xB59F, 0xAD7F, 0x73B6, 0x5A51, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xB57F, 0xAD5F, 0xAD7F, 0x9CFC, 0x528F, 0x528F, 0x52B0, 0x6B73, 0xAD7F, 0xA53E, 0x6B15, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xB59F, 0xAD7F, 0x8C79, 0x5AF1, 0x7BF5, 0x94BA, 0x7393, 0x7BD6, 0xAD7F, 0xB59F, 0x83F9, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xB59F, 0x8416, 0xB59F, 0xAD7F, 0x5ACF, 0xBDDF, 0x6311, 0x8C57, 0x6B53, 0x8C79, 0xA53F, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xB57F, 0x7BF6, 0xAD7F, 0xAD7F, 0x9499, 0x73B5, 0x9498, 0xAD7F, 0x9CFD, 0x6B74, 0x83F7, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x7B93, 0x8C7A, 0xAD7F, 0xFFFD, 0x0000, 0xB5BF, 0xFFFD, 0x0000, 0xAD7F, 0x6B53, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x94F5, 0xAD5F, 0xFFFD, 0xFFFF, 0x949B, 0xFFFD, 0xFFFD, 0xAD5F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x9CBB, 0xA53F, 0xA51E, 0xAD7F, 0x949A, 0x8C59, 0x83F6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xBD9F, 0x8436, 0x947A, 0xAD7F, 0xAD5F, 0xB57F, 0xBDBF, 0xB59F, 0xA4DA, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,


};






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
	char fourth[] = "Points:\nStraw (Level 1) =\n25 points\nTwigs (Level 2) =\n50 points\nBrick (Level 3) =\n75 points\nTime Bonus =\n1 point/sec left\nLives Bonus =\n60 points/life left\n\nReach 1500 points:\nBuild a house!\n\n<1:Back, 2:Menu>";
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
int pigc, pigr, wolfc, wolfr, mail;
void moveStoryWin(void){
	pigc++;
	wolfr++;
	mail = 1;
}
void displayStoryWin(void){
	pigc = 0;
	pigr = 7*16;
	wolfc = 5*16;
	wolfr = 0;
	mail = 0;
	ST7735_FillScreen(0);            // set screen to black
	ST7735_DrawBitmap(5*16,7*16+16,storyhouse,16,16); 
	Timer1_Init(moveStoryWin, 80000000/30);
	EnableInterrupts();
	while(pigc!=5*16){
		if(mail){
			ST7735_DrawBitmap(pigc,pigr+16,storypig,16,16); 
			ST7735_DrawBitmap(wolfc,wolfr+16,storywolf,16,16); 
			ST7735_DrawBitmap(5*16,7*16+16,storyhouse,16,16); 
			mail = 0;
		}
	}
	NVIC_DIS0_R = 1<<19;           //disable interrupts IRQ 19 in NVIC 
	delay(500);
	ST7735_FillScreen(0);            // set screen to black
	ST7735_SetCursor(0,5);
	ST7735_OutString("You made it!!!\nPress any button\nto return to menu");
	int input = 0;
	while(input == 0){input = Switch_In();}
	delayTouch();
};
int house;
void moveStoryIntro(void){
	wolfr++;
	if (wolfr == 2*16-16){
		house = 2;
		Sound_Chomp();
		pigr = pigr+16;
	}
	if (wolfr == 4*16-16){
		house = 3;
		Sound_Chomp();
		pigr = pigr+16;
	}
	if (wolfr == 6*16-16){
		house = 4;
		Sound_Chomp();
		pigr = pigr+16;
	}
	if (wolfr >= 2*16-16){
		if(pigr<4*16){
			pigr=pigr+1;
		}
		else if(wolfr >= 4*16-16){
			if(pigr<6*16){
				pigr=pigr+1;
			}
			else if(wolfr >= 6*16-16&&pigr<10*16){
				pigr=pigr+1;
			}
		}
	}
	mail = 1;
}
void movePigIntro(void){
	pigr=pigr+2;
	mail = 1;
}
void drawHousesIntro(void){
	int x = 3*16+8;
	switch(house){
		case 1:
			ST7735_DrawBitmap(x,2*16+16,storyhouse,16,16); 
			ST7735_DrawBitmap(x,4*16+16,storyhouse,16,16); 
			ST7735_DrawBitmap(x,6*16+16,storyhouse,16,16); 
		break;
		case 2:
			ST7735_DrawBitmap(x,4*16+16,storyhouse,16,16); 
			ST7735_DrawBitmap(x,6*16+16,storyhouse,16,16); 
		break;
		case 3:
			ST7735_DrawBitmap(x,6*16+16,storyhouse,16,16); 
		break;
		case 4:break;
	}
}
void displayStoryIntro(void){
	Sound_Init();
	pigc = 3*16+8;
	pigr = 2*16;
	wolfc = 3*16+8;
	wolfr = 0;
	mail = 0;
	house = 1;
	ST7735_FillScreen(0);            // set screen to black
	drawHousesIntro();
	Timer1_Init(moveStoryIntro, 80000000/30);
	EnableInterrupts();
	while(wolfr!=10*16){//off the screen
		if(mail){
			ST7735_DrawBitmap(pigc,pigr+16,storypig,16,16); 
			ST7735_DrawBitmap(wolfc,wolfr+16,storywolf,16,16); 
			drawHousesIntro();
			mail = 0;
		}
	}
	NVIC_DIS0_R = 1<<19;           //disable interrupts IRQ 19 in NVIC 

	delay(500);
	ST7735_FillScreen(0);            // set screen to black
	pigr = 0;
	mail = 0;
	Timer1_Init(movePigIntro, 80000000/25);	
		while(pigr<6*16){//middle of screen
		if(mail){
			ST7735_DrawBitmap(pigc,pigr+16,storypig,16,16); 
			mail = 0;
		}
	}
	ST7735_SetCursor(0,0);
	ST7735_OutString("WELCOME TO HOGCHASE\n\nCollect materials to\nbuild a new house!\n\nPress any button\nto continue");
	int input = 0;
	while(input == 0){input = Switch_In();}
	delayTouch();
};
