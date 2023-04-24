#ifndef __DATASTRUCT_H__
#define __DATASTRUCT_H__


typedef struct object
{
	//x,y:对象在屏幕中的位置
	int x;
	int y;

	char* pic;//对象的图片
	int width;//宽
	int height;//高
	int depth;//深度
	int priority;//在屏幕中显示的优先级

	int hitBox[8];//Obj的碰撞体积,在初始化时默认设置为bmp的尺寸。可以通过ReHitBox重新设置。

}obj;


obj InitObj(char* path, int x, int y, int priority);//初始化一个对象。但是碰撞体积设置位NULL
void ReHitBox(obj* object, int* re, int n);//object:要改变的对象。re:储存区域坐标的数组。n:数组的元素个数


#endif // !__DATASTRUCT_H__
