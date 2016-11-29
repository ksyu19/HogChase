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

typedef struct 
{
	float x, y; 
	
}vector2;

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