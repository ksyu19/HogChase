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

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
/*===================================================================================================================================
CONSTANTS, TYPEDEFS (ENUMS, STRUCTS)
=====================================================================================================================================*/
#define WOLVES_SIZE 3 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later (might depend on difficulty)
#define FLAGS_TOTAL 5 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later
typedef enum {NORTH,EAST,SOUTH,WEST,NONE} direction_t;
typedef enum {PAUSE,RESUME,QUIT} buttons;//implement later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*typedef enum {
	O,//open path
	C,//closed path
	A,//apple
	//S,//straw
	//T,//twigs
	//B,//bricks
	P,//pig
	W//wolf
}mapIcons;*/
typedef struct actor{
	int x;// x = -1 when actor doesn't exist
	int y;
	int speed;
	float radius;
	direction_t direction;
}actorType;
typedef struct flag{
	int x; // x = -1 when flag has been collected
	int y;
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
//Level 3 map:
mapIcons map1[10][8];
mapIcons map2[10][8];
mapIcons map3[10][8] ={
	{C,C,C,C,C,C,C,C},
	{C,O,O,O,O,O,O,C},
	{C,C,C,O,C,C,C,C},
	{C,O,C,O,O,O,O,C},
	{C,O,O,O,C,C,O,C},
	{C,O,C,O,O,O,O,C},
	{C,O,O,O,O,O,C,C},
	{C,O,C,C,O,O,O,C},
	{C,O,O,O,O,C,O,C},
	{C,C,C,C,C,C,C,C},
};
/*===================================================================================================================================
VECTOR FUNCTIONS: These are so that we can later move our sprites when we interface with the joystick.
=================================================================================================================================*/
typedef struct 
{
	uint16_t x, y; 
	
}vector2;
vector2 addVect(vector2 v1, vector2 v2)
{
	vector2 v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	return v3;
}
vector2 subVect(vector2 v1, vector2 v2)
{
	vector2 v3;
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	return v3;
}
void initVect (vector2 *vect, float x, float y)
{
	vect->x = x;
	vect->y = y;
}

float magnitude (vector2 *v)
{
	return (sqrt(v->x*v->x + v->y*v->y));
}	

/*===================================================================================================================================
INITIALIZATION FUNCTIONS (and helpers)
=====================================================================================================================================*/
/*void flagInit(int diff){
	int m;
	for (int i = 0; i < FLAGS_TOTAL; i++){
		switch(diff){
			case 1:		
				Flags[0].x = 1*16;
				Flags[0].y = 1*16;
				Flags[1].x = 6*16;
				Flags[1].y = 1*16;
				for (int i = 0; i< FLAGS_TOTAL;i++){
					if(Flags[i].x != -1){
						map1[Flags[i].y/16][Flags[i].x/16] = A;//straw flags for level 1
					}
				}//flags
				do {
					m = (Random()>>21)%80; //random number from 0 to 79  (%1280 to get random number from 0 to 1279)
				}while((map1[m/40][m%40]!=O));
				Flags[i].x = m%40*16;//column
				Flags[i].y = m/40*16;//row
			break;
			case 2:
				do {
					m = (Random()>>21)%80; //random number from 0 to 79
				}while((map2[m/40][m%40]!=O));
				Flags[i].x = m%40;//column
				Flags[i].y = m/40;//row
			break;
			case 3:
			do {
					m = (Random()>>21)%80; //random number from 0 to 79
				}while((map3[m/40][m%40]!=O));
				Flags[i].x = m%40;//column
				Flags[i].y = m/40;//row
			break;
		}//switch
	}//for
}//flagInit*/
void initMap(mapIcons map[10][8], int diff){
	//map[0][0] is the top left tile on the screen.
	//map[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	int i, j, w, f;
	w = 0;
	f = 0;
	if (lives ==3&&diff == 1){
		mapIcons mapArray[10][8] ={
			{C,C,C,C,C,C,C,C},
			{C,A,O,O,O,O,A,C},
			{C,C,C,O,C,C,C,C},
			{C,O,C,A,O,O,O,C},
			{C,O,O,O,C,C,O,C},
			{C,O,C,P,O,O,A,C},
			{C,O,O,O,O,O,C,C},
			{C,O,C,C,O,O,O,C},
			{C,A,O,O,O,C,W,C},
			{C,C,C,C,C,C,C,C},
		};
		int k,l;
		 for (k = 0; k < 10; k++){
			for (l = 0; l < 8; l++){
				map[k][l] = mapArray[k][l];
			}
		}
	}//start of the game
	if (lives ==3&&diff == 2){
		mapIcons mapArray[10][8] ={
			{C,C,C,C,C,C,C,C},
			{C,A,O,O,O,O,A,C},
			{C,C,C,O,C,C,C,C},
			{C,O,C,A,O,O,O,C},
			{C,O,O,O,C,C,O,C},
			{C,O,C,P,C,O,A,C},
			{C,O,O,O,O,O,C,C},
			{C,O,C,C,O,O,O,C},
			{C,A,O,O,W,C,W,C},
			{C,C,C,C,C,C,C,C},
		};
		int k,l;
		 for (k = 0; k < 10; k++){
			for (l = 0; l < 8; l++){
				map[k][l] = mapArray[k][l];
			}
		}
	}//start of the game
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 8; j++)
			{
				if (map[i][j] == C)
				{
					//draw Wall bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
					ST7735_DrawBitmap(j*16,(i)*16,wall,16,16);
				}
				else if (map[i][j] == A)
				{
					//draw apple bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
					ST7735_DrawBitmap(j*16,(i)*16,apple,16,16); 
					Flags[w].x = i*16;
					Flags[w].y = j*16;
					f++;
				}
				else if (map[i][j] == H)
				{
					ST7735_DrawBitmap(j*16,(i)*16,hole,16,16);
				}
				else if (map[i][j] == P)
				{
					ST7735_DrawBitmap(j*16,(i)*16,pigFront,16,16);
					pig.x = i*16;
					pig.y = j*16;
					pig.speed = 1;
					pig.direction = NORTH;
				}
				else if (map[i][j] == W)
				{
					ST7735_DrawBitmap(j*16,(i)*16,wolf,16,16);
					Wolves[w].x = i*16;
					Wolves[w].y = j*16;
					Wolves[w].speed = 1;
					Wolves[w].direction = NORTH;
					w++;
				}
				else 
				{
					//draw air ("grass")
					ST7735_DrawBitmap(j*16,(i)*16,grass,16,16);
				}
			}//inner for
	}//outer for
	for (int i = w; i<WOLVES_SIZE;i++){
		Wolves[i].x = -1;
	}//Wolves that don't exist
	for (int i = f; i<WOLVES_SIZE;i++){
		Flags[f].x = -1;
	}//flags that don't exist
	time = 5400; //3 min to play each round
}//Only call this function to initialize map during levels and between deaths
void displayActors(){
	//map[0][0] is the top left tile on the screen.
	//map[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	ST7735_DrawBitmap(pig.y,pig.x,pigFront,16,16);
	for (int i = 0; i<WOLVES_SIZE; i++){
		if (Wolves[i].x!=-1){
			ST7735_DrawBitmap(Wolves[i].y,Wolves[i].x,wolf,16,16);
		}
	}
}

