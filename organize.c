#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TExaS.h"




/*===================================================================================================================================
VECTOR FUNCTIONS: These are so that we can later move our sprites when we interface with the joystick.
=================================================================================================================================*/
typedef struct 
{
	float x, y; 
	
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

/*=====================================================
ENTITY
=====================================================*/

const short maxFuel =  100;
typedef enum { PIG, WOLF, NONE } entityType;
typedef enum direction {NORTH, EAST, SOUTH, WEST} Dir;

typedef struct
{
	vector2 position;
	Dir direction;
	float speed;
	short fuel;	//display as "energy"
	float radius;
	entityType type;
}entity;

/*======================================================================
LEVEL STUFF
=======================================================================*/
typedef enum {
	O,//open path
	C,//barrier
	A,//apple
	H,//hole
	P,//pig
	W//wolf
}mapIcons;
const short row = 32;
const short col = 40;

//Loads level data hard coded
void loadLevel(short level,mapIcons map[][col])
{
	if (level == 1)
	{
		mapIcons mapArray[row][col] = 
		{
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,O,O,O,C,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C}
		};
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
	}
	else if (level == 2)
		{
		mapIcons mapArray[row][col] =  
		{
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,O,O,O,C,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C}
		}; 
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
		}
	else if (level == 3)
		{		
		mapIcons mapArray[row][col] = 
		{
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,C,C,C,C,C,C,C,O,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,C,C,C,O,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,C,C,O,O,O,O,O,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,O,O,O,C,C},
		{C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,C,O,C,C,C,C,C,O,C,C,C,C,C,O,O,C,C,C,C,C,C,O,O,C,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,C,C,O,O,O,O,O,O,O,C,C,C,C,C,C,C,C,C,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,C,C,O,C,C,C,C,C,O,O,O,O,O,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,C,C,C,O,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,C,C,O,O,O,O,O,O,O,O,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,O,O,O,O,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,O,O,O,O,O,O,O,O,O,O,C,C,O,C},
		{C,C,O,C,C,C,O,C,C,C,C,C,C,C,C,O,C,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,C,C,C,O,O,O,O,O,O,O,O,O,O,O,O,C,O,C,O,C,O,C,O,C,C,C,C,C,C,C,C,C,O,C,C,O,C},
		{C,C,O,O,O,O,O,O,O,O,C,C,C,O,C,C,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,C},
		{C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C}
		}; 
			int i,j;
		  for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
			map[i][j] = mapArray[i][j];
			}
		}
		}
};
/*=====================================================================
GAME
=====================================================================*/

typedef struct
{
	//float deltaTime; //Store time since last game update
	short level; //Which level currently on
	mapIcons** levelData; //Level data
	
	//Entities
	entity enemy [5]; //Array of enemies
	entity player; //Player
	short enemyCount;
	int score;
	short lives;
	
} game;
//------------------------------------------
void initializeEntities(game* G)
{
	G->enemyCount = 0;
	int i=0;
	int j=0;
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < col; j++)
			{
				if (G->levelData[i][j]== P)
				{
					G->player.position.x = j*16 + 8;
					G->player.position.y = i*16 + 8;
					G->player.radius = 4;					//4 is just a placeholder
					G->player.fuel = maxFuel;
					G->player.type = PIG; //DO NOT CLEAR OUT THE PLAYER!
				}
				else if (G->levelData[i][j]== W)
				{
					G->enemy[G->enemyCount].position.x = j*16 + 8;
					G->enemy[G->enemyCount].position.y = i*16 + 8;
					G->enemy[G->enemyCount].radius = 4;					//4 is just a placeholder
					G->enemy[G->enemyCount].fuel = maxFuel;     //Enemies don't lose fuel.
					G->enemy[G->enemyCount].type = WOLF;
					G->levelData[i][j]= O;
					G->enemyCount++;
				}
			
			}
		}
		for (i = G->enemyCount; i < 5; i++)		//The entities that no longer exist must be "invisible"
		{
			G->player.position.x = -1;
			G->player.position.y = -1;
			G->player.radius = 0;					//4 is just a placeholder
			G->player.fuel = 0;
			G->player.type = NONE;
		}
};
//--------------------------------------------------------------------------

