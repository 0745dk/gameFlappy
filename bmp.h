#ifndef __BMP_H__
#define __BMP_H__

#include "datastruct.h"

void BmpDraw(obj object, int filterColor);
void RefreshBackground(int x0, int y0, int x1, int y1,int priority);
void FreeBackgroundTemp();//�ͷŵ�����bmp�ļ��Ļ��棬�����ڴ�й©��
void OpenBackGround(char*);//�򿪱���
int GetColorInBmp(int x, int y, obj object);

#endif // !__BMP_H__
