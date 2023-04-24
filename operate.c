#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include "thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "screen.h"
extern int touched;
extern int terminate;

void* Touch(void* arg)
{
	pthread_detach(pthread_self());
	arguments* realarg = arg;
	int x0 = realarg->x0;
	int y0 = realarg->y0;
	int x1 = realarg->x1;
	int y1 = realarg->y1;

	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
	{
		perror("failed to open /dev/input/event0");
		return;
	}

	int x = -1, y = -1;
	while (!terminate)
	{
		struct input_event ev;
		int  ret = read(fd, &ev, sizeof(ev));
		if (ret != sizeof(ev))
		{
			continue;
		}

		if (ev.type == EV_ABS && ev.code == ABS_X)
		{
			if (x == -1 && ev.value != 0)
			{
				x = ev.value;
			}
		}
		if (ev.type == EV_ABS && ev.code == ABS_Y)
		{
			if (y == -1 && ev.value != 0)
			{
				y = ev.value;
			}
		}

		if ((ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0) ||
			(ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0))
		{
			x = x * 800.0 / 1024;
			y = y * 480.0 / 600;

			if (x > x0 && x < x1 && y>y0 && y < y1)
			{
				touched = 1;
				x = -1, y = -1;
			}
			else
			{
				touched = 0;
				x = -1, y = -1;
			}
		}
	}
	free(realarg);
}

int TouchForMainThread(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
	{
		perror("failed to open /dev/input/event0");
		return;
	}

	int x = -1, y = -1;

	struct input_event ev;
	while (1)
	{
		int  ret = read(fd, &ev, sizeof(ev));
		if (ret != sizeof(ev))
		{
			continue;
		}
		if (ev.type == EV_ABS && ev.code == ABS_X)
		{
			if (x == -1 && ev.value != 0)
			{
				x = ev.value;
			}
		}
		if (ev.type == EV_ABS && ev.code == ABS_Y)
		{
			if (y == -1 && ev.value != 0)
			{
				y = ev.value;
			}
		}

		if ((ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0) ||
			(ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0))
		{
			x = x * 800.0 / 1024;
			y = y * 480.0 / 600;
			if (x > x0 && x < x1 && y>y0 && y < y1)
			{
				return 1;
			}
			else if (x > x2 && x < x3 && y>y2 && y < y3)
			{
				return 0;
			}
			else
			{
				x = -1;
				y = -1;
			}
		}
	}
}