void beginGame(game* G)
{
		initializeEntities(G);
		G->score = 0;
		G->lives = 3;
};
 
//------------------------------------------------------------------------------------------------------------------------------------
short nextPosIsWall(entity* ent, mapIcons** levelData)
{
	vector2 nextPos = ent->position;
	if (ent->direction == NORTH)
	{
		nextPos.y -= ent->speed;
	}
	else 	if (ent->direction == SOUTH)
	{
		nextPos.y += ent->speed;
	}
	else 	if (ent->direction == EAST)
	{
		nextPos.x += ent->speed;
	}
	else 	if (ent->direction == WEST)
	{
		nextPos.x -= ent->speed;
	}
	if (levelData[(int)(nextPos.y/16)][(int)(nextPos.x/16)] == C)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
};
//------------------------------------------------------------------------------------------------------------------------------------
void moveEntity (game *G, entity *ent)
{
	while (nextPosIsWall(&G->player , G->levelData) == 1)
	{
		if (ent->direction == NORTH)
		{
			ent->direction = WEST;
		}
		else if (ent->direction == SOUTH)
		{
			ent->direction = EAST;
		}
		else if (ent->direction == WEST)
		{
			ent->direction = SOUTH;
		}
		else if (ent->direction == EAST)
		{
			ent->direction = NORTH;
		}
	}
	vector2 nextPos = G->player.position;
	if (ent->direction == NORTH)
	{
		nextPos.y -= G->player.speed;
	}
	else 	if (ent->direction == SOUTH)
	{
		nextPos.y += G->player.speed;
	}
	else 	if (ent->direction == EAST)
	{
		nextPos.x += G->player.speed;
	}
	else 	if (ent->direction == WEST)
	{
		nextPos.x -= G->player.speed;
	}
	G->player.position = nextPos;
	
}
//------------------------------------------------------------------------------------------------------------------------------------
void respawn (game *G)
{
	int i, j;   // i is y, j is x
	for (i = 0; i < row; i++)
	{
			for (j = 0; j < col; j++)
		{
			if(G->levelData[i][j]== P)
			{
				G->player.position.x = j*16 + 8;
				G->player.position.y = i*16 + 8;					
				G->player.fuel = maxFuel;
			}
		}
	}		
}

/*===================================================================================================================================
PLAYER: This is where the character info is bundled and vectors can be dealt with.
=====================================================================================================================================*/
typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
}player;



/*======================================================================
COLLISION DETECTION
======================================================================*/
int collideWithHole(entity* ent, mapIcons** levelData)
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == H )
	{
		return 1;
	}
	return 0;
};

