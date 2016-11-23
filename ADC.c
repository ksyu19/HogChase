// put implementations for functions, explain how it works
// put your names here, date
// put implementations for functions, explain how it works
// put your names here, date

/*==================================================================================================================================
GLOBALS:
====================================================================================================================================*/
const int MAXFUEL = 100;

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
PLAYER:
These probably shouldn't be in the ADC.c file. We might just need another module for logic updates.
=====================================================================================================================================*/
typedef struct 
{
	vector2 position; //coordinates of player
	vector2 direction; 
	//add a pointer to bitmap info
	short alive; //1 if alive, 0 if dead
	unsigned short fuel; //to check for scoring and life purposes
	
}player;

void initPlayer (player *player1)
{
	player1->alive = 1;
	player1->fuel = MAXFUEL;
	
}
