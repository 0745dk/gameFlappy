#ifndef __BMP_H__
#define __BMP_H__

#include "datastruct.h"

void BmpDraw(obj object, int filterColor);
void RefreshBackground(int x0, int y0, int x1, int y1,int priority);
void FreeBackgroundTemp();//释放掉所有bmp文件的缓存，避免内存泄漏。
void OpenBackGround(char*);//打开背景
int GetColorInBmp(int x, int y, obj object);

#endif // !__BMP_H__
