#define _CRT_SECURE_NO_WARNINGS // VS用scanf()函数才需要的定义

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int M; //总进程数
int N; //资源种类
int ALL_RESOURCE[10];//各种资源的数目总和
int MAX[10][10]; //M个进程对N类资源最大资源需求量
int AVAILABLE[10]; //系统可用资源数
int ALLOCATION[10][10]; //M个进程已经得到N类资源的资源量
int NEED[10][10]; //M个进程还需要N类资源的资源量
int Request[10]; //请求资源个数
void inputdata() {
	int i, j;
	printf("请输入总进程数:");
	scanf("%d", &M);
	printf("请输入资源种类数:");
	scanf("%d", &N);
	printf("请输入各种资源的总数量(all):");
	for (j = 0; j < N; j++) {
		scanf("%d", &ALL_RESOURCE[j]);
	}

	printf(" 请输入各进程已经得到的资源量(allocation): \n\n");
	for (i = 0; i < M; i++)
	{
		printf("进程p%d:    ", i);
		for (j = 0; j < N; j++) {
			scanf("%d", &ALLOCATION[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf(" 请输入M个进程对N类资源最大资源需求量(MAX):\n\n");
	for (i = 0; i < M; i++) {
		for (i = 0; i < M; i++)
		{
			printf("进程p%d:   ", i);
			for (j = 0; j < N; j++) {
				scanf("%d", &MAX[i][j]);
			}
			printf("\n");
		}
	}

	printf("\n");
}

void showdata() //函数showdata,输出资源分配情况
{
	int i, j;
	printf("各种资源的总数量(all):[");
	for (j = 0; j < N; j++) {
		printf(" %d", ALL_RESOURCE[j]);
	}
	printf(" ]\n\n");

	printf("系统目前各种资源可用的数为(available):[");

	for (j = 0; j < N; j++) {
		printf(" %d", AVAILABLE[j]);
	}
	printf(" ]\n\n");

	printf(" 各进程已经得到的资源量(allocation): \n\n");
	for (j = 0; j < N; j++) {
		printf("       资源%d", j);
	}
	printf("\n");
	for (i = 0; i < M; i++)
	{
		printf("进程p%d:    ", i);
		for (j = 0; j < N; j++) {
			printf("%d       ", ALLOCATION[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf(" 各进程还需要的资源量(need):\n\n");
	for (j = 0; j < N; j++) {
		printf("       资源%d", j);
	}
	printf("\n");
	for (i = 0; i < M; i++) {
		for (i = 0; i < M; i++)
		{
			printf("进程p%d:   ", i);
			for (j = 0; j < N; j++) {
				printf("%d        ", NEED[i][j]);
			}
			printf("\n");
		}
	}

	printf("\n");
}

void changdata(int k) //分配资源
{
	int j;
	for (j = 0; j < N; j++)
	{
		AVAILABLE[j] = AVAILABLE[j] - Request[j];
		ALLOCATION[k][j] = ALLOCATION[k][j] + Request[j];
		NEED[k][j] = NEED[k][j] - Request[j];
	}
}

void rstordata(int k) //恢复现场
{
	int j;
	for (j = 0; j < N; j++)
	{
		AVAILABLE[j] = AVAILABLE[j] + Request[j];
		ALLOCATION[k][j] = ALLOCATION[k][j] - Request[j];
		NEED[k][j] = NEED[k][j] + Request[j];
	}
}


int chkerr(int s) //函数chkerr,检查是否安全
{
	int i = s;
	int flag = 0;
	int* FINISH = (int*)malloc(sizeof(int) * M);
	int* WORK = (int*)malloc(sizeof(int) * N);
	int* array = (int*)malloc(sizeof(int) * M);
	int cnt = 0;
	int j;

	memset(FINISH, 0, sizeof(FINISH));
	memset(WORK, 0, sizeof(WORK));
	printf("系统当前可用资源：");
	for (j = 0; j < N; j++)
	{
		WORK[j] = AVAILABLE[j]; // 各分配的资源回收
		printf("%d ", WORK[j]);
	}
	printf("\n");
	{
		int j;
		for (j = 0; j < N; j++) { // 资源检查
			if (NEED[i][j] > AVAILABLE[j]) { // 进程i所需要的资源j能否被满足
				flag = 1; // 有不满足的，就退出
				break;
			}
		}
	}

	if (flag == 0) {
		int j;
		FINISH[i] = 1; //i号进程，可以执行
		printf("进程p%d执行\n", i);
		printf("系统当前可用资源：");
		for (j = 0; j < N; j++)
		{
			WORK[j] += ALLOCATION[i][j]; // 各分配的资源回收
			printf("%d ", WORK[j]);
		}
		printf("\n");
		array[cnt++] = i;
	}

	while (1) {
		int i;
		int flag1 = 0;

		for (i = 0; i < M; ) {
			int sum = 0;
			int k;
			for (k = 0; k < M; k++) {
				sum += FINISH[k];
			}
			if (sum == M) {
				int ij;
				printf("存在安全序列：");
				for (ij = 0; ij < M; ij++) {
					printf("p%d ", array[ij]);
				}
				printf("\n");
				printf("【经安全性检查，系统安全，本次分配成功。】\n");
				printf("\n");
				return 1;
			}

			if (FINISH[i] != 1) {
				flag1 = 0;
				for (int j = 0; j < N; j++) { // 资源检查
					if (NEED[i][j] > WORK[j]) { // 进程i所需要的资源j能否被满足
						flag1 = 1; // 有不满足的，就退出
						break;
					}
				}
			}

			if (flag1 == 0 && FINISH[i] != 1) { // 表示i进程没执行，且可以执行
				int j;
				FINISH[i] = 1; //i号进程，可以执行
				printf("进程p%d执行\n", i);
				printf("系统当前可用资源：");
				for (j = 0; j < N; j++)
				{
					WORK[j] += ALLOCATION[i][j]; // 各分配的资源回收
					printf("%d ", WORK[j]);
				}
				printf("\n");
				array[cnt++] = i;
				i = 0;
			}
			else { // 不能执行判断下一个进程
				i++;
			}
		}
		printf("\n");
		printf("【系统不安全!!! 本次资源申请不成功!!!】\n");
		printf("\n");
		return 0;

	}
}

void bank()   //银行家算法主体
{
	int i = 0, j = 0;
	char flag = 'Y';

	while (flag == 'Y' || flag == 'y')
	{
		i = -1;
		while (i < 0 || i >= M)
		{
			printf(" 请输入需申请资源的进程号（从P0到P%d，否则重输入!）:", M - 1);
			printf("P");
			scanf("%d", &i);
			while (i < 0 || i >= M) {
				printf(" 输入的进程号不存在，重新输入!\n");
				printf(" 请输入需申请资源的进程号（从P0到P%d，否则重输入!）:", M - 1);
				printf("P");
				scanf("%d", &i);
			}
			printf(" 请输入进程P%d申请的资源数:\n", i);
			for (j = 0; j < N; j++)
			{
				printf(" 资源%d:", j);
				scanf("%d", &Request[j]);
				if (Request[j] > NEED[i][j]) //若请求的资源数大于进程还需要i类资源的资源量j
				{
					printf(" 进程P%d申请的资源数大于进程P%d还需要%d类资源的资源量!", i, i, j);
					printf("申请不合理，出错!请重新选择!\n\n");
					flag = 'N';
					break;
				}
				else
				{
					if (Request[j] > AVAILABLE[j]) //若请求的资源数大于可用资源数
					{
						printf(" 进程P%d申请的资源数大于系统可用%d类资源的资源量!", i, j);
						printf("申请不合理，出错!请重新选择!\n\n");
						flag = 'N';
						break;
					}
				}
			}
			if (flag == 'Y' || flag == 'y')
			{
				changdata(i); //调用changdata(i)函数，改变资源数
				if (chkerr(i)) //若系统安全
				{

					printf("\n");
					printf(" 是否继续银行家算法演示,按'Y'或'y'键继续,按'N'或'n'键退出演示: ");
					scanf("%*c%c", &flag);
					rstordata(i); //调用rstordata(i)函数，恢复资源数
					showdata();   //输出资源分配情况
				}
				else {      //若系统不安全

					printf("\n");
					printf(" 是否继续银行家算法演示,按'Y'或'y'键继续,按'N'或'n'键退出演示: ");

					scanf("%*c%c", &flag);
					showdata(); //输出资源分配情况
				}
			}
			else      //若flag=N||flag=n
			{
				printf("\n");
				printf(" 是否继续银行家算法演示,按'Y'或'y'键继续,按'N'或'n'键退出演示: ");

				scanf("%*c%c", &flag);
				showdata();
			}

		}
	}
}

//主函数
int main()
{
	int i = 0, j = 0, p;
	//初始化资源数量
	inputdata();
	for (j = 0; j < N; j++)
	{
		p = ALL_RESOURCE[j];
		for (i = 0; i < M; i++)
		{
			p = p - ALLOCATION[i][j];//减去已经被占据的资源
			AVAILABLE[j] = p;
			if (AVAILABLE[j] < 0)
				AVAILABLE[j] = 0;
		}
	}
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++) {
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
		}
	}

	showdata();
	bank();


	return 0;
}
