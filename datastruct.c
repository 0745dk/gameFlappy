#include "datastruct.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "stdlib.h"
unsigned char** record;
int objNums;
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

void GetPixelInBmp(obj* object){
    int fd = open(object->pic, O_RDONLY);
    if (fd == -1)
    {
        perror("bmpOp");
    }
    int valid_bytes_per_line; //每一行有效的数据字节数
    int laizi = 0; // 每一行末尾的填充的“赖子”数
    object->fill=0;
    int total_bytes_per_line; //每一行实际的字节数.
    int total_bytes; //整个像素数组的字节数
    valid_bytes_per_line = abs(object->width) * (object->depth / 8);
    if (valid_bytes_per_line % 4)
    {
        laizi = 4 - valid_bytes_per_line % 4;
        object->fill = laizi;
    }
    total_bytes_per_line = valid_bytes_per_line + laizi;
    total_bytes = abs(object->height) * total_bytes_per_line;
    unsigned char* pixel = (unsigned char*)malloc(total_bytes); // 要free
    lseek(fd, 54, SEEK_SET);
    read(fd, pixel, total_bytes);
    object->pixel = pixel;
    close(fd);
}
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
        return newObj;
    }
    unsigned char buf[4];
    int width, height,depth;
    lseek(fd, 28, SEEK_SET);
    int f = read(fd, &buf, 2);
    if (f == -1)
    {
        perror("read depth error");
        return newObj;
    }
    depth = (buf[1]) << 8 | (buf[0]);
    if (depth != 24 && depth != 32)
    {
        printf("plz give me a 24bit or 32bit bmp file. depth=%d\n name=%s",depth,path);
        close(fd);
        return newObj;
    }
    lseek(fd, 0x12, SEEK_SET);
    f = read(fd, &buf, 4);
    if (f == -1)
    {
        perror("read width error");
        return newObj;
    }
    width = (buf[3] << 24) | (buf[2] << 16) | (buf[1]) << 8 | (buf[0]);

    f = read(fd, &buf, 4);
    if (f == -1)
    {
        perror("read height error");
        return newObj;
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
    GetPixelInBmp(&newObj);
    close(fd);
    return newObj;
}

void FreeAllObj(){
    for (int i = 0; i < objNums; ++i) {
        free(record[i]);
    }
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