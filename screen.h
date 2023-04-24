#ifndef __SCREEN_H__
#define __SCREEN_H__


#include <linux/fb.h>


void Fill_Color(int color);//全屏填充为<color>色
void LcdOp();//打开Lcd屏幕
void LcdClos();//关闭Lcd屏幕
void DrawCircle(int x, int y, int r, int color);//画圆。x:圆心横向坐标。y:纵向。r:圆心。color:颜色
void XY_Color(int x, int y, int color);





#endif // !__SCREEN_H__
