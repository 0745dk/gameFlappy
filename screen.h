#ifndef __SCREEN_H__
#define __SCREEN_H__


#include <linux/fb.h>


void Fill_Color(int color);//ȫ�����Ϊ<color>ɫ
void LcdOp();//��Lcd��Ļ
void LcdClos();//�ر�Lcd��Ļ
void DrawCircle(int x, int y, int r, int color);//��Բ��x:Բ�ĺ������ꡣy:����r:Բ�ġ�color:��ɫ
void XY_Color(int x, int y, int color);





#endif // !__SCREEN_H__