/*===================================================================================================================================
GAME FUNCTIONS
=====================================================================================================================================*/
int checkCanMove(direction_t dir, actorType *a, mapIcons map[10][8]){
	int move = 0;//0 = can't move, 1 = can move
	int loc;
			switch(dir){
			case NORTH:
				loc = (*a).y-(*a).speed;
				if (map[loc][(*a).x] != C)
					move = 1;
				break;
			case EAST:
				loc = (*a).x + (*a).speed;
				if (map[(*a).y][loc] != C)
					move = 1;
				break;
			case SOUTH:
				loc = (*a).y+(*a).speed;
				if (map[loc][(*a).x] != C)
					move = 1;
				break;
			case WEST:
				loc = (*a).x - (*a).speed;
				if (map[(*a).y][loc] != C)
					move = 1;
				break;
			case NONE: break;
		}//level 1
	return move;	
}

void move(actorType *p, mapIcons map[10][8]){
	if (checkCanMove(p->direction, p, map)){	
		switch(p->direction){
			case NORTH:
				(*p).y -= (*p).speed;
				break;
			case EAST:
				(*p).x += (*p).speed;
				break;
			case SOUTH:
				(*p).y += (*p).speed;
				break;
			case WEST:
				(*p).x -= (*p).speed;
				break;
			case NONE:break;
		}//switch
	}
}//move

