// PlayGame.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

#include <stdint.h>
#include "random.h"
#include "Switch.h"
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include <math.h>
#include "Graphics.h"
#include "Display.h"
#include "Sound.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
/*===================================================================================================================================
CONSTANTS, TYPEDEFS (ENUMS, STRUCTS)
=====================================================================================================================================*/
#define WOLVES_SIZE 3
#define FLAGS_TOTAL 5
typedef enum {NORTH,EAST,SOUTH,WEST,NONE} direction_t;
typedef enum {PAUSE,RESUME,QUIT} buttons;
/*typedef enum {
	O,//open path
	X,//closed path
	A,//apple
	//S,//straw
	//T,//twigs
	//B,//bricks
	P,//pig
	W//wolf
}mapIcons;*/
typedef struct actor{
	int r;// r = -1 when actor doesn't exist
	int c;
	int speed;
	direction_t direction;
}actorType;
typedef struct flag{
	int r; // r = -1 when flag has been collected
	int c;
}flagType;
/*===================================================================================================================================
DECLARE VARIABLES
=====================================================================================================================================*/
actorType pig; 
actorType Wolves[WOLVES_SIZE];
flagType Flags[FLAGS_TOTAL];
int lives;
int winlose;
int flagcount;
int time;
int score;
/*===================================================================================================================================
INITIALIZATION FUNCTIONS (and helpers)
=====================================================================================================================================*/
void initMap(mapIcons map[ROW][COL], int diff){
	//map[0][0] is the top left tile on the screen.
	//map[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	int i, j, w, f;
	w = 0;
	f = 0;
	if (lives ==3&&diff == 1){
		mapIcons map1[ROW][COL]= { 
			{P,O,O,X,X,O,O,A},
			{O,X,O,X,X,O,X,O},
			{O,X,O,X,X,O,X,O},
			{O,O,O,O,O,O,O,O},
			{O,O,X,X,X,X,O,O},
			{O,O,O,O,O,O,O,O},
			{X,X,X,O,O,X,X,X},
			{A,O,O,O,O,O,O,A},
			{O,X,O,X,X,O,X,O},
			{O,O,A,X,X,A,O,O}
		};
		int k,l;
		for (k = 0; k < ROW; k++){
			for (l = 0; l < COL; l++){
				map[k][l] = map1[k][l];
			}	
		}
	}//level1
	if (lives ==3&&diff == 2){
		mapIcons map2[ROW][COL]={
			{P,O,O,X,X,O,O,A},
			{O,X,O,X,X,O,X,O},
			{O,X,O,X,X,O,X,O},
			{O,O,O,O,O,O,O,O},
			{O,O,X,X,X,X,O,O},
			{O,O,O,O,O,O,O,O},
			{X,X,X,O,O,X,X,X},
			{A,O,O,O,O,O,O,A},
			{O,X,O,X,X,O,X,O},
			{O,O,A,X,X,A,O,W}
		};
		int k,l;
		for (k = 0; k < ROW; k++){
			for (l = 0; l < COL; l++){
				map[k][l] = map2[k][l];
			}	
		}
	}//level2
	if (lives ==3&&diff == 3){
		mapIcons map3[ROW][COL]= { 
			{A,O,O,O,O,O,P,O},
			{O,X,X,O,O,X,X,O},
			{O,X,O,O,O,O,X,O},
			{O,O,O,X,X,O,A,O},
			{O,O,X,A,W,X,O,O},
			{X,O,X,O,O,X,O,X},
			{O,O,X,O,O,X,O,O},
			{O,O,O,O,O,W,O,O},
			{O,O,X,O,O,O,X,O},
			{X,A,O,O,X,O,A,O}
		};
		int k,l;
		for (k = 0; k < ROW; k++){
			for (l = 0; l < COL; l++){
				map[k][l] = map3[k][l];
			}	
		}
	}//level3
	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
				if (map[i][j] == X)
				{
					//draw Wall bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
					ST7735_DrawBitmap(j*16,(i+1)*16,wall,16,16);
				}
				else if (map[i][j] == A)
				{
					//draw apple bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
					Flags[f].r = i*16;
					Flags[f].c = j*16;
					f++;
				}
				else if (map[i][j] == H)
				{
					ST7735_DrawBitmap(j*16,(i+1)*16,hole,16,16);
				}
				else if (map[i][j] == P)
				{
					pig.r = i*16;
					pig.c = j*16;
					pig.speed = 2;
					pig.direction = NORTH;
				}
				else if (map[i][j] == W)
				{
					Wolves[w].r = i*16;
					Wolves[w].c = j*16;
					Wolves[w].speed = 1;
					Wolves[w].direction = NORTH;
					w++;
				}
				else 
				{
					//draw air ("grass")
					ST7735_DrawBitmap(j*16,(i+1)*16,grass,16,16);
				}
			}//inner for
	}//outer for
	for (int i = w; i<WOLVES_SIZE;i++){
		Wolves[i].r = -1;
	}//Wolves that don't exist
	for (int i = f; i<FLAGS_TOTAL;i++){
		Flags[i].r = -1;
	}//flags that don't exist
	time = 5400; //3 min to play each round
}//Only call this function to initialize map during levels and between deaths

