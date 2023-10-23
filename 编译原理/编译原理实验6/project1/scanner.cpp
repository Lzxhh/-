#include <string>
#include <list>
#include <stack>
#include <iostream>
#include "scanner.h"
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include "scanner.h"
using namespace std;
 
#define LETTER			0	//标识符种类码
#define NUM				1	//常数种类码
#define BEGINKEYWORD	10	//关键字表在总表中的起始位置
#define BEGINOPERATOR	100	//运算符表在总表中的起始位置
#define BEGINBOU		200	//界符表在总表中的起始位置


string token;				//字符数组，存放已读入的字符序列
string instr;				//输入符号串
char character;				//全局变量字符，存放最新读入的字符
map<string, int> id;		//总表
map<string, int>::iterator ite;
map<string, int> Symbol;	//标识符集
map<string, int> Digit;		//常数集
vector<string> keyword;
vector<string> operatorword;
vector<string> boundary;

int index;					//当前输入符号读入字符的位置
typedef struct {			//单词二元组
	string value;			//单词种别码
	int categoryCode;		//单词自身的值
}Binary;

//函数定义
void InitKeyword();		//初始化关键字表
void InitOperator();	//初始化运算符表
void InitBoundary();	//初始化界符表
void InitAll();			//初始化汇总
bool IsLetter();		//判断是否为单词
bool IsNum();			//判断是否为数字
void retract();			//回退
char GetChar();			//读取一个字符
void removeBlank();		//去掉空格
void concat();			//连接字符串
int matchKeyword();		//关键字表匹配
string matchSymbol();	//标识符表匹配
string matchNum();		//常数表匹配
Binary scanner();		//扫描器
void showtable();		//表展示


void InitKeyword() {
	keyword.push_back("if");		//10
	keyword.push_back("else");		//11
	keyword.push_back("while");		//12
	keyword.push_back("signed");	//13
	keyword.push_back("throw");		//14
	keyword.push_back("union");		//15
	keyword.push_back("this");		//16
	keyword.push_back("int");		//17
	keyword.push_back("char");		//18
	keyword.push_back("double");	//19
	keyword.push_back("unsigned");	//20
	keyword.push_back("const");		//21
	keyword.push_back("goto");		//22
	keyword.push_back("virtual");	//23
	keyword.push_back("for");		//24
	keyword.push_back("float");		//25
	keyword.push_back("break");		//26
	keyword.push_back("auto");		//27
	keyword.push_back("class");		//28
	keyword.push_back("operator");	//29
	keyword.push_back("case");		//30
	keyword.push_back("do");		//31
	keyword.push_back("long");		//32
	keyword.push_back("typedef");	//33
	keyword.push_back("static");	//34
	keyword.push_back("friend");	//35
	keyword.push_back("tmplate");	//36
	keyword.push_back("default");	//37
	keyword.push_back("new");		//38
	keyword.push_back("void");		//39
	keyword.push_back("register");	//40
	keyword.push_back("extern");	//41
	keyword.push_back("return");	//42
	keyword.push_back("enum");		//43
	keyword.push_back("inline");	//44
	keyword.push_back("try");		//45
	keyword.push_back("short");		//46
	keyword.push_back("continue");	//47
	keyword.push_back("sizeof");	//48
	keyword.push_back("switch");	//49
	keyword.push_back("private");	//50
	keyword.push_back("protected");	//51
	keyword.push_back("asm");		//52
	keyword.push_back("while");		//53
	keyword.push_back("catch");		//54
	keyword.push_back("delete");	//55
	keyword.push_back("public");	//56
	keyword.push_back("volatile");	//57
	keyword.push_back("struct");	//58
}

void InitOperator() {
	operatorword.push_back("+");	//100
	operatorword.push_back("+=");	//101
	operatorword.push_back("++");	//102

	operatorword.push_back("-");	//103
	operatorword.push_back("--");	//104
	operatorword.push_back("->");
	operatorword.push_back("-=");

	operatorword.push_back("*");
	operatorword.push_back("*=");
	operatorword.push_back("*/");


	operatorword.push_back("/");
	operatorword.push_back("/=");
	operatorword.push_back("//");
	operatorword.push_back("/*");


	operatorword.push_back("%");
	operatorword.push_back("%=");

	operatorword.push_back("=");
	operatorword.push_back("==");

	operatorword.push_back(">");
	operatorword.push_back(">=");
	operatorword.push_back(">>");

	operatorword.push_back("<");
	operatorword.push_back("<<");
	operatorword.push_back("<=");
	operatorword.push_back("!");
	operatorword.push_back("!=");
	operatorword.push_back("&&");
	operatorword.push_back("||");
	operatorword.push_back("&");
	operatorword.push_back("|");



}

