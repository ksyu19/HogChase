// PlayGame.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

#include <stdint.h>
#include "Display.h"
#include "random.h"
/*===================================================================================================================================
CONSTANTS, TYPEDEFS (ENUMS, STRUCTS)
=====================================================================================================================================*/
#define WOLVES_SIZE 3 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later (might depend on difficulty)
#define FLAGS_TOTAL 10 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later
typedef enum {NORTH,EAST,SOUTH,WEST} direction_t;
typedef enum {PAUSE,RESUME,QUIT} buttons;//implement later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef enum {
	O,//open path
	C,//closed path
	S,//straw
	T,//twigs
	B,//bricks
	P,//pig
	W//wolf
}mapIcons;
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
//Level 1 Map:
mapIcons map1[32][40] = 
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
//Level 2 map:
mapIcons map2[32][40] = 
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
//Level 3 map:
mapIcons map3[32][40] = 
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
/*===================================================================================================================================
INITIALIZATION FUNCTIONS (and helpers)
=====================================================================================================================================*/
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
/*===================================================================================================================================
GAME FUNCTIONS
=====================================================================================================================================*/
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

/*===================================================================================================================================
VECTOR FUNCTIONS:
These are so that we can later move our sprites when we interface with the joystick.
=================================================================================================================================*/
/*typedef struct 
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

*/
/*===================================================================================================================================
PLAYER: This is where the character info is bundled and vectors can be dealt with.
=====================================================================================================================================*/
/*typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
}player;
*/


