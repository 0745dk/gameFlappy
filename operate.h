#ifndef __OPERATE_H__
#define __OPERATE_H__


typedef struct point
{
    int x; //点击“点”的x轴坐标
    int y; //点击“点”的y轴坐标

} ts_point;

/*触摸到下面指定的区域,返回1。否则返回0

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
*/
void* Touch(void* arg);//触摸屏事件
int TouchForMainThread(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);

#endif // !__OPERATE_H__