void InitBoundary() {
	boundary.push_back("(");		//200
	boundary.push_back(")");		//201
	boundary.push_back("{");		//202
	boundary.push_back("}");		//203
	boundary.push_back("#");		//204
	boundary.push_back(",");		//205
	boundary.push_back(";");		//206
}

void InitAll() {
	InitKeyword();
	InitOperator();
	InitBoundary();
	id["letter"] = 0;
	id["num"] = 1;
	for (int j = 0; j < keyword.size(); j++) {
		id[keyword[j]] = j + BEGINKEYWORD;
	}
	for (int j = 0; j < operatorword.size(); j++) {
		id[operatorword[j]] = j + BEGINOPERATOR;
	}
	for (int j = 0; j < boundary.size(); j++) {
		id[boundary[j]] = j + BEGINBOU;
	}
	index = 0;
	character = ' ';
	token = "";

}

bool IsLetter() {
	if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
		return true;
	return false;
}

bool IsNum() {
	if (character >= '0' && character <= '9')
		return true;
	return false;
}

void retract() {//回退字符的函数
	character = ' ';
	index--;
}

char GetChar() {
	character = instr[index++];
	return character;
}

void removeBlank() {
	while (character == ' ' || character == '\t') {
		character = instr[index++];
	}
}

void concat() {
	token = token + character;
}

int matchKeyword() {
	for (int i = 0; i < keyword.size(); i++) {
		if (keyword[i] == token)
			return (i + BEGINKEYWORD);
	}
	return LETTER;
}

string matchSymbol() {
	ite = Symbol.find(token);
	if (ite != Symbol.end()) {
		return ite->first;
	}
	else {
		Symbol[token] = Symbol.size();
		return token;
	}
}

string matchNum() {
	ite = Digit.find(token);
	if (ite != Digit.end()) {
		return ite->first;
	}
	else {
		Digit[token] = Digit.size();
		return token;
	}
}

Binary scanner() {
	token = "";
	GetChar();     // 从输入缓冲区读取一个字符到ch中
	removeBlank(); // 去掉空格
	Binary binary;
	binary.categoryCode = 0;
	binary.value = "";
	if (IsLetter()) {
		while (IsLetter() || IsNum()) {
			concat();
			GetChar();
		}
		retract();		//回退
		if (matchKeyword() == LETTER) {
			binary.categoryCode = LETTER;
			binary.value = matchSymbol();
		}
		else {
			binary.categoryCode = id[token];
			binary.value = token;
		}
		return binary;

	}
	else if (IsNum()) {
		while (IsNum()) {
			concat();
			GetChar();
		}
		retract();
		binary.categoryCode = NUM;
		binary.value = matchNum();
		return binary;
	}
	else {
		switch (character)
		{
		case '+':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["+="];
				binary.value = "+=";
				return binary;
			}
			else if (character == '+') {
				binary.categoryCode = id["++"];
				binary.value = "++";
				return binary;
			}
			retract();
			binary.categoryCode = id["+"];
			binary.value = "+";
			return binary;
			break;
		}

		case '-':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["-="];
				binary.value = "-=";
				return binary;
			}
			else if (character == '-') {
				binary.categoryCode = id["-="];
				binary.value = "--";
				return binary;
			}
			else if (character == '>') {
				binary.categoryCode = id["->"];
				binary.value = "->";
				return binary;
			}
			retract();
			binary.categoryCode = id["-"];
			binary.value = "-";
			return binary;
			break;
		}

		case '*':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["*="];
				binary.value = "*=";
				return binary;
			}
			else if (character == '/') {
				binary.categoryCode = id["/*"];
				binary.value = "/*";
				return binary;
			}
			retract();
			binary.categoryCode = id["*"];
			binary.value = "*";
			return binary;
			break;
		}


		case '/':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["/="];
				binary.value = "/=";
				return binary;
			}
			else if (character == '/') {
				while (GetChar()) {
					if (character == '\n') {
						retract();
						break;
					}
				}
				binary.categoryCode = id["//"];
				binary.value = "//";
				return binary;
			}
			else if (character == '*') {
				while (GetChar()) {
					if (character == '*') {
						GetChar();
						if (character == '/') {
							retract();
							retract();
							break;
						}
					}
				}
				binary.categoryCode = id["/*"];
				binary.value = "/*";
				return binary;
			}
			retract();
			binary.categoryCode = id["/"];
			binary.value = "/";
			return binary;
			break;

		}

		case '%':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["%="];
				binary.value = "%=";
				return binary;
			}
			retract();
			binary.categoryCode = id["%"];
			binary.value = "%";
			return binary;
			break;
		}

		case '=':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["=="];
				binary.value = "==";
				return binary;
			}
			retract();
			binary.categoryCode = id["="];
			binary.value = "=";
			return binary;
			break;
		}


		case '>':
		{
			if (character == '=' || character == '>') {
				string tmp;
				tmp.push_back('>');
				tmp.push_back(character);
				binary.categoryCode = id[tmp];
				binary.value = tmp;
				return binary;
			}
			retract();
			binary.categoryCode = id[">"];
			binary.value = ">";
			return binary;
			break;
		}

		case '<':
		{
			GetChar();
			if (character == '=' || character == '<') {
				string tmp;
				tmp.push_back('<');
				tmp.push_back(character);
				binary.categoryCode = id[tmp];
				binary.value = tmp;
				return binary;
			}
			retract();
			binary.categoryCode = id["<"];
			binary.value = "<";
			return binary;
			break;
		}

		case '!':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["!="];
				binary.value = "!=";
				return binary;
			}
			retract();
			binary.categoryCode = id["!"];
			binary.value = "!";
			return binary;
			break;
		}

		case '&':
		{
			GetChar();
			if (character == '=') {
				binary.categoryCode = id["&&"];
				binary.value = "&&";
				return binary;
			}
			retract();
			binary.categoryCode = id["&"];
			binary.value = "&&";
			return binary;
			break;
		}

		case '|':
		{
			GetChar();
			if (character == '||') {
				binary.categoryCode = id["||"];
				binary.value = "||";
				return binary;
			}
			retract();
			binary.categoryCode = id["|"];
			binary.value = "|";
			return binary;
			break;
		}


		case '(':
		case ')':
		case '{':
		case '}':
		case '#':
		case ',':
		case ';':
		{
			string tmp;
			tmp.push_back(character);
			binary.categoryCode = id[tmp];
			binary.value = tmp;
			return binary;
			break;
		}

		default:
			binary.categoryCode = -1;
			binary.value = "ERROR";
			return binary;
			break;
		}
	}

}

