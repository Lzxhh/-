#define _CRT_SECURE_NO_WARNINGS // VS��scanf()��������Ҫ�Ķ���

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int M; //�ܽ�����
int N; //��Դ����
int ALL_RESOURCE[10];//������Դ����Ŀ�ܺ�
int MAX[10][10]; //M�����̶�N����Դ�����Դ������
int AVAILABLE[10]; //ϵͳ������Դ��
int ALLOCATION[10][10]; //M�������Ѿ��õ�N����Դ����Դ��
int NEED[10][10]; //M�����̻���ҪN����Դ����Դ��
int Request[10]; //������Դ����
void inputdata() {
	int i, j;
	printf("�������ܽ�����:");
	scanf("%d", &M);
	printf("��������Դ������:");
	scanf("%d", &N);
	printf("�����������Դ��������(all):");
	for (j = 0; j < N; j++) {
		scanf("%d", &ALL_RESOURCE[j]);
	}

	printf(" ������������Ѿ��õ�����Դ��(allocation): \n\n");
	for (i = 0; i < M; i++)
	{
		printf("����p%d:    ", i);
		for (j = 0; j < N; j++) {
			scanf("%d", &ALLOCATION[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf(" ������M�����̶�N����Դ�����Դ������(MAX):\n\n");
	for (i = 0; i < M; i++) {
		for (i = 0; i < M; i++)
		{
			printf("����p%d:   ", i);
			for (j = 0; j < N; j++) {
				scanf("%d", &MAX[i][j]);
			}
			printf("\n");
		}
	}

	printf("\n");
}

void showdata() //����showdata,�����Դ�������
{
	int i, j;
	printf("������Դ��������(all):[");
	for (j = 0; j < N; j++) {
		printf(" %d", ALL_RESOURCE[j]);
	}
	printf(" ]\n\n");

	printf("ϵͳĿǰ������Դ���õ���Ϊ(available):[");

	for (j = 0; j < N; j++) {
		printf(" %d", AVAILABLE[j]);
	}
	printf(" ]\n\n");

	printf(" �������Ѿ��õ�����Դ��(allocation): \n\n");
	for (j = 0; j < N; j++) {
		printf("       ��Դ%d", j);
	}
	printf("\n");
	for (i = 0; i < M; i++)
	{
		printf("����p%d:    ", i);
		for (j = 0; j < N; j++) {
			printf("%d       ", ALLOCATION[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf(" �����̻���Ҫ����Դ��(need):\n\n");
	for (j = 0; j < N; j++) {
		printf("       ��Դ%d", j);
	}
	printf("\n");
	for (i = 0; i < M; i++) {
		for (i = 0; i < M; i++)
		{
			printf("����p%d:   ", i);
			for (j = 0; j < N; j++) {
				printf("%d        ", NEED[i][j]);
			}
			printf("\n");
		}
	}

	printf("\n");
}

void changdata(int k) //������Դ
{
	int j;
	for (j = 0; j < N; j++)
	{
		AVAILABLE[j] = AVAILABLE[j] - Request[j];
		ALLOCATION[k][j] = ALLOCATION[k][j] + Request[j];
		NEED[k][j] = NEED[k][j] - Request[j];
	}
}

void rstordata(int k) //�ָ��ֳ�
{
	int j;
	for (j = 0; j < N; j++)
	{
		AVAILABLE[j] = AVAILABLE[j] + Request[j];
		ALLOCATION[k][j] = ALLOCATION[k][j] - Request[j];
		NEED[k][j] = NEED[k][j] + Request[j];
	}
}


int chkerr(int s) //����chkerr,����Ƿ�ȫ
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
	printf("ϵͳ��ǰ������Դ��");
	for (j = 0; j < N; j++)
	{
		WORK[j] = AVAILABLE[j]; // ���������Դ����
		printf("%d ", WORK[j]);
	}
	printf("\n");
	{
		int j;
		for (j = 0; j < N; j++) { // ��Դ���
			if (NEED[i][j] > AVAILABLE[j]) { // ����i����Ҫ����Դj�ܷ�����
				flag = 1; // �в�����ģ����˳�
				break;
			}
		}
	}

	if (flag == 0) {
		int j;
		FINISH[i] = 1; //i�Ž��̣�����ִ��
		printf("����p%dִ��\n", i);
		printf("ϵͳ��ǰ������Դ��");
		for (j = 0; j < N; j++)
		{
			WORK[j] += ALLOCATION[i][j]; // ���������Դ����
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
				printf("���ڰ�ȫ���У�");
				for (ij = 0; ij < M; ij++) {
					printf("p%d ", array[ij]);
				}
				printf("\n");
				printf("������ȫ�Լ�飬ϵͳ��ȫ�����η���ɹ�����\n");
				printf("\n");
				return 1;
			}

			if (FINISH[i] != 1) {
				flag1 = 0;
				for (int j = 0; j < N; j++) { // ��Դ���
					if (NEED[i][j] > WORK[j]) { // ����i����Ҫ����Դj�ܷ�����
						flag1 = 1; // �в�����ģ����˳�
						break;
					}
				}
			}

			if (flag1 == 0 && FINISH[i] != 1) { // ��ʾi����ûִ�У��ҿ���ִ��
				int j;
				FINISH[i] = 1; //i�Ž��̣�����ִ��
				printf("����p%dִ��\n", i);
				printf("ϵͳ��ǰ������Դ��");
				for (j = 0; j < N; j++)
				{
					WORK[j] += ALLOCATION[i][j]; // ���������Դ����
					printf("%d ", WORK[j]);
				}
				printf("\n");
				array[cnt++] = i;
				i = 0;
			}
			else { // ����ִ���ж���һ������
				i++;
			}
		}
		printf("\n");
		printf("��ϵͳ����ȫ!!! ������Դ���벻�ɹ�!!!��\n");
		printf("\n");
		return 0;

	}
}

void bank()   //���м��㷨����
{
	int i = 0, j = 0;
	char flag = 'Y';

	while (flag == 'Y' || flag == 'y')
	{
		i = -1;
		while (i < 0 || i >= M)
		{
			printf(" ��������������Դ�Ľ��̺ţ���P0��P%d������������!��:", M - 1);
			printf("P");
			scanf("%d", &i);
			while (i < 0 || i >= M) {
				printf(" ����Ľ��̺Ų����ڣ���������!\n");
				printf(" ��������������Դ�Ľ��̺ţ���P0��P%d������������!��:", M - 1);
				printf("P");
				scanf("%d", &i);
			}
			printf(" ���������P%d�������Դ��:\n", i);
			for (j = 0; j < N; j++)
			{
				printf(" ��Դ%d:", j);
				scanf("%d", &Request[j]);
				if (Request[j] > NEED[i][j]) //���������Դ�����ڽ��̻���Ҫi����Դ����Դ��j
				{
					printf(" ����P%d�������Դ�����ڽ���P%d����Ҫ%d����Դ����Դ��!", i, i, j);
					printf("���벻��������!������ѡ��!\n\n");
					flag = 'N';
					break;
				}
				else
				{
					if (Request[j] > AVAILABLE[j]) //���������Դ�����ڿ�����Դ��
					{
						printf(" ����P%d�������Դ������ϵͳ����%d����Դ����Դ��!", i, j);
						printf("���벻��������!������ѡ��!\n\n");
						flag = 'N';
						break;
					}
				}
			}
			if (flag == 'Y' || flag == 'y')
			{
				changdata(i); //����changdata(i)�������ı���Դ��
				if (chkerr(i)) //��ϵͳ��ȫ
				{

					printf("\n");
					printf(" �Ƿ�������м��㷨��ʾ,��'Y'��'y'������,��'N'��'n'���˳���ʾ: ");
					scanf("%*c%c", &flag);
					rstordata(i); //����rstordata(i)�������ָ���Դ��
					showdata();   //�����Դ�������
				}
				else {      //��ϵͳ����ȫ

					printf("\n");
					printf(" �Ƿ�������м��㷨��ʾ,��'Y'��'y'������,��'N'��'n'���˳���ʾ: ");

					scanf("%*c%c", &flag);
					showdata(); //�����Դ�������
				}
			}
			else      //��flag=N||flag=n
			{
				printf("\n");
				printf(" �Ƿ�������м��㷨��ʾ,��'Y'��'y'������,��'N'��'n'���˳���ʾ: ");

				scanf("%*c%c", &flag);
				showdata();
			}

		}
	}
}

//������
int main()
{
	int i = 0, j = 0, p;
	//��ʼ����Դ����
	inputdata();
	for (j = 0; j < N; j++)
	{
		p = ALL_RESOURCE[j];
		for (i = 0; i < M; i++)
		{
			p = p - ALLOCATION[i][j];//��ȥ�Ѿ���ռ�ݵ���Դ
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