direction_t findbestpath(actorType *wolf, mapIcons map[10][8]){
	direction_t best;
	int e = checkCanMove(EAST,wolf,map);
	int w = checkCanMove(WEST,wolf,map);
	int s = checkCanMove(SOUTH,wolf,map);
	int n = checkCanMove(NORTH,wolf,map);
	if(pig.x > (*wolf).x){
		if(e){
			best = EAST;
		}
	}
	else if(pig.x < (*wolf).x){
		if(w){
			best = WEST;
		}
	}
	else if(pig.y > (*wolf).y){
		if(s){
			best = SOUTH;
		}	
	}
	else if (pig.y < (*wolf).y){
		if(n){
			best = NORTH;
		}	
	}
	else if (checkCanMove((*wolf).direction,wolf,map)){
		best = (*wolf).direction;
	}
	else if(e){
			best = EAST;
		}
	else if(w){
			best = WEST;
		}
	else if(s){
			best = SOUTH;
		}
	else if(n){
			best = NORTH;
	}
	//adjust later for better algorithm!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return best;
}
void moveEnemy(actorType *wolf, mapIcons map[10][8]){
	(*wolf).direction = findbestpath(wolf, map);
	move(wolf, map);
}//moveEnemy

void collideWolves(actorType *wolf, mapIcons map[10][8],int diff){
	if((pig.x == wolf->x&&pig.y == wolf->y))
	{
		lives--;
		initMap(map,diff);
		//PlayGameInit(diff);//reset actors and time
	}//rough collision detection with wolves -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1fix later, maybe add rocks/obstacles
}
//void collideFlags(flagType *f){
/*======================================================================
COLLISION DETECTION
======================================================================*/
int collideWithHole(actorType pig, mapIcons levelData[row][col])
{
	pig.radius = 3;
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = pig.y / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if top collides
	
	i = (pig.y - pig.radius) / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (pig.y + pig.radius) / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if left collides
	i = pig.x/ 16;
	j = (pig.x - pig.radius)/ 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if right collides
	i = pig.y / 16;
	j = (pig.x + pig.radius)/ 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	return 0;
};

//-----------------------------------------------
int collideWithApple(actorType pig, mapIcons levelData[row][col])
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = pig.y / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if top collides
	
	i = (pig.y - pig.radius) / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (pig.y + pig.radius) / 16;
	j = pig.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if left collides
	i = pig.x/ 16;
	j = (pig.x - pig.radius)/ 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if right collides
	i = pig.y / 16;
	j = (pig.x + pig.radius)/ 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	return 0;
}

