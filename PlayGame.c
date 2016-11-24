// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

#include <stdint.h>
#include "Display.h"
#define WOLVES_SIZE 3 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later (might depend on difficulty)
#define FLAGS_TOTAL 10 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later

typedef enum {NORTH,EAST,SOUTH,WEST} direction_t;
typedef enum {PAUSE,RESUME,QUIT} buttons;//implement later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef struct actor{
	int x;
	int y;
	int speed;
	direction_t direction;
}actorType;
typedef struct flag{
	int x; // x = -1 when flag has been collected
	int y;
}flagType;

actorType pig; 
actorType Wolves[WOLVES_SIZE];
flagType Flags[FLAGS_TOTAL];
int lives;
int winlose;
int flagcount;
int time;

void PlayGameInit(void){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11!fix locations speed direction later!!!!
	pig.x = 0;
	pig.y = 0;
	pig.speed = 0;
	pig.direction = NORTH;
	for (int i = 0; i<WOLVES_SIZE; i++){
		Wolves[i].x = 0;
		Wolves[i].y = 0;
		Wolves[i].speed = 0;
		Wolves[i].direction = NORTH;
	}
	time = 10; //need Systick to decrement time!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

int checkCanMove(direction_t dir){
	int move;//0 = can't move, 1 = can move
	//check if path available!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return move;	
}

void move(actorType *p){
	if (checkCanMove(p->direction)){	
		switch(p->direction){
			case NORTH:
				(*p).y += (*p).speed;
				break;
			case EAST:
				(*p).x += (*p).speed;
				break;
			case SOUTH:
				(*p).y -= (*p).speed;
				break;
			case WEST:
				(*p).x -= (*p).speed;
				break;
		}//switch
	}
}//move

direction_t findbestpath(){
	direction_t best;
	//find the best path!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return best;
}
void moveEnemy(actorType *wolf){
	direction_t dir;
	dir = findbestpath();//adjust later for difficulty!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	(*wolf).direction = dir;
	move(wolf);
}//moveEnemy

void updatemap(){
	//update the map!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if flag[i] == -1, then do not display
}
void updateradar(){
	//update the radar!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if flag[i] == -1, then do not display
}
void flagInit(){
	for (int i = 0; i < FLAGS_TOTAL; i++){
		Flags[i].x = 0;
		Flags[i].y = 0;
	}
	//initialize location of flags!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
void collideWolves(actorType *wolf){
	if((pig.x == wolf->x&&pig.y == wolf->y))
	{
		lives--;
		PlayGameInit();//reset actors and time
	}//rough collision detection with wolves -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1fix later, maybe add rocks/obstacles
}
void collideFlags(flagType *f){
	if(f->x!=-1){
		if((pig.x==f->x)&&(pig.y==f->y)){
			flagcount++;
			f->x = -1;
		}
	}
}
int playgame(int difficulty){
		PlayGameInit();
		lives = 3;
		winlose = -1;//0 = lose, 1 = win
		flagcount = 0;
		flagInit();
		while (winlose==-1){
			displaymap();
			displayradar();
			//user input!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!add an interrupt that changes pig's direction when user presses button, if possible to face in that direction
			move(&pig);
			for (int i = 0; i<WOLVES_SIZE; i++){
				move(&Wolves[i]);
				collideWolves(&Wolves[i]);
			}
			for(int i = 0; i< FLAGS_TOTAL; i++){
				collideFlags(&Flags[i]);
			}
			updatemap();
			updateradar();
			if(lives == 0){
				winlose = 0;
				return winlose;
			}
			if (flagcount == 10){
				winlose = 1;
				return winlose;
			}
		}//while
		return -1;
}