//-----------------------------------------------
int collideWithApple(entity* ent, mapIcons** levelData)
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == A )
	{
		return 1;
	}
	
	return 0;
};
//---------------------------------------------------------
short entitiesColliding(entity* ent1, entity* ent2)
{
	vector2 distance;
		distance.x = abs(ent1->position.x -  ent2->position.x);
		distance.y = abs(ent1->position.y -  ent2->position.y);
		
	if (magnitude(&distance) < ent1->radius + ent2->radius)
	{
		return 1;
	}
	else
	{
		return 0;
	}
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


//-------------------------------------------
void clearFrame()
{
	//Blank all the pixels

}
//-------------------------------------------
int entityIsInView(entity* ent, vector2 camPos)
{
	int minX = camPos.x - 16*4 - 8 ; //need to figure this # out
	int maxX = camPos.x + 16*5 + 8; 
	int minY = camPos.y - 16*3 - 8;
	int maxY = camPos.y + 16*4 + 8; //Need to find min and max x and y on the SCREEN (some math)
	
	if (ent->position.x < minX || ent->position.x > maxX)
	{
		return 0;
	}
	else if (ent->position.y < minY || ent->position.y > maxY)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
//-------------------------------------------
void renderEntity(entity* ent, vector2 camPos)
{
	if (entityIsInView(ent, camPos))
	{
		if ( ent->type == PIG )
		{
			//Draw player Bitmap at player position
		}
		else if ( ent->type == WOLF )
		{
			//Draw enemy Bitmap at enemy position
		}
		else
		{
			//Draw nothing
		}
	}
}	
//----------------------------------------------------------------------------------------------
void renderMap (mapIcons visMap[][10])
{
	//visMap :map icon array of all tiles that are currently visible
	//visMap[0][0] is the top left tile on the screen.
	//visMap[7][9] is the bottom right tile on the screen
	//Draw Bitmaps by looping through the array and rendering appropriate tiles.
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (visMap[i][j] == C)
			{
				//draw Wall bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
			}
			else if (visMap[i][j] == A)
			{
				//draw apple bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
			}
			else if (visMap[i][j] == H)
			{
				//draw hole bitmap with top left corner of bitmap at pixel coordinate (j*16, i*16)
			}
			//NOTE: Entities are not dealt with in this function.
		}
	}
	
};

//-------------------------------------------
void renderFrame(game* G)
{
	vector2 camPos;
	
	mapIcons visMap [8][10];
	//Set camera based on player position
	short camI = G->player.position.y / 16;
	short camJ = G->player.position.x / 16;
	
	camPos.x = camJ + 8;
	camPos.y = camI + 8;
	
	//Render tiles around player
	int i, j, x = 0, y = 0;
	for (i = camI-3; i <= camI+4; i++)
	{
		for (j = camJ - 4; j <= (camJ + 5); j++)
		{
			visMap[x][y] = G->levelData[i][j];
			y++;
		}
		x++;
	}
	renderMap (visMap);
	
	//Render Enemies
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			renderEntity(&G->enemy[i], camPos);
		}
	}
	
	//Render player
	renderEntity(&G->player, camPos);
	
	clearFrame();
	

}
//-------------------------------------------
void updateGame (game *G)
{
	int i;
	//TO DO: Process player input.
	moveEntity (G, &G->player); 
	for (i = 0; i < 5; i++)
	{
		if (G->enemy[i].type != NONE)
		{
			moveEntity (G, &G->enemy[i]);
		}
	}
};

/*=====================================================================
INT MAIN
======================================================================*/
int main()
{
	game RallyX;

	beginGame(&RallyX);
	
	//Game running
	while(1)
	{
		updateGame(&RallyX);				//TO DO LATER!!!!!!!!!!!!!!!
		handleCollisions(&RallyX);
		renderFrame(&RallyX);
	}
	return 0;
}
//#include <stdint.h>
//#include "Display.h"
//#include "random.h"
/*===================================================================================================================================
CONSTANTS, TYPEDEFS (ENUMS, STRUCTS)
=====================================================================================================================================*/
//#define WOLVES_SIZE 3 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later (might depend on difficulty)
//#define FLAGS_TOTAL 10 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later
//typedef enum {NORTH,EAST,SOUTH,WEST} direction_t;
//typedef enum {PAUSE,RESUME,QUIT} buttons;//implement later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*===================================================================================================================================
DECLARE VARIABLES
=====================================================================================================================================

