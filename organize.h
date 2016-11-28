/*=====================================================================
INT MAIN
======================================================================*/


typedef enum { PIG, WOLF, NONE } entityType;
typedef enum direction {NORTH, EAST, SOUTH, WEST} Dir;

typedef enum {
	O,//open path
	C,//barrier
	A,//apple
	H,//hole
	P,//pig
	W//wolf
}mapIcons;

typedef struct 
{
	float x, y; 
	
}vector2;

typedef struct
{
	vector2 position;
	Dir direction;
	float speed;
	short fuel;	//display as "energy"
	float radius;
	entityType type;
}entity;

typedef struct
{
	//float deltaTime; //Store time since last game update
	short level; //Which level currently on
	mapIcons** levelData[3]; //Level datas
	
	//Entities
	entity enemy [5]; //Array of enemies
	entity player; //Player
	short enemyCount;
	int score;
	short lives;
	
} game;
void beginGame(game*);
void updateGame(game*);				//TO DO LATER!!!!!!!!!!!!!!!
void handleCollisions(game*);
void renderFrame(game*);
