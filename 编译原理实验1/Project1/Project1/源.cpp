#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <conio.h>
using namespace std;
//词法分析程序
//首先定义类别码
/*
第一类：标识符   letter(letter | digit)*  无穷集
第二类：常数    (digit)+  无穷集
第三类：保留字(32)
auto       break    case     char        const      continue
default    do       double   else        enum       extern
float      for      goto     if          int        long
register   return   short    signed      sizeof     static
struct     switch   typedef  union       unsigned   void
volatile    while

第四类：界符  ‘/*’、‘//’、 () { } [ ] " "  '
第五类：运算符 <、<=、>、>=、=、+、-、*、/、^、

对所有可数符号进行编码：
<$,0>
<auto,1>
...
<while,32>
<+，33>
<-,34>
<*,35>
</,36>
<<,37>
<<=,38>
<>,39>
<>=,40>
<=,41>
<==,42>
<!=,43>
<;,44>
<(,45>
<),46>
<^,47>
<,,48>
<",49>
<',50>
<#,51>
<&,52>
<&&,53>
<|,54>
<||,55>
<%,56>
<~,57>
<<<,58>左移
<>>,59>右移
<[,60>
<],61>
<{,62>
<},63>
<\,64>
<.,65>
<?,66>
<:,67>
<!,68>
"[","]","{","}"
<常数99  ,数值>
<标识符100 ，标识符指针>


*/

//全局变量，保留字表
static char Reserve_Word_list[19][15] = {
	"void","int","float", "double", "if", "else",
	"for","do","while" "break", "case", "char",
	"continue","default", "return", "struct",
	"switch", "typedef","unsigned",
};
//界符运算符表
static char Operation_Word_list[36][10] = {
	"+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
	"!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
	"&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
	"}", "\\", ".", "\?", ":", "!"
};

static  char Flag_table[1000][50] = { "" };//标识符表


/********查找保留字*****************/
int searchReserveWord(char Reserve_Word_list[][15], char s[])
{
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(Reserve_Word_list[i], s) == 0)
		{//若成功查找，则返回类别码
			return i + 1;//返回类别码
		}
	}
	return -1;//否则返回-1，代表查找不成功，即为标识符
}


/*********************判断是否为字母********************/
bool isLetter(char letter)
{
	if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')
	{
		return true;
	}
	else
	{
		return false;
	}
}



/*****************判断是否为数字************************/
bool IsDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}



/********************编译预处理，取出无用的字符和注释**********************/
void pretreatment(char r[], int Code_Pointer)
{
	char tempString[10000];
	int count = 0;
	for (int i = 0; i <= Code_Pointer; i++)
	{
		if (r[i] == '/' && r[i + 1] == '/')
		{//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
			while (r[i] != '\n')
			{
				i++;//向后扫描
			}
		}
		if (r[i] == '/' && r[i + 1] == '*')
		{//若为多行注释“/* 。。。*/”则去除该内容
			i += 2;
			while (r[i] != '*' || r[i + 1] != '/')
			{
				i++;//继续扫描
				if (r[i] == '$')
				{
					printf("注释出错，没有找到 */，程序结束！！！\n");
					exit(0);
				}
			}
			i += 2;//跨过“*/”
		}
		if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
		{//若出现无用字符，则过滤；否则加载
			tempString[count++] = r[i];
		}
	}
	tempString[count] = '\0';
	strcpy(r, tempString);//过滤之后的源程序
}



