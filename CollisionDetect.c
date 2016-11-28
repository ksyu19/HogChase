#include <stdio.h>
#include <math.h>
#include "CollisionDetect.h"

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