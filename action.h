#ifndef __ACTION_H__
#define __ACTION_H__
#include "datastruct.h"

#define Left 1 //←
#define Right 2//→
#define Up 3   //↑
#define Down 4 //↓

int Acceleration(int times,int v0,int a);//使object有一个重力加速度+空气阻力下坠。返回object的速度
int Hit(obj Obj1, obj Obj2);//判断obj1和obj2是否相撞。1返回相撞。0返回不相撞
void MoveObjTo(obj* object, int x, int y);//将object移动到x,y处。
void Mov(obj* object, int pixel, int direction);//将Obj向direction方向移动pixel个像素。


#endif // !__ACTION_H__
