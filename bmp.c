#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//------Open所需头文件-------

#include <stdio.h>
//------Perror所需头文件-------

//#include <sys/types.h>
#include <unistd.h>
//------Lseek---------------

#include "screen.h"
//------全局变量----------

#include <sys/mman.h>
#include <stdlib.h>
#include "bmp.h"
#include "character.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
obj backGround;
//unsigned char* backgroundPixel = NULL;
extern struct fb_var_screeninfo info;
extern int* plcd;
char* occupiedPixel;//屏幕中已经被占用的像素点,这些像素点不会被刷新.数字的大小表示图片的优先级
#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)>(b))?(b):(a))

int clamp(int x, int min,int max){
    return x > max ? max : x < min ? min : x;
}


/*刷新背景的某个区域到屏幕上。

(x0,y0)
*-------------------
|					|
|	  要刷新的区域	|
|					|
-------------------*
					(x1,y1)
priority:要刷新的区域的优先级.优先级相同的才会刷新。
*/
void RefreshBackground(int x0, int y0, int x1, int y1, int priority)
{
	int width = SCREEN_WIDTH, height = SCREEN_HEIGHT, depth = SCREEN_HEIGHT;
    x0 = clamp(x0, 0, abs(width) - 1);
    x1 = clamp(x1, 0, abs(width) - 1);
    y0 = clamp(y0, 0, abs(height) - 1);
    y1 = clamp(y1, 0, abs(height) - 1);

	int x, y;
	for (y = y0; y < y1; y++)//y区域的行数
	{
		for (x = x0; x < x1; x++)//x区域的列数
		{
            if (y*SCREEN_WIDTH+x<0||y*SCREEN_WIDTH+x>=SCREEN_WIDTH * SCREEN_HEIGHT)
            {
                continue;
            }
			if (*(occupiedPixel + y * SCREEN_WIDTH + x) != priority)
			{
				continue;
			}
			int color = GetColorInBmp(x, y, backGround);
			if (*(plcd + y * info.xres + x) != color)
			{
				XY_Color(x, y, color);
			}
		}
	}
}

void FreeBackgroundTemp()
{
	free(occupiedPixel);
}

void OpenBackGround(char* back)//打开一张背景图。打开后可以使用移动函数、刷新函数等需要背景图片的函数
{
	backGround = InitObj(back, 0, 0, 0);
	int fd = open(back, O_RDONLY);
	if (fd == -1)
	{
		perror("bmpOp");
	}
	occupiedPixel = (char*)malloc(sizeof(char) * SCREEN_WIDTH * SCREEN_HEIGHT);
	//将背景图的颜色信息存入缓存区中
	close(fd);
	BmpDraw(backGround, -1);
}

void BmpDraw(obj object, int filterColor)
{
	int x0 = object.x, y0 = object.y;
	int color;
	int x, y;
	for (y = y0; y < y0 + abs(object.height); y++)
	{
		for (x = x0; x < x0 + abs(object.width); x++)
		{
			if (x > SCREEN_WIDTH || x<0 || y>SCREEN_HEIGHT-1 || y < 0)
			{
				continue;
			}
			color = GetColorInBmp(x - x0, y - y0, object);//获取obj的颜色
			if (color != filterColor)
			{
				*(occupiedPixel + y * SCREEN_WIDTH + x) = object.priority;
				XY_Color(x, y, color);
			}
			else if (*(occupiedPixel + y * SCREEN_WIDTH + x) == object.priority)//刷新成背景色
			{
				color = GetColorInBmp(x, y,backGround);
				if (*(plcd + y * info.xres + x) != color)//与背景色不一样的像素点
				{
					XY_Color(x, y, color);
				}
			}
		}
	}
}

//在颜色数组中找到bmp文件x,y点的颜色。返回颜色值.object是用来获取bmp文件的信息的
int GetColorInBmp(int x, int y, obj object)
{
    unsigned char a, r, g, b;
    int width = object.width, height = object.height;
    int realx0 = width > 0 ? x : (width - x);
    int realy0 = height > 0 ? (height - y) : y;
    realy0--;
    if (realx0 > width || realx0<0 || realy0>height || realy0 < 0)
    {
        return 0xFF;
    }
    int i = (realx0*3) + realy0 * (3*width+object.fill);

    b = object.pixel[i++];
    g = object.pixel[i++];
    r = object.pixel[i++];
    if (object.depth == 32)
    {
        a = object.pixel[i++];
    }
    else
    {
        a = 0;
    }
    return (a << 24) | (r << 16) | (g << 8) | b;
}