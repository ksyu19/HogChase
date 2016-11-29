#include "HogChase.h"

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
