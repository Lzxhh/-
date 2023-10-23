/*//Peterson�㷨
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#define COUNT 100

int flag[2] = { 0, 0 };
int nLoop0 = 0, nLoop1 = 0;//ת�˴���
int nTemp1, nTemp2, nRandom;//�˻���ʱ�洢1��2  ת�������
int nAccount1 = 0, nAccount2 = 0;//�˻�1��2
int turn;

DWORD WINAPI Fun0(HANDLE Thread)
{
	do
	{
		flag[0] = 1;
		turn = 1;
		while ((flag[turn] == 1) && (turn == 1));
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1 current:%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2 current:%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("nAccount1 recive:%d,now: %d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("nAccount2 recive: %d,now: %d\n", nRandom, nAccount2);
		nLoop0++;//ת�˴���+1
		flag[0] = 0;//�����һ�����̵�ʹ������
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0 loop count%d ��\n", nLoop0);
	return 0;
}
DWORD WINAPI Fun1(HANDLE Thread)
{
	do
	{
		flag[1] = 1;
		turn = 0;
		while ((flag[turn] == 1) && (turn == 0));
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1 current:%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2 current:%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("nAccount1 recive: %d,now: %d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("nAccount2 recive: %d,now:%d\n", nRandom, nAccount2);
		nLoop1++;//ת�˴���+1
		flag[1] = 0;//�����һ�����̵�ʹ������
	}while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1 loop count: %d ��\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();//��ʼʱ��
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//�����߳�
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//�ȴ�
	endtime = GetTickCount();//����ʱ��
	printf("time��%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//�����߳�
	CloseHandle(Thread[1]);
    system("pause");
	return 0;
}*/

/*
//Windows
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define COUNT 1000000

int nLoop0 = 0,nLoop1 = 0;//ת�˴���
int nTemp1, nTemp2, nRandom;//�˻���ʱ�洢1��2  ת�������
int nAccount1 = 0, nAccount2 = 0;//�˻�1��2
HANDLE mutex;

DWORD WINAPI Fun0(HANDLE Thread)
{
	do
	{
		WaitForSingleObject(mutex, INFINITE);
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1��ǰ�ʽ�Ϊ��%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2��ǰ�ʽ�Ϊ��%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("nAccount1�յ��ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("nAccount2ת���ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount2);
		nLoop0++;//ת�˴���+1
		ReleaseMutex(mutex);
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0ѭ������Ϊ %d ��\n", nLoop0);
	return 0;
}

DWORD WINAPI Fun1(HANDLE Thread)
{
	do
	{
		WaitForSingleObject(mutex, INFINITE);
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1��ǰ�ʽ�Ϊ��%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2��ǰ�ʽ�Ϊ��%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("&&&nAccount1�յ��ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("&&&nAccount2ת���ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount2);
		nLoop1++;//ת�˴���+1
		ReleaseMutex(mutex);
	} while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1ѭ������Ϊ %d ��\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();
	mutex = CreateMutex(NULL, FALSE, NULL);
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//�����̺߳���
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//�ȴ�
	endtime = GetTickCount();
	printf("����ʱ��Ϊ��%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//�����߳�
	CloseHandle(Thread[1]);
	return 0;
}
*/
//Windows�޻���
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define COUNT 100

int nLoop0 = 0,nLoop1 = 0;//ת�˴���
int nTemp1, nTemp2, nRandom;//�˻���ʱ�洢1��2  ת�������
int nAccount1 = 0, nAccount2 = 0;//�˻�1��2

DWORD WINAPI Fun0(HANDLE Thread)
{
	do
	{
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1��ǰ�ʽ�Ϊ��%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2��ǰ�ʽ�Ϊ��%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("nAccount1�յ��ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("nAccount2ת���ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount2);
		nLoop0++;//ת�˴���+1
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0ѭ������Ϊ %d ��\n", nLoop0);
	return 0;
}

DWORD WINAPI Fun1(HANDLE Thread)
{
	do
	{
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1��ǰ�ʽ�Ϊ��%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2��ǰ�ʽ�Ϊ��%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//�˻�1ת��
		printf("nAccount1�յ��ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//�˻�2ת��
		printf("nAccount2ת���ʽ� %d�����ʽ�Ϊ��%d\n", nRandom, nAccount2);
		nLoop1++;//ת�˴���+1
	} while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1ѭ������Ϊ %d ��\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//�����̺߳���
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//�ȴ�
	endtime = GetTickCount();
	printf("����ʱ��Ϊ��%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//�����߳�
	CloseHandle(Thread[1]);
	return 0;
}
