
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "gy39.h"

//初始化串口
//file: 串口所对应的文件名
//baudrate：波特率
int init_serial(const char* file, int baudrate)
{
	int fd;

	fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open device error:");
		return -1;
	}

	struct termios myserial;
	//清空结构体
	memset(&myserial, 0, sizeof(myserial));
	//O_RDWR               
	myserial.c_cflag |= (CLOCAL | CREAD);
	//设置控制模式状态，本地连接，接受使能
	//设置 数据位
	myserial.c_cflag &= ~CSIZE;   //清空数据位
	myserial.c_cflag &= ~CRTSCTS; //无硬件流控制
	myserial.c_cflag |= CS8;      //数据位:8

	myserial.c_cflag &= ~CSTOPB;//   //1位停止位
	myserial.c_cflag &= ~PARENB;  //不要校验
	//myserial.c_iflag |= IGNPAR;   //不要校验
	//myserial.c_oflag = 0;  //输入模式
	//myserial.c_lflag = 0;  //不激活终端模式

	switch (baudrate)
	{
	case 9600:
		cfsetospeed(&myserial, B9600);  //设置波特率
		cfsetispeed(&myserial, B9600);
		break;
	case 115200:
		cfsetospeed(&myserial, B115200);  //设置波特率
		cfsetispeed(&myserial, B115200);
		break;
	case 19200:
		cfsetospeed(&myserial, B19200);  //设置波特率
		cfsetispeed(&myserial, B19200);
		break;
	case 38400:
		cfsetospeed(&myserial, B38400);  //设置波特率
		cfsetispeed(&myserial, B38400);
		break;

	}

	/* 刷新输出队列,清除正接受的数据 */
	tcflush(fd, TCIFLUSH);

	/* 改变配置 */
	tcsetattr(fd, TCSANOW, &myserial);

	return fd;
}

//解析并处理数据
void parse_data(unsigned char data[], int len)
{
	int i;

	//debug
	for (i = 0; i < len; i++)
	{
		printf("%02x ", data[i]);
	}
	printf("\n");

	if (data[0] != 0x5A || data[1] != 0x5A)
	{
		printf(" ERROR data\n");
		return;
	}

	int type = data[2];
	int l = data[3];


	if (type == 0x15)
	{
		int d = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | (data[7]);

		double lux = d / 100.0;

		printf("lux = %g\n", lux);

		//在屏幕上合适位置上，显示这个光照强度
	}
	else if (type == 0x45)
	{
		//解析： 温度、湿度、气压、海拔
		double T = ((data[4] << 8) | data[5]) / 100;
		double P = ((data[6] << 24) | (data[7] << 16) | (data[8] << 8) | (data[9])) / 100;
		double Hum = ((data[10] << 8) | data[11]) / 100;
		double H = ((data[12] << 8) | data[13]) / 100;
		printf("T = %g\nP = %g\nHum = %g\nH = %g\n", T, P, Hum, H);
	}

}


void* get_gy39_data(void* d)
{
	int ret;
	int fd = init_serial(COM2, 9600);
	if (fd == -1)
	{
		perror("Init_serial");
	}



	// 发送命令给  GY39模块。
	unsigned char cmd[3] = { 0xA5, 0X83, 0x28 };
	ret = write(fd, cmd, 3);



	unsigned char data[16] = { 0x5A, 0x5A };
	int i;


	while (1)
	{
		unsigned char ch;
		// 连续读取两个 0x5A    ,帧头
		while (1)
		{
			do
			{
				read(fd, &ch, 1);
			} while (ch != 0x5A);

			//
			read(fd, &ch, 1);
			if (ch != 0x5A)
			{
				continue;
			}
		}
		i = 2;

		read(fd, &data[i++], 1); // type
		read(fd, &data[i++], 1); // len
		int len = data[i - 1];
		int j;
		for (j = 0; j < len; j++)
		{
			read(fd, &data[i++], 1);
		}

		read(fd, &data[i++], 1); //checksum


		parse_data(data, i); //解析(处理)这一帧GY39的数据
		sleep(2);

	}

	close(fd);
}