// PlayGame.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Student names: Alex Smith and Karena Yu
// Last modification date: 11-16-16

#include <stdint.h>
#include "Display.h"
#include "random.h"
#include "Switch.h"
#include "tm4c123gh6pm.h"
#include "ADC.h"
/*===================================================================================================================================
CONSTANTS, TYPEDEFS (ENUMS, STRUCTS)
=====================================================================================================================================*/
#define WOLVES_SIZE 3 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later (might depend on difficulty)
#define FLAGS_TOTAL 5 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!adjust later
typedef enum {NORTH,EAST,SOUTH,WEST,NONE} direction_t;
typedef enum {PAUSE,RESUME,QUIT} buttons;//implement later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef enum {
	O,//open path
	C,//closed path
	A,//apple
	//S,//straw
	//T,//twigs
	//B,//bricks
	P,//pig
	W//wolf
}mapIcons;
typedef struct actor{
	int x;// x = -1 when actor doesn't exist
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
//Level 3 map:
mapIcons map1[10][8] ={
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
mapIcons map2[10][8] ={
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
//Level 1 Map:
/*mapIcons map1[32][40] = 
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
}; */
/*===================================================================================================================================
INITIALIZATION FUNCTIONS (and helpers)
=====================================================================================================================================*/
void flagInit(int diff){
	int m;
	for (int i = 0; i < FLAGS_TOTAL; i++){
		switch(diff){
			case 1:		
				do {
					m = (Random()>>21)%80; //random number from 0 to 79  (%1280 to get random number from 0 to 1279)
				}while((map1[m/40][m%40]!=O));
				Flags[i].x = m%40;//column
				Flags[i].y = m/40;//row
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
}//flagInit
void PlayGameInit(int diff){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11!fix locations speed direction later!!!!
	switch(diff){
		case 1://for level 1 difficulty map
			pig.x = 3;//column in map
			pig.y = 6;//row in map
			pig.direction = NORTH;
			Wolves[0].x = 6;
			Wolves[0].y = 8;
			Wolves[1].x = -1; //doesn't exist
			Wolves[2].x = -1; //doesn't exist
		break;
		case 2://for level 2 difficulty map
			pig.x = 20;//column in map
			pig.y = 17;//row in map
			pig.direction = EAST;
			Wolves[0].x = 5;
			Wolves[0].y = 26;
			Wolves[1].x = 19;
			Wolves[1].y = 26;
			Wolves[2].x = -1;
		break;
		case 3://for level 3 difficulty map
			/* values for map1 large:
			pig.x = 20;//column in map
			pig.y = 17;//row in map
			pig.direction = EAST;
			Wolves[0].x = 5;
			Wolves[0].y = 26;
			Wolves[1].x = 19;
			Wolves[1].y = 26;
			Wolves[2].x = 33;
			Wolves[2].y = 26;
			*/
		break;
	}
	pig.speed = 1;
	for (int i = 0; i<WOLVES_SIZE; i++){
		Wolves[i].speed = 1;
		Wolves[i].direction = NORTH;
	}
	time = 5400; //3 min to play each round
}
/*===================================================================================================================================
GAME FUNCTIONS
=====================================================================================================================================*/
int checkCanMove(direction_t dir, actorType *a, int diff){
	int move = 0;//0 = can't move, 1 = can move
	int loc;
	switch(diff){
		case 1: 
			switch(dir){
			case NORTH:
				loc = (*a).y-(*a).speed;
				if (map1[loc][(*a).x] != C)
					move = 1;
				break;
			case EAST:
				loc = (*a).x + (*a).speed;
				if (map1[(*a).y][loc] != C)
					move = 1;
				break;
			case SOUTH:
				loc = (*a).y+(*a).speed;
				if (map1[loc][(*a).x] != C)
					move = 1;
				break;
			case WEST:
				loc = (*a).x - (*a).speed;
				if (map1[(*a).y][loc] != C)
					move = 1;
				break;
			case NONE: break;
		}//level 1
		case 2: 
			switch(dir){
			case NORTH:
				loc = (*a).y-(*a).speed;
				if (map2[loc][(*a).x] != C)
					move = 1;
				break;
			case EAST:
				loc = (*a).x + (*a).speed;
				if (map2[(*a).y][loc] != C)
					move = 1;
				break;
			case SOUTH:
				loc = (*a).y+(*a).speed;
				if (map2[loc][(*a).x] != C)
					move = 1;
				break;
			case WEST:
				loc = (*a).x - (*a).speed;
				if (map2[(*a).y][loc] != C)
					move = 1;
				break;
			case NONE: break;
		}//level 2
		case 3: 
			switch(dir){
			case NORTH:
				loc = (*a).y-(*a).speed;
				if (map3[loc][(*a).x] != C)
					move = 1;
				break;
			case EAST:
				loc = (*a).x + (*a).speed;
				if (map3[(*a).y][loc] != C)
					move = 1;
				break;
			case SOUTH:
				loc = (*a).y+(*a).speed;
				if (map3[loc][(*a).x] != C)
					move = 1;
				break;
			case WEST:
				loc = (*a).x - (*a).speed;
				if (map3[(*a).y][loc] != C)
					move = 1;
				break;
			case NONE: break;
		}//level 3
	}//switch diff
	return move;	
}

void move(actorType *p, int diff){
	if (checkCanMove(p->direction, p, diff)){	
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

direction_t findbestpath(actorType *wolf, int diff){
	direction_t best;
	int e = checkCanMove(EAST,wolf,diff);
	int w = checkCanMove(WEST,wolf,diff);
	int s = checkCanMove(SOUTH,wolf,diff);
	int n = checkCanMove(NORTH,wolf,diff);
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
	else if (checkCanMove((*wolf).direction,wolf,diff)){
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
void moveEnemy(actorType *wolf, int diff){
	(*wolf).direction = findbestpath(wolf, diff);
	move(wolf, diff);
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
					map1[Flags[i].y][Flags[i].x] = A;//straw flags for level 1
				}
			}//flags
			map1[pig.y][pig.x] = P;//pig
			for (int i = 0; i< WOLVES_SIZE;i++){
				if (Wolves[i].x!=-1){
					map1[Wolves[i].y][Wolves[i].x] = W; //if Flag and Wolf are in same spot, wolf takes precedent in display
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
void SysTick_Handler(void) // every 25 ms
{
	ADC_In89(ADCdata);
	status = 1;
	time--;
}

direction_t Convert(uint32_t input[2]){//CALIBRATE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//data[0];//North/South (vert - PE5)
  //data[1];//East/West (horz - PE4)
	direction_t output = NONE;
	if (4000>input[0]&&input[0]>3500){
		output = NORTH;
	}
	else if (500>input[0]&&input[0]>0){
		output = SOUTH;
	}
	else if (500>input[1]&&input[1]>0){
		output = EAST;
	}
	else if (4000>input[1]&&input[1]>3500){
		output = WEST;
	}
  return output;
}

int playgame(int difficulty){//ADD SOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		PlayGameInit(difficulty);//set locations of pigs and wolves
		updatemap(difficulty);//include pig and wolf locations in map
		flagInit(difficulty);//set locations of flags in map
		updatemap(difficulty);//include flag locations in map
		lives = 3;
		winlose = -1;//0 = lose, 1 = win
		flagcount = 0;
		score = 0;
		direction_t tempDir;
		while (winlose==-1){
			if(status){
			displaymap();
			//displayradar();
			//userinput
			tempDir = Convert(ADCdata);
			if (checkCanMove(tempDir, &pig, difficulty)){
				pig.direction = tempDir;
			}
			move(&pig, difficulty);
			for (int i = 0; i<WOLVES_SIZE; i++){
				if(Wolves[i].x!=-1){
					moveEnemy(&Wolves[i], difficulty);
					collideWolves(&Wolves[i],difficulty);
				}
			}
			for(int i = 0; i< FLAGS_TOTAL; i++){
				collideFlags(&Flags[i]);
			}
			updatemap(difficulty);
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

