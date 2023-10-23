#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<conio.h>
using namespace std;
#define MAX 100
char token[MAX];
char token1[MAX];		//用于检测文件输入是否匹配
char token2[MAX];
char current;
int i = 0;

void S();
void E();
void E1();
void T();
void A();
void F();//函数推导过程
void T1();
void M();
void V();
void error(char *c);    //提出语法错误
void error1();  //提出非法输入错误

void init()		//初始化
{
	current = token[i];
}

void judge(char ch, int i)		//判断文件读取内容是否合法，包括检查非法字符和不匹配现象
{
	switch (ch)
	{
	case '1':
		if (token[i] != '=')
			error1();
		break;
	case '2':
		if (token[i] != '(')
			error1();
		break;
	case '3':
		if (token[i] != ')')
			error1();
		break;
	case '4':
		if (token[i] != '+')
			error1();
		break;
	case '5':
		if (token[i] != '-')
			error1();
		break;
	case '6':
		if (token[i] != '*')
			error1();
		break;
	case '7':
		if (token[i] != '/')
			error1();
		break;
	case '8':
		if (token[i] != 'i')
			error1();
		break;
	case '9':
		if (token[i] != '#')
			error1();
		break;
	default:
		error1();
		break;
	}
}

void error(char *c)
{
	printf("%s is wrong\n", c);
	cout << "the input is not defined" << endl;
	exit(0);
}

void error1()
{
	cout << "illegal input！" << endl;
	cout << "the input is not defined" << endl;
	exit(0);
}

void advance()		//读取下一个字符
{
	if (i == MAX)
		exit(0);
	current = token[++i];
}

void S()
{
	if (current == 'i')
	{
		V();
		if (current == '=')
		{
			advance();
			E();
		}
	}
	else
		error(const_cast<char*>("S"));
}

void E()
{
	if (current == 'i' || current == '(')
	{
		T();
		E1();
	}
	else
		error(const_cast<char*>("E"));
}

void E1()
{
	if (current == '+' || current == '-')
	{
		A();
		T();
		E1();
	}
	else
		if (!(current == ')' || current == '#'))
			error(const_cast<char*>("E1"));
}

void T()
{
	if (current == 'i' || current == '(')
	{
		F();
		T1();
	}
	else
		error(const_cast<char*>("T"));
}

void T1()
{
	if (current == '*' || current == '/')
	{
		M();
		F();
		T1();
	}
	else
		if (!(current == ')' || current == '#' || current == '+' || current == '-'))
			error(const_cast<char*>("T1"));
}

void M()
{
	if (current == '*')
		advance();
	else
		if (current == '/')
			advance();
		else
			error(const_cast<char*>("M"));
}

void A()
{
	if (current == '+')
		advance();
	else
		if (current == '-')
			advance();
		else
			error(const_cast<char*>("A"));
}

void F()
{
	if (current == '(')
	{
		advance();
		E();
		if (current == ')')
			advance();
	}
	else
		if (current == 'i')
			advance();
		else
			error(const_cast<char*>("F"));
}

void V()
{
	if (current == 'i')
		advance();
	else
		error(const_cast<char*>("V"));
}

void readfile()
{
	int j = 1;
	int i = 0;
	FILE *fp;
	//i=i+i*i
	if ((fp = fopen("input2.txt", "r")) == NULL)
	{
		cout << "can not open the file！" << endl;
		exit(0);
	}
	/*i=i+i/
	if ((fp = fopen("input2.txt", "r")) == NULL)
	{
		cout << "文件打开失败！" << endl;
		exit(0);
	}*/
	token1[0] = fgetc(fp);
	while (!feof(fp))
	{
		token1[j++] = fgetc(fp);
		if ((j % 5) == 0)
		{
			token2[i] = token1[j - 4];
			token[i] = token1[j - 2];
			judge(token2[i], i);
			i++;
		}
	}
	if ((j - 1) % 5 != 0) {
		error1();
	}
	cout << "the input is ：" << token << endl;
}

int main()
{
	readfile();
	init();
	S();
	if (current == '#')
		cout << "correct input!" << endl;
	return 1;
}
