#ifndef __DATASTRUCT_H__
#define __DATASTRUCT_H__


typedef struct object
{
	//x,y:��������Ļ�е�λ��
	int x;
	int y;

	char* pic;//�����ͼƬ
	int width;//��
	int height;//��
	int depth;//���
	int priority;//����Ļ����ʾ�����ȼ�

	int hitBox[8];//Obj����ײ���,�ڳ�ʼ��ʱĬ������Ϊbmp�ĳߴ硣����ͨ��ReHitBox�������á�

}obj;


obj InitObj(char* path, int x, int y, int priority);//��ʼ��һ�����󡣵�����ײ�������λNULL
void ReHitBox(obj* object, int* re, int n);//object:Ҫ�ı�Ķ���re:����������������顣n:�����Ԫ�ظ���


#endif // !__DATASTRUCT_H__