/****************************扫描分析，状态转换***********************/
void Scanner(int& syn, char Resource_Code[], char token[], int& Code_Pointer)
{//根据DFA的状态转换图设计
	int i, count = 0;//count用来做token[]的指示器，收集有用字符
	char ch;//作为判断使用
	ch = Resource_Code[Code_Pointer];
	while (ch == ' ')
	{//过滤空格
		Code_Pointer++;
		ch = Resource_Code[Code_Pointer];
	}
	for (i = 0; i < 15; i++)
	{//初始化
		token[i] = '\0';
	}
	if (isLetter(Resource_Code[Code_Pointer]))
	{//开头为字母
		token[count++] = Resource_Code[Code_Pointer];//收集
		Code_Pointer++;//下移
		while (isLetter(Resource_Code[Code_Pointer]) || IsDigit(Resource_Code[Code_Pointer]))
		{//后跟字母或数字
			token[count++] = Resource_Code[Code_Pointer];//收集
			Code_Pointer++;//下移
		}//多读了一个字符既是下次将要开始的指针位置
		token[count] = '\0';
		syn = searchReserveWord(Reserve_Word_list, token);//查表找到类别码
		if (syn == -1)
		{//若不是保留字则是标识符
			syn = 100;//标识符类别码
		}
		return;
	}
	else if (IsDigit(Resource_Code[Code_Pointer]))
	{//首字符为数字
		while (IsDigit(Resource_Code[Code_Pointer]))
		{//后跟数字
			token[count++] = Resource_Code[Code_Pointer];//收集
			Code_Pointer++;
		}//多读了一个字符既是下次将要开始的指针位置
		token[count] = '\0';
		syn = 99;//常数类别码
	}
	else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
		|| ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '['
		|| ch == ']' || ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
	{//若为运算符或者界符，查表得到结果
		token[0] = Resource_Code[Code_Pointer];
		token[1] = '\0';//形成单字符串
		for (i = 0; i < 36; i++)
		{//查运算符界符表
			if (strcmp(token, Operation_Word_list[i]) == 0)
			{
				syn = 33 + i;
				break;//查到即推出
			}
		}
		Code_Pointer++;//指针下移，为下一扫描做准备
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '<')
	{//<,<=,<<
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '=')
		{
			syn = 38;
		}
		else if (Resource_Code[Code_Pointer] == '<')
		{//左移
			Code_Pointer--;
			syn = 58;
		}
		else
		{
			Code_Pointer--;
			syn = 37;
		}
		Code_Pointer++;//指针下移
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '>')
	{//>,>=,>>
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '=')
		{
			syn = 40;
		}
		else if (Resource_Code[Code_Pointer] == '>')
		{
			syn = 59;
		}
		else
		{
			Code_Pointer--;
			syn = 39;
		}
		Code_Pointer++;
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '=')
	{//=.==
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '=')
		{
			syn = 42;
		}
		else
		{
			Code_Pointer--;
			syn = 41;
		}
		Code_Pointer++;
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '!')
	{//!,!=
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '=')
		{
			syn = 43;
		}
		else
		{
			syn = 68;
			Code_Pointer--;
		}
		Code_Pointer++;
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '&')
	{//&,&&
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '&')
		{
			syn = 53;
		}
		else
		{
			Code_Pointer--;
			syn = 52;
		}
		Code_Pointer++;
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '|')
	{//|,||
		Code_Pointer++;
		if (Resource_Code[Code_Pointer] == '|')
		{
			syn = 55;
		}
		else
		{
			Code_Pointer--;
			syn = 54;
		}
		Code_Pointer++;
		return;
	}
	else  if (Resource_Code[Code_Pointer] == '$')
	{//结束符
		syn = 0;//类别码为0
	}
	else
	{//不能被以上词法分析识别，则出错。
		printf("错误，不存在 %c \n", ch);
		getchar();
		exit(0);
	}
}


int main()
{
	//打开一个文件，读取其中的源程序
	char Resource_Code[10000];
	char token[15] = { 0 };
	int syn = -1, i;//初始化
	int Code_Pointer = 0;//源程序指针
	FILE* fp, * fp1;
	if ((fp = fopen("read1.txt", "r")) == NULL)
	{//打开源程序
		cout << "文件无法打开！";
		getchar();
		exit(0);
	}
	Resource_Code[Code_Pointer] = fgetc(fp);
	while (Resource_Code[Code_Pointer] != '$')
	{//将源程序读入Resource_Code[]数组
		Code_Pointer++;
		Resource_Code[Code_Pointer] = fgetc(fp);
	}
	Resource_Code[++Code_Pointer] = '\0';
	fclose(fp);

	cout << endl << "源程序为:" << endl;
	cout << Resource_Code << endl;
	//对源程序进行过滤
	pretreatment(Resource_Code, Code_Pointer);
	cout << endl << "过滤之后的程序:" << endl;
	cout << Resource_Code << endl;
	Code_Pointer = 0;//从头开始读

	if ((fp1 = fopen("out.txt", "w+")) == NULL)
	{//打开源程序
		cout << "文件无法打开！";
		exit(0);
	}

	while (syn != 0)
	{
		//启动扫描
		Scanner(syn, Resource_Code, token, Code_Pointer);
		if (syn == 100)
		{//标识符
			for (i = 0; i < 1000; i++)
			{//插入标识符表中
				if (strcmp(Flag_table[i], token) == 0)
				{//已在表中
					break;
				}
				if (strcmp(Flag_table[i], "") == 0)
				{//查找空间
					strcpy(Flag_table[i], token);
					break;
				}
			}
			printf("(标识符  ,%s)\n", token);
			fprintf(fp1, "(标识符   ,%s)\n", token);
		}
		else if (syn >= 1 && syn <= 32)
		{//保留字
			printf("(保留字  ,%s)\n", Reserve_Word_list[syn - 1]);
			fprintf(fp1, "(保留字   ,%s)\n", Reserve_Word_list[syn - 1]);
		}
		else if (syn == 99)
		{//const 常数
			printf("(常数   ,   %s)\n", token);
			fprintf(fp1, "(常数   ,   %s)\n", token);
		}
		else if (syn >= 33 && syn <= 68)
		{
			printf("(界符   ,  %s)\n", Operation_Word_list[syn - 33]);
			fprintf(fp1, "(界符   ,  %s)\n", Operation_Word_list[syn - 33]);
		}
	}
	fclose(fp1);
	getchar();
	return 0;
}
