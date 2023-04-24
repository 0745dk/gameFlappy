#include "datastruct.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "bmp.h"

obj InitObj(char* path,int x0,int y0,int priority)
{
	obj newObj;
	newObj.x = x0;
	newObj.y = y0;
	newObj.pic = path;
	newObj.priority = priority;
	
	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("bmpOp");
	}
	unsigned char buf[4];
	int width, height,depth;
	lseek(fd, 28, SEEK_SET);
	int f = read(fd, &buf, 2);
	if (f == -1)
	{
		perror("read depth error");
	}
	depth = (buf[1]) << 8 | (buf[0]);
	if (depth != 24 && depth != 32)
	{
		printf("plz give me a 24bit or 32bit bmp file.\n");
		close(fd);
		return;
	}
	lseek(fd, 0x12, SEEK_SET);
	f = read(fd, &buf, 4);
	if (f == -1)
	{
		perror("read width error");
	}
	width = (buf[3] << 24) | (buf[2] << 16) | (buf[1]) << 8 | (buf[0]);

	f = read(fd, &buf, 4);
	if (f == -1)
	{
		perror("read height error");
	}
	height = (buf[3] << 24) | (buf[2] << 16) | (buf[1]) << 8 | (buf[0]);
	newObj.width = width;
	newObj.height = height;
	newObj.depth = depth;
	newObj.hitBox[0] = 0;
	newObj.hitBox[1] = 0;
	newObj.hitBox[2] = width; 
	newObj.hitBox[3] = height;
	newObj.hitBox[4] = -1;
	newObj.hitBox[5] = -1;
	newObj.hitBox[6] = -1;
	newObj.hitBox[7] = -1;
	close(fd);
	return newObj;

}

void ReHitBox(obj* object, int* re,int n)
{
	if (n>8)
	{
		printf("To many elements(more than 8)\n");
		return;
	}
	int i;
	for (i = 0; i < n; i++)
	{
		object->hitBox[i] = *(re + i);
	}
}