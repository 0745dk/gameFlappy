#ifndef __ACTION_H__
#define __ACTION_H__
#include "datastruct.h"

#define Left 1 //��
#define Right 2//��
#define Up 3   //��
#define Down 4 //��

int Acceleration(int times,int v0,int a);//ʹobject��һ���������ٶ�+����������׹������object���ٶ�
int Hit(obj Obj1, obj Obj2);//�ж�obj1��obj2�Ƿ���ײ��1������ײ��0���ز���ײ
void MoveObjTo(obj* object, int x, int y);//��object�ƶ���x,y����
void Mov(obj* object, int pixel, int direction);//��Obj��direction�����ƶ�pixel�����ء�


#endif // !__ACTION_H__