===================================================================================================================================
INITIALIZATION FUNCTIONS (and helpers)
=====================================================================================================================================
void flagInit(int diff){
	int m;
	for (int i = 0; i < FLAGS_TOTAL; i++){
		switch(diff){
			case 1:		
				do {
					m = (Random()>>21)%1280; //random number from 0 to 1279
				}while((map1[m/40][m%40]!=O));
				Flags[i].x = m%40;//column
				Flags[i].y = m/40;//row
			break;
			case 2:
			break;
			case 3:
			break;
		}//switch
	}//for
}//flagInit
void PlayGameInit(int diff){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11!fix locations speed direction later!!!!
	switch(diff){
		case 1://for level 1 difficulty map
			pig.x = 20;//column in map
			pig.y = 17;//row in map
			pig.direction = EAST;
			Wolves[0].x = 5;
			Wolves[0].y = 26;
			Wolves[1].x = 19;
			Wolves[1].y = 26;
			Wolves[2].x = 33;
			Wolves[2].y = 26;
		break;
		case 2://for level 2 difficulty map
			pig.x = 20;//column in map
			pig.y = 17;//row in map
			pig.direction = EAST;
			Wolves[0].x = 5;
			Wolves[0].y = 26;
			Wolves[1].x = 19;
			Wolves[1].y = 26;
			Wolves[2].x = 33;
			Wolves[2].y = 26;
		break;
		case 3://for level 3 difficulty map
			pig.x = 20;//column in map
			pig.y = 17;//row in map
			pig.direction = EAST;
			Wolves[0].x = 5;
			Wolves[0].y = 26;
			Wolves[1].x = 19;
			Wolves[1].y = 26;
			Wolves[2].x = 33;
			Wolves[2].y = 26;
		break;
	}
	pig.speed = 1;
	for (int i = 0; i<WOLVES_SIZE; i++){
		Wolves[i].speed = 1;
		Wolves[i].direction = NORTH;
	}
	time = 10; //need Systick to decrement time!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
===================================================================================================================================
GAME FUNCTIONS
=====================================================================================================================================
int checkCanMove(direction_t dir){
	int move;//0 = can't move, 1 = can move
	//check if path available!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return move;	
}

void move(actorType *p){
	if (checkCanMove(p->direction)){	
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

void collideWolves(actorType *wolf, int diff){
	if((pig.x == wolf->x&&pig.y == wolf->y))
	{
		lives--;
		PlayGameInit(diff);//reset actors and time
	}//rough collision detection with wolves -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1fix later, maybe add rocks/obstacles
}
void collideFlags(flagType *f){
	if(f->x!=-1){
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
					map1[Flags[i].y][Flags[i].x] = S;//straw flags for level 1
				}
			}//flags
			map1[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				map1[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
			}//wolves
		break;
		case 2:
			for (int i = 0; i< FLAGS_TOTAL;i++){
				if(Flags[i].x != -1){
					map1[Flags[i].y][Flags[i].x] = T;//twig flags for level 2
				}
			}//flags
			map1[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				map1[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
			}//wolves
		break;
		case 3:
			for (int i = 0; i< FLAGS_TOTAL;i++){
				if(Flags[i].x != -1){
					map1[Flags[i].y][Flags[i].x] = B;//brick flags for level 3
				}
			}//flags
			map1[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				map1[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
			}//wolves
		break;			
	}//switch
}
void updateradar(){
	//update the radar!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if flag[i] == -1, then do not display
}
int playgame(int difficulty){
		PlayGameInit(difficulty);//set locations of pigs and wolves
		updatemap(difficulty);//include pig and wolf locations in map
		flagInit(difficulty);//set locations of flags in map
		updatemap(difficulty);//include flag locations in map
		lives = 3;
		winlose = -1;//0 = lose, 1 = win
		flagcount = 0;
		score = 0;
		while (winlose==-1){
			displaymap();
			displayradar();
			//user input!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!add an interrupt that changes pig's direction when user presses button, if possible to face in that direction
			move(&pig);
			for (int i = 0; i<WOLVES_SIZE; i++){
				move(&Wolves[i]);
				collideWolves(&Wolves[i],difficulty);
			}
			for(int i = 0; i< FLAGS_TOTAL; i++){
				collideFlags(&Flags[i]);
			}
			updatemap(difficulty);
			updateradar();
			if(lives == 0){
				winlose = 0;
				return winlose;
			}
			if (flagcount == 10){
				score = score + time*10;//increase score by some factor dependent on time!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
				winlose = score;
				return winlose;
			}
		}//while
		return -1;
}
*/
