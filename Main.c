#include <stdio.h>
#include "organize.h"

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
