#include "datastruct.h"
#include "bmp.h"
#include "operate.h"
int GameOver()
{
	obj restart = InitObj("./Restart.bmp", 294, 150, 4);
	obj exit = InitObj("./exit.bmp", 294, 263, 4);
	BmpDraw(restart, 0xFFFFFF);
	BmpDraw(exit, 0xFFFFFF);
	while (1)
	{
		int f = TouchForMainThread(294, 150, 505, 217, 294, 263, 505, 330);
		if (f)//Restart
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

}
