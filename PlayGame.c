// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

#include <stdint.h>
#include "Display.h"
typedef enum {NORTH,EAST,SOUTH,WEST}direction_t ; 
typedef struct actor{
	int x;
	int y;
	int speed;
	direction_t direction;
}actorType;
/*int getX(){
}
int getY(){
}
int getSpeed(){
}
int getDirection(){
}
void setX(){
}
void setY(){
}
void setSpeed(){
}
void setDirection(){
}*/

actorType pig = {0,0,0,NORTH};
actorType Wolves[3] = {{0,0,0,NORTH},{0,0,0,NORTH},{0,0,0,NORTH}};
int lives = 3;
int winlose = 0;//0 = lose, 1 = win
int flags = 0;
int checkCanMove(direction_t dir){
	int move;//0 = can't move, 1 = can move
	return move;	
}
void move(actorType *p){
	if ((*p).direction == NORTH){
		if(checkCanMove(NORTH)){
			(*p).y += (*p).speed;
		}
	}//moveNorth
	if ((*p).direction == EAST){
		if(checkCanMove(EAST)){
			(*p).x += (*p).speed;
		}
	}//moveEast
	if ((*p).direction == SOUTH){
		if(checkCanMove(SOUTH)){
			(*p).y -= (*p).speed;
		}
	}//moveSouth
	if ((*p).direction == WEST){
		if(checkCanMove(WEST)){
			(*p).x -= (*p).speed;
		}
	}//moveWest
}//move
direction_t findbestpath(){
	direction_t best;
	return best;
}
void moveEnemy(actorType *wolf){
	direction_t dir;
	dir = findbestpath();//adjust later for difficulty
	(*wolf).direction = dir;
	move(wolf);
}//moveEnemy
void updatemap(){
}
void updateradar(){
}
int playgame(int difficulty){
		displaymap();
		displayradar();
		move(&pig);
		move(&Wolves[0]);
		move(&Wolves[1]);
		move(&Wolves[2]);
		if((pig.x == Wolves[0].x&&pig.y == Wolves[0].y)||(pig.x == Wolves[1].x&&pig.y == Wolves[1].y)||(pig.x == Wolves[2].x&&pig.y == Wolves[2].y))
		{
			//decrement lives
			//reset position
			//reset time
		}//rough collision detection
		updatemap();
		updateradar();
		if(lives == 0){
			winlose = 0;
			return winlose;
		}
		if (flags == 10){
			winlose = 1;
			return winlose;
		}
		//time
		return -1;
}

