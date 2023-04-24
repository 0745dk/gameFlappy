#include "bmp.h"
#include "datastruct.h"
#include "action.h"
#include <stdio.h>
#define MaxSpeed 10
#define UpMaxSpeed -10
#define Gravity 9.8

int Hit(obj Obj1, obj Obj2)
{
	int i, j;
	for (i = 0; i < 8 && Obj1.hitBox[i] != -1; i += 4)
	{
		for (j = 0; j < 8 && Obj2.hitBox[j] != -1; j += 4)
		{
			int x1 = Obj1.x + Obj1.hitBox[i], x2 = Obj1.x + Obj1.hitBox[i + 2], y1 = Obj1.y + Obj1.hitBox[i + 1], y2 = Obj1.y + +Obj1.hitBox[i + 3];
			int a1 = Obj2.x + Obj2.hitBox[j], a2 = Obj2.x + Obj2.hitBox[j + 2], b1 = Obj2.y + Obj2.hitBox[j + 1], b2 = Obj2.y + +Obj2.hitBox[j + 3];
			if (x1 < a2 && a1 < x2 && y1 < b2 && b1 < y2)
			{
				return 1;
			}
		}
	}
	return 0;
}


void MoveObjTo(obj* object, int x, int y)//将object移动到x,y处。
{
	object->x = x;
	object->y = y;
	BmpDraw(*object, 0xFFFFFF);
}


void Mov(obj* object, int pixel, int direction)
{
	if (direction == Left || direction == Right && pixel < 0)
	{
		MoveObjTo(object, object->x - abs(pixel), object->y);
		RefreshBackground(object->x + object->width, object->y, object->x + object->width + abs(pixel), object->y + object->height, object->priority);
	}
	else if (direction == Right || direction == Left && pixel < 0)
	{
		MoveObjTo(object, object->x + abs(pixel), object->y);
		RefreshBackground(object->x - abs(pixel), object->y, object->x, object->y + object->height, object->priority);
	}
	else if (direction == Up || direction == Down && pixel < 0)
	{
		MoveObjTo(object, object->x, object->y - abs(pixel));
		RefreshBackground(object->x, object->y + object->height, object->x + object->width, object->y + object->height + abs(pixel), object->priority);
	}
	else if (direction == Down || direction == Up && pixel < 0)
	{
		MoveObjTo(object, object->x, object->y + abs(pixel));
		RefreshBackground(object->x, object->y - abs(pixel), object->x + object->width, object->y, object->priority);
	}
}

int Acceleration(int times, int v0, int a)//1秒: times = 36.467
{
	double sec = times / 37.467;
	sec /= 2;
	double v = sec * Gravity - (a)+v0 > MaxSpeed ? MaxSpeed : sec * Gravity - (a)+v0;
	v = v < UpMaxSpeed ? UpMaxSpeed : v;

	return (int)v;
}
