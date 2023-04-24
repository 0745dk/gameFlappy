#ifndef __OPERATE_H__
#define __OPERATE_H__


typedef struct point
{
    int x; //������㡱��x������
    int y; //������㡱��y������

} ts_point;

/*����������ָ��������,����1�����򷵻�0

(x0,y0)
*---------------------
|					 |
|					 |
|					 |
|	  Ҫˢ�µ�����	 |
|					 |
|					 |
|					 |
|                    |
---------------------*
					(x1,y1)
*/
void* Touch(void* arg);//�������¼�
int TouchForMainThread(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);

#endif // !__OPERATE_H__