void displayActors(){
	//map[0][0] is the top left tile on the screen.
	//map[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	for (int i = 0; i<FLAGS_TOTAL; i++){
		if (Flags[i].r!=-1){
			ST7735_DrawBitmap(Flags[i].c,Flags[i].r+16,apple,16,16);
		}
	}
	ST7735_DrawBitmap(pig.c,pig.r+16,pigFront,16,16);
	for (int i = 0; i<WOLVES_SIZE; i++){
		if (Wolves[i].r!=-1){
			ST7735_DrawBitmap(Wolves[i].c,Wolves[i].r+16,wolf,16,16);
		}
	}
}

/*===================================================================================================================================
GAME FUNCTIONS
=====================================================================================================================================*/
int checkCanMove(direction_t dir, actorType *a, mapIcons map[ROW][COL]){
	//0 = can't move, 1 = can move
	int actr, actr2, actc, actc2;
			int tempr, tempc;
			switch(dir){
			case NORTH:
				tempr = (a->r) - (a->speed);
				tempc = (a->c);
				break;
			case EAST:
				tempr = (a->r);
				tempc = (a->c) + (a->speed);
				break;
			case SOUTH:
				tempr = (a->r) + (a->speed);
				tempc = (a->c);
				break;
			case WEST:
				tempr = (a->r);
				tempc = (a->c) - (a->speed);
				break;
			case NONE: 
				tempr = (a->r);
				tempc = (a->c);
			break;
		}//switch
		actr = (tempr)/ 16;
		if ((tempr) % 16 != 0) {
			actr2 = actr + 1;
		}
		else { 
			actr2 = actr; 
		}
		actc = (tempc) / 16;
		if ((tempc) % 16 != 0) {
			actc2 = actc + 1;
		}
		else { 
			actc2 = actc;
		}
	if (map[actr][actc] == X || map[actr2][actc] == X || map[actr][actc2] == X || map[actr2][actc2] == X) {
		return 0;
	}
	int pigtop	= tempr;
 	int pigbot = tempr+15;
	int pigr = tempc+15;
  int pigl =  tempc;
	if(160<pigbot||0>pigtop||128<pigr||0>pigl){
		return 0;
	}
	return 1;	//can move
}

void move(actorType *p, mapIcons map[ROW][COL]){
		switch(p->direction){
			case NORTH:
				(*p).r -= (*p).speed;
				break;
			case EAST:
				(*p).c += (*p).speed;
				break;
			case SOUTH:
				(*p).r += (*p).speed;
				break;
			case WEST:
				(*p).c -= (*p).speed;
				break;
			case NONE:break;
		}//switch
}//move
direction_t autoDir(actorType *a, mapIcons map[ROW][COL]){
	if (checkCanMove((*a).direction,a,map)){return pig.direction;}
	if(checkCanMove(EAST,a,map)){return EAST;}
	if(checkCanMove(WEST,a,map)){return WEST;}
	if(checkCanMove(SOUTH,a,map)){return SOUTH;}
	if(checkCanMove(NORTH,a,map)){return NORTH;}
	return NONE;
}
direction_t findbestpath(actorType *wolf, mapIcons map[ROW][COL]){
	direction_t best;
	int e = checkCanMove(EAST,wolf,map);
	int w = checkCanMove(WEST,wolf,map);
	int s = checkCanMove(SOUTH,wolf,map);
	int n = checkCanMove(NORTH,wolf,map);
	if((pig.c > (*wolf).c)&&e){
			best = EAST;
	}
	else if((pig.c < (*wolf).c)&&w){
			best = WEST;
	}
	else if((pig.r > (*wolf).r)&&s){
			best = SOUTH;
	}
	else if ((pig.r < (*wolf).r)&&n){
			best = NORTH;	
	}
	else 
		best = autoDir(wolf,map);
	return best;
}
void moveEnemy(actorType *wolf, mapIcons map[ROW][COL]){
	(*wolf).direction = findbestpath(wolf, map);
	move(wolf, map);
}//moveEnemy
void collideWolves(actorType *wolf, mapIcons map[ROW][COL],int diff){
	int wolftop = wolf->r;
	int wolfbot = wolf->r+15;
	int wolfr = wolf ->c+15;
	int wolfl = wolf->c;
	int pigtop	= pig.r;
 	int pigbot = pig.r+15;
	int pigr = pig.c+15;
  int pigl =  pig.c;
	if(wolftop<=pigbot&&wolfbot>=pigtop&&wolfr>=pigl&&wolfl<=pigr)
	{
		lives--;
		Sound_Chomp();
		displayLives(lives,time); 
		while(Switch_In() == 0){}
		initMap(map,diff);
	}
}