void showtable() {
	cout << "**********" << "关键字" << "**********" << endl;
	cout << "关键字符\t种类码" << endl;
	for (int i = 0; i < keyword.size(); i++) {
		if (keyword[i].size() >= 8) {
			cout << keyword[i] << "\t" << i + BEGINKEYWORD << endl;
		}
		else {
			cout << keyword[i] << "\t\t" << i + BEGINKEYWORD << endl;
		}
	}
	cout << "**********" << "界符" << "**********" << endl;
	cout << "界符\t\t种类码" << endl;
	for (int i = 0; i < boundary.size(); i++) {
		cout << boundary[i] << "\t\t" << i + BEGINBOU << endl;
	}
	cout << "**********" << "运算符" << "**********" << endl;
	cout << "运算符\t\t种类码" << endl;
	for (int i = 0; i < operatorword.size(); i++) {
		cout << operatorword[i] << "\t\t" << i + BEGINOPERATOR << endl;
	}
	cout << "**********" << "标识符" << "**********" << endl;
	cout << "标识符\t\t表中位置" << endl;
	for (ite = Symbol.begin(); ite != Symbol.end(); ite++) {
		if (ite->first.size() >= 8) {
			cout << ite->first << "\t" << ite->second << endl;
		}
		else {
			cout << ite->first << "\t\t" << ite->second << endl;
		}
	}
	cout << "**********" << "常数表" << "**********" << endl;
	cout << "常数\t\t表中位置" << endl;
	for (ite = Digit.begin(); ite != Digit.end(); ite++) {
		cout << ite->first << "\t\t" << ite->second << endl;
	}
}

int main1() {
	InitAll();
	cout << "请输入待分析的源程序代码：输入$代表结束输入\n" << endl;
	string in;

	while (cin >> in && in != "$") {
		int add = in.find("//");
		int add2 = in.find("/*");
		if (add == in.npos && add2 == in.npos) {
			instr = instr + " " + in;
		}
		else if (add != in.npos && add2 == in.npos) {
			in = in.substr(0, add);
			instr = instr + " " + in;
		}
		else if (add == in.npos && add2 != in.npos) {
			while (1) {
				if (in.find("*/") != in.npos)
					break;
				cin >> in;
			};

		}
		if (cin.get() == '\n') {
			instr += '\n';
		}
	}
	cout << "*******************预处理********************" << endl;
	cout << instr;
	Binary binary;
	binary.categoryCode = -1;
	binary.value = "";
	ofstream out("lexout.txt");
	//for (map<string,int>::iterator i = id.begin(); i !=id.end(); i++) {
	//	cout << i->first << i->second << endl;
	//}
	cout << "\n*********************识别结果**************************" << endl;
	while (index < instr.size()) {
		binary = scanner();
		if (binary.categoryCode != -1) {
			cout << "(" << binary.categoryCode << "," << binary.value << ")" << endl;
			out << "(" << binary.categoryCode << "," << binary.value << ")" << endl;
		}
	}

	showtable();
	return 0;
}
