#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//------Open����ͷ�ļ�-------

#include <stdio.h>
//------Perror����ͷ�ļ�-------

//#include <sys/types.h>
#include <unistd.h>
//------Lseek---------------

#include "screen.h"
//------ȫ�ֱ���----------

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
char* occupiedPixel;//��Ļ���Ѿ���ռ�õ����ص�,��Щ���ص㲻�ᱻˢ��.���ֵĴ�С��ʾͼƬ�����ȼ�
#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)>(b))?(b):(a))

int clamp(int x, int min,int max){
    return x > max ? max : x < min ? min : x;
}


/*ˢ�±�����ĳ��������Ļ�ϡ�

(x0,y0)
*-------------------
|					|
|	  Ҫˢ�µ�����	|
|					|
-------------------*
					(x1,y1)
priority:Ҫˢ�µ���������ȼ�.���ȼ���ͬ�ĲŻ�ˢ�¡�
*/
void RefreshBackground(int x0, int y0, int x1, int y1, int priority)
{
	int width = SCREEN_WIDTH, height = SCREEN_HEIGHT, depth = SCREEN_HEIGHT;
    x0 = clamp(x0, 0, abs(width) - 1);
    x1 = clamp(x1, 0, abs(width) - 1);
    y0 = clamp(y0, 0, abs(height) - 1);
    y1 = clamp(y1, 0, abs(height) - 1);

	int x, y;
	for (y = y0; y < y1; y++)//y���������
	{
		for (x = x0; x < x1; x++)//x���������
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

void OpenBackGround(char* back)//��һ�ű���ͼ���򿪺����ʹ���ƶ�������ˢ�º�������Ҫ����ͼƬ�ĺ���
{
	backGround = InitObj(back, 0, 0, 0);
	int fd = open(back, O_RDONLY);
	if (fd == -1)
	{
		perror("bmpOp");
	}
	occupiedPixel = (char*)malloc(sizeof(char) * SCREEN_WIDTH * SCREEN_HEIGHT);
	//������ͼ����ɫ��Ϣ���뻺������
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
			color = GetColorInBmp(x - x0, y - y0, object);//��ȡobj����ɫ
			if (color != filterColor)
			{
				*(occupiedPixel + y * SCREEN_WIDTH + x) = object.priority;
				XY_Color(x, y, color);
			}
			else if (*(occupiedPixel + y * SCREEN_WIDTH + x) == object.priority)//ˢ�³ɱ���ɫ
			{
				color = GetColorInBmp(x, y,backGround);
				if (*(plcd + y * info.xres + x) != color)//�뱳��ɫ��һ�������ص�
				{
					XY_Color(x, y, color);
				}
			}
		}
	}
}

//����ɫ�������ҵ�bmp�ļ�x,y�����ɫ��������ɫֵ.object��������ȡbmp�ļ�����Ϣ��
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