void collideFlags(mapIcons map[ROW][COL]){
	int pigr = pig.r / 16;
	int pigr2;
	if (pig.r % 16 != 0) {
		pigr2 = pigr + 1;
	}
	else { 
		pigr2 = pigr; 
	}
	
	int pigc = pig.c / 16;
	int pigc2;
	if (pig.c % 16 != 0) {
		pigc2 = pigc + 1;
	}
	else { 
		pigc2 = pigc; 
	}
	
	if (map[pigr][pigc] == A) {
		flagcount++;
		score = score+10;//increment score based on flags
		map[pigr][pigc] = O;
		ST7735_DrawBitmap(pigc*16,pigr*16+16,grass,16,16); 
		for (int k = 0; k<FLAGS_TOTAL;k++){
			if ((pigc == Flags[k].c/16)&&(pigr == Flags[k].r/16))
				{
					Flags[k].r = -1;
				}
			}
		Sound_Flag();
	}
	if (map[pigr2][pigc] == A) {
		flagcount++;
		score = score+10;//increment score based on flags
		map[pigr2][pigc] = O;
		ST7735_DrawBitmap(pigc*16,pigr2*16+16,grass,16,16); 
		for (int k = 0; k<FLAGS_TOTAL;k++){
			if ((pigc == Flags[k].c/16)&&(pigr2 == Flags[k].r/16))
				{
					Flags[k].r = -1;
				}
			}
		Sound_Flag();
	}
	if (map[pigr][pigc2] == A) {
		flagcount++;
		score = score+10;//increment score based on flags
		map[pigr][pigc2] = O;
		ST7735_DrawBitmap(pigc2*16,pigr*16+16,grass,16,16); 
		for (int k = 0; k<FLAGS_TOTAL;k++){
			if ((pigc2 == Flags[k].c/16)&&(pigr == Flags[k].r/16))
				{
					Flags[k].r = -1;
				}
			}
		Sound_Flag();
	}
	if (map[pigr2][pigc2] == A) {
		flagcount++;
		score = score+10;//increment score based on flags
		map[pigr2][pigc2] = O;
		ST7735_DrawBitmap(pigc2*16,pigr2*16+16,grass,16,16); 
		for (int k = 0; k<FLAGS_TOTAL;k++){
			if ((pigc2 == Flags[k].c/16)&&(pigr2 == Flags[k].r/16))
				{
					Flags[k].r = -1;
				}
			}
		Sound_Flag();
	}
}

void updateradar(){
	//update the radar!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if flag[i] == -1, then do not display
}
uint32_t ADCdata[2];
char status;
void SysTick_Init(void){
	//SYSTICK Initialization
	NVIC_ST_CTRL_R = 0;                   											// disable SysTick during setup
  NVIC_ST_RELOAD_R = 2666667;  																// reload value for interrupts at 30 Hz (0.03 s)
  NVIC_ST_CURRENT_R = 0;                											// any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;  // priority 1   
  NVIC_ST_CTRL_R = 0x0007;																		// enable SysTick with core clock and interrupts
	status = 0;
}
void SysTick_Handler(void){// every 33 ms
	ADC_In89(ADCdata);
	status = 1;
	time--;
}

direction_t Convert(uint32_t input[2]){
	//data[0];//North/South (vert - PE5)
  //data[1];//East/West (horz - PE4)
	direction_t output = NONE;
	if (4096>input[0]&&input[0]>=3500){
		output = NORTH;
	}
	else if (500>=input[0]&&input[0]>0){
		output = SOUTH;
	}
	else if (500>=input[1]&&input[1]>0){
		output = EAST;
	}
	else if (4096>input[1]&&input[1]>=3500){
		output = WEST;
	}
  return output;
}

int playgame(int difficulty){
		lives = 3;
		winlose = -1;//0 = lose, 1 = win
		flagcount = 0;
		score = 0;
		mapIcons map[ROW][COL];
		initMap(map,difficulty);
	  Sound_Init();
		SysTick_Init();
		EnableInterrupts();
		while (winlose==-1){
			if(status){
			displayActors();
			//displayradar();
			//userinput
			pig.direction = Convert(ADCdata);
			if (checkCanMove(pig.direction, &pig, map)){
				move(&pig, map);
			}
			for (int i = 0; i<WOLVES_SIZE; i++){
				if(Wolves[i].r!=-1){
					moveEnemy(&Wolves[i], map);
					collideWolves(&Wolves[i],map,difficulty);
				}
			}
			collideFlags(map);
			//updateradar();
			if(time == 0){
				lives--;
				Sound_Chomp();
				displayLives(lives,time); 
				while(Switch_In() == 0){}
				initMap(map,difficulty);
			}
			if(lives == 0){
				winlose = 0;
				displaylose(); 
				while(Switch_In() == 0){}
				return winlose;
			}
			if (flagcount == FLAGS_TOTAL){
				displaylevelwin(time/30, lives, score); 
				score = score + time/30 + lives*100;
				winlose = score;
				while(Switch_In() == 0){} 
				return winlose;
			}
			status = 0;
		}//if
	}//while
	return -1;
}

