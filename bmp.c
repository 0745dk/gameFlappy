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
obj backGround;
unsigned char* backgroundPixel = NULL;
extern struct fb_var_screeninfo info;
extern int* plcd;
char* occupiedPixel;//图片中已经被占用的像素点,这些像素点不会被刷新.数字的大小表示图片的优先级
#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)>(b))?(b):(a))


/*刷新背景的某个区域到屏幕上。

(x0,y0)
*---------------------
|					 |
|					 |
|					 |
|	  要刷新的区域	 |
|					 |
|					 |
|					 |
|                    |
---------------------*
					(x1,y1)
priority:要刷新的区域的优先级.优先级相同的才会刷新。
*/
void RefreshBackground(int x0, int y0, int x1, int y1, int priority)
{
	int width = backGround.width, height = backGround.height, depth = backGround.height;
	x0 = x0 > abs(width) - 1 ? abs(width) - 1 : x0;
	x1 = x1 > abs(width) - 1 ? abs(width) - 1 : x1;
	y0 = y0 > abs(height) - 1 ? abs(height) - 1 : y0;
	y1 = y1 > abs(height) - 1 ? abs(height) - 1 : y1;

	int x, y;
	for (y = y0; y < y1; y++)//y区域的行数
	{
		for (x = x0; x < x1; x++)//x区域的列数
		{
			if (*(occupiedPixel + y * backGround.width + x) != priority)
			{
				continue;
			}
			int color = GetColorInBmp(x, y, backgroundPixel, backGround);
			if (*(plcd + y * info.xres + x) != color)
			{
				XY_Color(x, y, color);
			}
		}
	}
}

void FreeBackgroundTemp()
{
	free(backgroundPixel);
}

void OpenBackGround(char* back)//打开一张背景图。打开后可以使用移动函数、刷新函数等需要背景图片的函数
{
	backGround = InitObj(back, 0, 0, 0);
	int fd = open(back, O_RDONLY);
	if (fd == -1)
	{
		perror("bmpOp");
	}
	occupiedPixel = (char*)malloc(sizeof(char) * backGround.width * backGround.height);
	int valid_bytes_per_line; //每一行有效的数据字节数
	int laizi = 0; // 每一行末尾的填充的“赖子”数
	int total_bytes_per_line; //每一行实际的字节数.
	int total_bytes; //整个像素数组的字节数

	valid_bytes_per_line = abs(backGround.width) * (backGround.depth / 8);
	if (valid_bytes_per_line % 4)
	{
		laizi = 4 - valid_bytes_per_line % 4;
	}
	total_bytes_per_line = valid_bytes_per_line + laizi;
	total_bytes = abs(backGround.height) * total_bytes_per_line;

	backgroundPixel = (unsigned char*)malloc(total_bytes);
	lseek(fd, 54, SEEK_SET);
	read(fd, backgroundPixel, total_bytes);
	//将背景图的颜色信息存入缓存区中
	close(fd);
	BmpDraw(backGround, -1);
}

void BmpDraw(obj object, int filterColor)
{
	int x0 = object.x, y0 = object.y;
	if (x0 > backGround.width || x0 < -object.width || y0 > backGround.height || y0 < -object.height)
	{
		return;
	}

	int fd = open(object.pic, O_RDONLY);
	if (fd == -1)
	{
		perror("bmpOp");
	}
	int valid_bytes_per_line; //每一行有效的数据字节数
	int laizi = 0; // 每一行末尾的填充的“赖子”数
	int total_bytes_per_line; //每一行实际的字节数.
	int total_bytes; //整个像素数组的字节数
	valid_bytes_per_line = abs(object.width) * (object.depth / 8);
	if (valid_bytes_per_line % 4)
	{
		laizi = 4 - valid_bytes_per_line % 4;
	}
	total_bytes_per_line = valid_bytes_per_line + laizi;
	total_bytes = abs(object.height) * total_bytes_per_line;
	unsigned char* pixel = (unsigned char*)malloc(total_bytes);
	lseek(fd, 54, SEEK_SET);
	read(fd, pixel, total_bytes);
	// 解析像素数据，并在屏幕上显示

	int color;
	int x, y;
	for (y = y0; y < y0 + abs(object.height); y++)
	{
		for (x = x0; x < x0 + abs(object.width); x++)
		{
			if (x > backGround.width || x<0 || y>backGround.height-1 || y < 0)
			{
				continue;
			}
			color = GetColorInBmp(x - x0, y - y0, pixel, object);//获取obj的颜色
			if (color != filterColor)
			{
				//printf("%d,%d\n", x, y);
				*(occupiedPixel + y * backGround.width + x) = object.priority;
				XY_Color(x, y, color);
			}
			else if (*(occupiedPixel + y * backGround.width + x) == object.priority)//刷新成背景色
			{
				color = GetColorInBmp(x, y, backgroundPixel, backGround);
				if (*(plcd + y * info.xres + x) != color)//与背景色不一样的像素点
				{
					XY_Color(x, y, color);
				}
			}
		}
	}
	free(pixel);
	close(fd);
}

//在颜色数组中找到bmp文件x,y点的颜色。返回颜色值.object是用来获取bmp文件的信息的
int GetColorInBmp(int x, int y, unsigned char* pixel, obj object)
{
	unsigned char a, r, g, b;
	int width = object.width, height = object.height, depth = object.height;
	int realx0 = width > 0 ? x : (width - x);
	int realy0 = height > 0 ? (height - y) : y;
	realy0--;
	if (realx0 > width || realx0<0 || realy0>height || realy0 < 0)
	{
		return -1;
	}
	int i = (realx0 + realy0 * width) * 3;

	b = pixel[i++];
	g = pixel[i++];
	r = pixel[i++];
	if (object.depth == 32)
	{
		a = pixel[i++];
	}
	else
	{
		a = 0;
	}
	return (a << 24) | (r << 16) | (g << 8) | b;
}