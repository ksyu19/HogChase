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
#include <math.h>

/*=====================================================================
GAME
=====================================================================*/
typedef dir{enum NORTH, EAST, SOUTH, WEST} ;
typedef struct
{
	float deltaTime; //Store time since last game update
	short level; //Which level currently on
	level levelData[3]; //Level datas
	
	//Entities
	entity* enemies; //Array of enemies
	entity* player; //Player

} game;

/*===================================================================================================================================
VECTOR FUNCTIONS:
These are so that we can later move our sprites when we interface with the joystick.
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

/*===================================================================================================================================
PLAYER: This is where the character info is bundled and vectors can be dealt with.
=====================================================================================================================================*/
typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
}player;


/*=====================================================
ENTITY
=====================================================*/
const short maxFuel =  20;
typedef enum { PLAYER, PIG, COW, CHICKEN } entityType;
typedef struct
{
	vector2 position;
	dir curDir;
	float speed;
	short fuel;
	float radius;
	entityType type;
} entity;

//--------------------------------------------------------------------
void initializeEntity(entity* ent)
{
	
};

/*======================================================================
COLLISION DETECTION
======================================================================*/
int collideWithWalls(entity* ent, short** levelData)
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 1 )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 1 )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 1 )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == 1 )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == 1 )
	{
		return 1;
	}
	
	return 0;
};
//-----------------------------------------------
int collideWithBoulder(entity* ent, short** levelData)
{
	//Every tile occupies a space of 16x16
	short i, j;
	
	//Check if center collides
	i = ent->position.y / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 3 )
	{
		return 1;
	}
	
	//Check if top collides
	i = (ent->position.y - ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 3 )
	{
		return 1;
	}
	
	//Check if bottom collides
	i = (ent->position.y + ent->radius) / 16;
	j = ent->position.x / 16;
	if ( levelData[i][j] == 3 )
	{
		return 1;
	}
	
	//Check if left collides
	i = ent->position.y / 16;
	j = (ent->position.x - ent->radius) / 16;
	if ( levelData[i][j] == 3 )
	{
		return 1;
	}
	
	//Check if right collides
	i = ent->position.y / 16;
	j = (ent->position.x + ent->radius) / 16;
	if ( levelData[i][j] == 3 )
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
		
	if (magnitude(distance) < ent1->radius + ent2->radius)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*======================================================================
LEVEL STUFF
=======================================================================*/
const short row = 32;
const short col = 40;

//Loads level data hard coded
short** loadLevel(short levelNum)
{
	short levDat[row][col];
	if (levelNum == 1)
	{
		
	}
	else if (levelNum == 2)
	{
	
	}
	else if (levelNum == 3)
	{
	
	}
	else
	{
		//???????
	}
	return levDat;
};


//------------------------------------------
void initialize Game(game* G)
{

};

//------------------------------------------
void initializeLevels(game* G);
{
	int i;
	for (i = 0; i < 3; i++)
	{
		G->levelData[i] = loadLevel(i+1);
	}
};

//------------------------------------------
void initializeEntities(game* G)
{
	//Create Enemies Data
	G->enemies = (entity*) malloc((2+level)*sizeof(entity*));
};
//-------------------------------------------
void clearFrame()
{
	//Blank all the pixels

}
//-------------------------------------------
void renderEntity(entity* ent)
{
	if ( ent->type == PLAYER )
	{
		//Draw player Bitmap at player position
	}
	else if ( ent->type == ENEMY )
	{
		//Draw enemy Bitmap at enemy position
	}
	else
	{
		//Draw nothing
	}
}
//-------------------------------------------
void renderFrame(game* G)
{
	//Set camera based on player position
	short camI = G->player.y / 16;
	short camJ = G->player.x / 16;
	
	//Render tiles around player
	int i, j;
	for (i = camI-3; i <= camI+4; i++)
	{
		for (j = camJ - 4; i <= camJ + 4)
		{
			//LOAD WALL INTO BUFFER
			if (G->levelData[level][i][j] ==  1)
			{
				//DRAW WALL AT y=i*16, x=j*16
			}
		}
		
	}
	
	//Render Enemies
	for (i = 0; i < 3; i++)
	{
		renderEntity(G->enemy[i]);
	}
	
	//Render player
	renderEntity(G->player);
	
	clearFrame();
	renderBuffer();

}

/*=====================================================================
INT MAIN
======================================================================*/
int main()
{
	game RallyX;

	initializeGame(&RallyX);
	
	//Game running
	while(1)
	{
		updateGame(&RallyX);
		handleCollisions(&RallyX);
		renderFrame(&RallyX);
	}
	return 0;
}
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
/*
