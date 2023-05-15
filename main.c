#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"
#include "bmp.h"
#include "character.h"
#include "operate.h"
#include "action.h"
#include "datastruct.h"
#include "gameUI.h"
#include <time.h>
#include <pthread.h>
#include "thread.h"

#include <string.h>

int touched = 0;//一个全局变量,用于记录是否按到了屏幕
int terminate = 0;//记录进程的状态。terminate=1时，游戏结束

void Dbg(int num){
    printf("%d          \n",num);
    fflush(stdout);
}
int main()
{
	static pthread_t th1;
	static arguments* arg;
	arg = (arguments*)malloc(sizeof(*arg));
	arg->x0 = 0;
	arg->y0 = 0;
	arg->x1 = 800;
	arg->y1 = 480;

	if (pthread_create(&th1, NULL, Touch, arg))
		free(arg);

	srand((unsigned)time(NULL));//设置随机函数的种子
	LcdOp();//打开Lcd
start:
    OpenBackGround("./background.bmp");//打开背景
	obj bird = InitObj("./bird.bmp", 195, 232, 3);// 195, 232
	BmpDraw(bird, 0xFFFFFF);//初始化小鸟并绘制小鸟图片
    obj ground = InitObj("./ground.bmp", 0, 440, 1);
	BmpDraw(ground, 0xFFFFFF);//初始化大地并绘制大地图片
	obj Pipe1 = InitObj("./pipe.bmp", 800, -(110 + rand() % 286), 2);
	obj Pipe2 = InitObj("./pipe.bmp", 1066, -(110 + rand() % 286), 2);
	obj Pipe3 = InitObj("./pipe.bmp", 1332, -(110 + rand() % 286), 2);
    obj Pipe4 = InitObj("./pipe.bmp", 1999, -99, 2);
	//初始化所有的管道,y轴坐标是一个在-369~-90的一个随机值(这样才能让管子的缝隙在屏幕之间)

	int re[8] = { 0,0,51,402,0,538,51,917 };
	ReHitBox(&Pipe1, re, 8);
	ReHitBox(&Pipe2, re, 8);
	ReHitBox(&Pipe3, re, 8);
	ReHitBox(&Pipe4, re, 8);
	//重新设置管道的碰撞体积(因为管道可通过的地方在没有重新设置碰撞体积时,是无法通过的)

	int times = 0;
	//7"40 times = 287 记录的变量,用于记录游戏的时间,这个变量被用于计算重力加速度(1s = 37times)

	int sp4 = 0;//管道4的速度
	int v = 0;//小鸟的向下的速度
	int a;//小鸟向上的加速度
	int v0;
	int score = 0;
	while (!terminate)
	{
		if (Hit(bird, ground) || Hit(bird, Pipe1) || Hit(bird, Pipe2) || Hit(bird, Pipe3) || Hit(bird, Pipe4) || bird.y < -20)
		{
			if (GameOver())
			{
                FreeBackgroundTemp();
                goto start;
			}
			else
			{
				terminate = 0;
				Fill_Color(0);
				break;
			}
		}//当鸟撞到大地、管道时,游戏失败
		DisplayNum(0, 0, score);//记录分数用,在屏幕左上角
		a = 0;
		if (touched)
		{
			a = 20;
			touched = 0;
			times = 0;
		}
		v0 = v;
		v = Acceleration(times, v0, a);//计算v
		Mov(&bird, v, Down);
		Mov(&ground, 3, Left);
		Mov(&Pipe1, 3, Left);
		Mov(&Pipe2, 3, Left);
		Mov(&Pipe3, 3, Left);
		Mov(&Pipe4, sp4, Left);
		usleep(1000);//控制游戏的速度至人眼能适应的程度
		times++;
		//循环对象的使用
		if (ground.x < -1047)
		{
			MoveObjTo(&ground, 0, 440);
		}
		if (Pipe1.x < 0 && Pipe1.x > -4)
		{
			Pipe4.x = 800;
			Pipe4.y = -(110 + rand() % 286);
			sp4 = 3;
		}
		if (Pipe2.x < 0 && Pipe2.x > -4)
		{
			Pipe1.x = 800;
			Pipe1.y = -(110 + rand() % 286);
		}
		if (Pipe3.x < 0 && Pipe3.x > -4)
		{
			Pipe2.x = 800;
			Pipe2.y = -(110 + rand() % 286);
		}
		if (Pipe4.x < 0 && Pipe4.x > -4)
		{
			Pipe3.x = 800;
			Pipe3.y = -(110 + rand() % 286);
		}

		if (Pipe1.x < 0 + 195 && Pipe1.x > -4 + 195)
		{
			score++;
		}
		if (Pipe2.x < 0 + 195 && Pipe2.x > -4 + 195)
		{
			score++;
		}
		if (Pipe3.x < 0 + 195 && Pipe3.x > -4 + 195)
		{
			score++;
		}
		if (Pipe4.x < 0 + 195 && Pipe4.x > -4 + 195)
		{
			score++;
		}
	}
    FreeAllObj();
	LcdClos();
	FreeBackgroundTemp();
}


