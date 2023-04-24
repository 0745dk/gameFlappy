#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include "screen.h"
#include "bmp.h"

int* plcd;
int sfd;
struct fb_var_screeninfo info;

void LcdOp()
{
	sfd = open("/dev/fb0", O_RDWR);
	if (sfd == -1)
	{
		perror("lcdfd_OP");
		return;
	}//打开触摸屏文件



	int ret = ioctl(sfd, FBIOGET_VSCREENINFO, &info);
	if (ret == -1)
	{
		perror("lcdfd_Info");
		close(sfd);
		return;
	}//获取信息
	

	int length = info.xres * info.yres * info.bits_per_pixel / 8;
	plcd = (int*)mmap(NULL, length, PROT_WRITE, MAP_SHARED, sfd, 0);
	if (plcd == MAP_FAILED)
	{
		perror("lcdfd_mmap");
		close(sfd);
		return;
	}//映射

}

void LcdClos()
{
	int length = info.xres * info.yres * info.bits_per_pixel / 8;
	munmap(plcd, length);
	close(sfd);
}
//将显示屏变为一种颜色。plcd就是映射的指针。
void Fill_Color(int color)
{
	int i, j;
	int colors[480][800];

	for (i = 0; i < 480; i++)
	{
		for (j = 0; j < 800; j++)
		{
			*(plcd + i * 800 + j) = color;
		}
	}
}

void DrawCircle(int x, int y, int r, int color)
{
	int i, j;
	for (i = y - r; i < y + r; i++)
	{
		for (j = x - r; j < x + r; j++)
		{
			if ((i - y) * (i - y) + (j - x) * (j - x) < r * r)
			{
				*(plcd + i * info.xres + j) = color;
			}
		}
	}
}

void XY_Color(int x, int y, int color)
{
	if (y >= 0 && y < info.yres && x >= 0 && x < info.xres)
	{
		*(plcd + y * info.xres + x) = color;
	}
}