//---------------------------------------------------------
short entitiesColliding(actorType pig, actorType Wolves[WOLVES_SIZE])
{
	vector2 distance;
	for (int i = 0; i < WOLVES_SIZE ; i++)
	{
		distance.x = abs(pig.x -  Wolves[i].x);
		distance.y = abs(pig.y -  Wolves[i].y);
		
		if (magnitude(&distance) < pig.radius + Wolves[i].radius)
		{
			return 1;
		}
	}	
	return 0;
}
//---------------------------------------------------------
short collideWithWolf (game *G)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			if (entitiesColliding(&G->player, &G->enemy[i])== 1)
			{
				return 1;
			}
		}
		
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
void handleCollisions(game *G)
{
	if (collideWithApple(&G->player, G->levelData))
	{
		G->levelData[(int)(G->player.position.y/16)][(int)(G->player.position.x/16)] = O;
		G->score = G->score+50;
	}
	else if (collideWithWolf(G)||(collideWithHole(&G->player,  G->levelData)))
	{
		G->lives--;
		if (G->lives > 0 )
		{
		respawn (G);
		}
		else
		{
			//This is game over.
		}
	}
}

/*if(f->x!=-1){
		if((pig.x==f->x)&&(pig.y==f->y)){
			flagcount++;
			score = score+10;//increment score based on flags
			f->x = -1;
		}
	}
}
void updatemap(int diff){
	switch(diff){
		case 1:
			for (int i = 0; i< FLAGS_TOTAL;i++){
				if(Flags[i].x != -1){
					map1[Flags[i].y][Flags[i].x] = A;//straw flags for level 1
				}
			}//flags
			map1[(pig.y)/16][(pig.x)/16] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				if (Wolves[i].x!=-1){
					map1[(Wolves[i].y)/16][(Wolves[i].x)/16] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
				}
			}//wolves
		break;
		case 2:
			for (int i = 0; i< FLAGS_TOTAL;i++){
				if(Flags[i].x != -1){
					map2[Flags[i].y][Flags[i].x] = A;//twig flags for level 2
				}
			}//flags
			map2[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				if (Wolves[i].x!=-1){
					map2[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
				}
			}//wolves
		break;
		case 3:
			for (int i = 0; i< FLAGS_TOTAL;i++){
				if(Flags[i].x != -1){
					map3[Flags[i].y][Flags[i].x] = A;//brick flags for level 3
				}
			}//flags
			map3[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				if (Wolves[i].x!=-1){
					map3[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
				}
			}//wolves
		break;			
	}//switch
}*/

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
void SysTick_Handler(void){// every 25 ms
	ADC_In89(ADCdata);
	status = 1;
	time--;
}

direction_t Convert(uint32_t input[2]){
	//data[0];//North/South (vert - PE5)
  //data[1];//East/West (horz - PE4)
	direction_t output = NONE;
	if (4096>input[1]&&input[1]>=3500){
		output = NORTH;
	}
	else if (500>=input[1]&&input[1]>0){
		output = SOUTH;
	}
	else if (500>=input[0]&&input[0]>0){
		output = EAST;
	}
	else if (4096>input[0]&&input[0]>=3500){
		output = WEST;
	}
  return output;
}

int playgame(int difficulty){//ADD SOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		lives = 3;
		winlose = -1;//0 = lose, 1 = win
		flagcount = 0;
		score = 0;
		direction_t tempDir;
		mapIcons map[10][8];
		initMap(map,difficulty);
		SysTick_Init();
		EnableInterrupts();
		while (winlose==-1){
			if(status){
			displayActors();
			//displayradar();
			//userinput
			tempDir = Convert(ADCdata);
			if (checkCanMove(tempDir, &pig, map)){
				pig.direction = tempDir;
			}
			move(&pig, map);
			for (int i = 0; i<WOLVES_SIZE; i++){
				if(Wolves[i].x!=-1){
					moveEnemy(&Wolves[i], map);
					collideWolves(&Wolves[i],map,difficulty);
				}
			}
			for(int i = 0; i< FLAGS_TOTAL; i++){
				collideFlags(&Flags[i]);
			}
			//updateradar();
			if(lives == 0){
				winlose = 0;
				return winlose;
			}
			if (flagcount == 10){
				score = score + time*10;//increase score by some factor dependent on time!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
				winlose = score;
				return winlose;
			}
			status = 0;
		}//if
		}//while
		return -1;
}

