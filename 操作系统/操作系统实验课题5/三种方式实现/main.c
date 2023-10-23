/*//Peterson算法
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#define COUNT 100

int flag[2] = { 0, 0 };
int nLoop0 = 0, nLoop1 = 0;//转账次数
int nTemp1, nTemp2, nRandom;//账户临时存储1、2  转账随机数
int nAccount1 = 0, nAccount2 = 0;//账户1、2
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
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("nAccount1 recive:%d,now: %d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("nAccount2 recive: %d,now: %d\n", nRandom, nAccount2);
		nLoop0++;//转账次数+1
		flag[0] = 0;//解除另一个进程的使用限制
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0 loop count%d 次\n", nLoop0);
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
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("nAccount1 recive: %d,now: %d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("nAccount2 recive: %d,now:%d\n", nRandom, nAccount2);
		nLoop1++;//转账次数+1
		flag[1] = 0;//解除另一个进程的使用限制
	}while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1 loop count: %d 次\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();//开始时间
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//创建线程
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//等待
	endtime = GetTickCount();//结束时间
	printf("time：%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//结束线程
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

int nLoop0 = 0,nLoop1 = 0;//转账次数
int nTemp1, nTemp2, nRandom;//账户临时存储1、2  转账随机数
int nAccount1 = 0, nAccount2 = 0;//账户1、2
HANDLE mutex;

DWORD WINAPI Fun0(HANDLE Thread)
{
	do
	{
		WaitForSingleObject(mutex, INFINITE);
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1当前资金为：%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2当前资金为：%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("nAccount1收到资金 %d，现资金为：%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("nAccount2转出资金 %d，现资金为：%d\n", nRandom, nAccount2);
		nLoop0++;//转账次数+1
		ReleaseMutex(mutex);
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0循环次数为 %d 次\n", nLoop0);
	return 0;
}

DWORD WINAPI Fun1(HANDLE Thread)
{
	do
	{
		WaitForSingleObject(mutex, INFINITE);
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1当前资金为：%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2当前资金为：%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("&&&nAccount1收到资金 %d，现资金为：%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("&&&nAccount2转出资金 %d，现资金为：%d\n", nRandom, nAccount2);
		nLoop1++;//转账次数+1
		ReleaseMutex(mutex);
	} while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1循环次数为 %d 次\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();
	mutex = CreateMutex(NULL, FALSE, NULL);
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//创建线程函数
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//等待
	endtime = GetTickCount();
	printf("运行时间为：%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//结束线程
	CloseHandle(Thread[1]);
	return 0;
}
*/
//Windows无互斥
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define COUNT 100

int nLoop0 = 0,nLoop1 = 0;//转账次数
int nTemp1, nTemp2, nRandom;//账户临时存储1、2  转账随机数
int nAccount1 = 0, nAccount2 = 0;//账户1、2

DWORD WINAPI Fun0(HANDLE Thread)
{
	do
	{
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1当前资金为：%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2当前资金为：%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("nAccount1收到资金 %d，现资金为：%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("nAccount2转出资金 %d，现资金为：%d\n", nRandom, nAccount2);
		nLoop0++;//转账次数+1
	} while (((nAccount1 + nAccount2) == 0) && (nLoop0 < COUNT));
	printf("Fun0循环次数为 %d 次\n", nLoop0);
	return 0;
}

DWORD WINAPI Fun1(HANDLE Thread)
{
	do
	{
		nRandom = rand() % 1000;
		nTemp1 = nAccount1;
		printf("nAccount1当前资金为：%d\n", nTemp1);
		nTemp2 = nAccount2;
		printf("nAccount2当前资金为：%d\n", nTemp2);
		nAccount1 = nTemp1 + nRandom;//账户1转入
		printf("nAccount1收到资金 %d，现资金为：%d\n", nRandom, nAccount1);
		nAccount2 = nTemp2 - nRandom;//账户2转出
		printf("nAccount2转出资金 %d，现资金为：%d\n", nRandom, nAccount2);
		nLoop1++;//转账次数+1
	} while (((nAccount1 + nAccount2) == 0) && (nLoop1 < COUNT));
	printf("Fun1循环次数为 %d 次\n", nLoop1);
	return 0;
}

int main()
{
	HANDLE Thread[2];
	DWORD starttime, endtime;
	starttime = GetTickCount();
	Thread[0] = CreateThread(NULL, 0, Fun0, NULL, 0, NULL);//创建线程函数
	Thread[1] = CreateThread(NULL, 0, Fun1, NULL, 0, NULL);
	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);//等待
	endtime = GetTickCount();
	printf("运行时间为：%ld ms\n", endtime - starttime);
	CloseHandle(Thread[0]);//结束线程
	CloseHandle(Thread[1]);
	return 0;
}
