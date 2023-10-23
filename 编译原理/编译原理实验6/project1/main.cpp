
/*
�������������ֵ��� SLR(1)�ķ��﷨�Ƶ������м������Ԫʽ�Ĺ��̡�
G[A]:
A��V=E
E��E+T�OE-T�OT
T��T*F�OT/F�OF
F��(E)�Oi
V��i
[���˵��] �ս���� i Ϊ�û�����ļ򵥱����� ����ʶ���Ķ��塣
[���Ҫ��]
��1�������ķ��� SLR(1)����������﷨�Ƶ�������̣�����ÿһ����ʽ��Ӧ�����嶯����
��2������м������Ԫʽ�Ľṹ��
��3�����봮Ӧ�Ǵʷ������������Ԫʽ���У���ĳ��ֵ��䡰ר�� 1������������
���Ϊ��ֵ������Ԫʽ�����м��ļ���
��4��������������������������걸��������������ִ�н����Ԫʽ���С�
*/

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
using namespace std;


typedef struct sentence
{
	int finish = 0;  //�Ƿ����
	int tag = 3;  //��ǰԲ���λ��
	string str;
}sentence;


typedef struct
{
	int I;  //��ǰ״̬
	char Vt;  //�ս����
	char tag;  //r||s
	int action;  //����
}act;


typedef struct
{
	int I;  //��ǰ״̬
	char Vn;  //���ս��
	int next_I;  //ת��״̬
}go;


typedef struct
{
	int number;  //���
	list<sentence> l;
}I;  //״̬


typedef struct
{
	char op;
	char arg1;
	char arg2;
	char result;
}siyuanshi;


typedef struct
{
	char name;
	string value;  //place
}var;  //����


list<I> DFA;  //״̬��
list<char>* First;  //FIRST��
list<char>* Follow;  //FOLLOW��
list<act> ACTION;  //ACTION[i][j]
list<go> GOTO;  //GO[i][j]
list<char> Vt;  //�ս���ż�
list<char> Vn;  //���ս���ż�


char input[100];// = { 'i','=','a','*','(','b','+','c',')','/','d','#' };

void justify(int ma, int i) {
	switch (ma) {
	case 200:
		input[i] = '(';
		break;
	case 201:
		input[i] = ')';
		break;
	case 100:
		input[i] = '+';
		break;
	case 103:
		input[i] = '-';
		break;
	case 107:
		input[i] = '*';
		break;
	case 110:
		input[i] = '/';
		break;
	case 0:
		input[i] = 'i';
		break;
	case 204:
		input[i] = '#';
		break;
	case 116:
		input[i] = '=';
		break;
	default:
		break;
	}
}


void readfile()
{
	main1();
	cout << "����ʵ��һ�����" << endl;
	ifstream infile("lexout.txt", ios::in);


	int num = 0, k = 0;
	string str;
	string tmps = "";
	while (getline(infile, str)) {
		while (str[k] != ',') {
			tmps += str[k];
			k++;
		}
		k = 1;
		int m = atoi(tmps.c_str());
		cout << m << endl;
		justify(m, num++);
		tmps.clear();
	}
	input[num] = '#';
	cout << endl << "���봮Ϊ��" << input << endl << endl;
}


int S[100];  //����ջ
int S_top = 0;  //����ջջ��
char T[100];  //����ջ ջ�������ջ����ͬ
string value[100];  //�洢���ŵ�����
list<siyuanshi> L_sys;  //��Ԫʽ
list<int> sys;  //������Ԫʽ��״̬��
list<var> V;  //������


int result_count = 0;
string G[11] = { "S->A", "A->V=E","E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->i","V->i" };


act getACTION(int i, char b)
{
	char c = b;
	if (b >= 'a' && b <= 'z')
		b = 'i';
	list<act>::iterator it;
	for (it = ACTION.begin(); it != ACTION.end(); it++)
	{
		if (it->I == i && it->Vt == b)
		{
			return *it;
		}
	}
	return { 0,0,0,0 };  //ERROR
}


int getGOTO(int i, char b)
{
	list<go>::iterator it;
	for (it = GOTO.begin(); it != GOTO.end(); it++)
	{
		if (it->I == i && it->Vn == b)
			return  it->next_I;
	}
	return -1;  //ERROR
}


char newTemp()
{
	char c = 'A';
	result_count++;
	return c + result_count - 1;
}


void SLR()
{
	int V_num = 0;  //���ű�
	int index = 0;
	S[0] = 0;  //init
	cout << "����ջ��(| |�е�Ϊ���ս���ŵ�����)" << endl;
	while (getACTION(S[S_top], input[index]).tag != 'r' || getACTION(S[S_top], input[index]).action != 0)  //acc
	{
		/*	if (input[index] >= 'a'&&input[index] <= 'z')
			{
				string str = "i";
				str += to_string(V_num);
				V_num++;
				V.push_back({ str,input[index] });
			}
			for (list<var>::iterator v = V.begin(); v != V.end(); v++)
			{
				cout << v->name << " " << v->value << endl;
			}
		*/
		if (getACTION(S[S_top], input[index]).tag == 0)  //ERROR
		{
			cout << "error!" << endl;  //������
			return;
		}
		else if (getACTION(S[S_top], input[index]).tag == 's')
		{   //		push
			S[S_top + 1] = getACTION(S[S_top], input[index]).action;  //push����
			T[S_top + 1] = input[index];
			value[S_top + 1] = input[index];
			S_top++;
			index++;  //advance();��ȡ��һ������
		}
		else if (getACTION(S[S_top], input[index]).tag == 'r')
		{
			char arg1, arg2, result;  //��Ԫʽ
			int i = getACTION(S[S_top], input[index]).action;  //Ҫ��Լ�Ĳ���ʽ
			string str = "";
			for (int j = 0; j < G[i].length() - 3; j++)
			{
				//		pop����
				if (value[S_top] != "(" && value[S_top] != ")")
				{
					string ss = "";
					ss += value[S_top];
					ss += str;
					str = string(ss);
				}


				if (j == 0)
					if (value[S_top].length() == 1)  //ֻ�ǵ�������
						arg1 = value[S_top][0];
					else  //������ҷ���
					{
						for (list<var>::iterator v = V.begin(); v != V.end(); v++)
						{
							if (v->value == value[S_top])
							{
								arg1 = v->name;
								break;
							}
						}
					}
				if (j == 2)
					if (value[S_top].length() == 1)
						arg2 = value[S_top][0];
					else
						for (list<var>::iterator v = V.begin(); v != V.end(); v++)
						{
							if (v->value == value[S_top])
							{
								arg2 = v->name;
								break;
							}
						}
				S[S_top] = -1;
				T[S_top] = 0;
				value[S_top] = "";
				S_top--;
			}
			for (list<int>::iterator ii = sys.begin(); ii != sys.end(); ii++)  //������Ԫʽ
				if (i == *ii)
				{
					result = newTemp();
					V.push_back({ result,str });
					L_sys.push_back({ G[i][4],arg1,arg2,result });
					break;
				}
			//push
			S[S_top + 1] = getGOTO(S[S_top], G[i][0]);
			T[S_top + 1] = G[i][0];
			value[S_top + 1] = str;  //���Դ���
			S_top++;
		}
		for (int tt = 0; tt <= 15; tt++)
		{
			if (tt <= S_top)
				if (S[tt] >= 10)
					cout << S[tt] << " ";
				else
					cout << S[tt] << "  ";
			else
				cout << "   ";
		}


		for (int tt = 0; tt <= S_top; tt++)
		{
			if (tt <= S_top)
			{
				cout << T[tt] << " ";
				if (isupper(T[tt]))
					cout << "|" << value[tt] << "| ";
			}
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl << "ƥ��ɹ���" << endl;
	cout << endl << "��Ԫʽ��" << endl;
	for (list<siyuanshi>::iterator s = L_sys.begin(); s != L_sys.end(); s++)
	{
		cout << '(' << s->op << "," << s->arg1 << "," << s->arg2 << ',' << s->result << ')' << endl;
	}
}


void print(sentence s)
{
	int r;
	cout << "  ";
	for (r = 0; r < s.str.length(); r++)
		if (r == s.tag)
			cout << "��" << s.str[r];
		else
			cout << s.str[r];
	if (r == s.tag)
		cout << "��";
	cout << endl;
}




void find(char ch, int tag_Vn)
{
	for (int qq = 0; qq < 11; qq++)
	{
		if (ch == G[qq][0])
		{
			if (G[qq][3] >= 'A' && G[qq][3] <= 'Z')
				if (G[qq][3] == ch)  //������ѭ���ظ�����
					;
				else
					find(G[qq][3], tag_Vn);
			else
			{
				int tag = 0;
				for (list<char>::iterator i = First[tag_Vn].begin(); i != First[tag_Vn].end(); i++)
				{
					if (*i == G[qq][3])
					{
						tag = 1;
						break;
					}
				}
				if (tag == 0)
					First[tag_Vn].push_back(G[qq][3]);
			}
		}
	}
}


int findVn(char c)
{
	int tag = -1;
	for (list<char>::iterator i = Vn.begin(); i != Vn.end(); i++)
	{
		tag++;
		if (c == *i)
			return tag;
	}
	return -1;
}




void scan()  //������ż�
{
	for (int i = 0; i < 11; i++)//�ķ�����
	{
		for (int j = 0; j < G[i].length(); j++)
		{
			if (j == 1 || j == 2)
				continue;
			if (isupper(G[i][j]))
			{
				if (find(Vn.begin(), Vn.end(), G[i][j]) == Vn.end())
					Vn.push_back(G[i][j]);
			}
			else
			{
				if (find(Vt.begin(), Vt.end(), G[i][j]) == Vt.end())
					Vt.push_back(G[i][j]);
			}
		}
	}
	Vt.push_back('#');




	First = new list<char>[Vn.size()];
	int tag_Vn = -1;
	for (list<char>::iterator ch = Vn.begin(); ch != Vn.end(); ch++)
	{
		tag_Vn++;
		for (int qq = 0; qq < 11; qq++)
		{
			if (*ch == G[qq][0])
			{
				if (G[qq][3] >= 'A' && G[qq][3] <= 'Z')
				{
					find(G[qq][3], tag_Vn);
				}
				else
				{
					First[tag_Vn].push_back(G[qq][3]);
				}
			}
		}
	}




	Follow = new list<char>[Vn.size()];
	Follow[0].push_back('#');


	int follow_sum = 0;  //follow�������ܺ�
	int previous_sum = -1;  //֮ǰ���ܺ�
	while (follow_sum != previous_sum)
	{
		for (int i = 0; i < 11; i++)
		{
			for (int j = 3; j < G[i].length() - 1; j++)
			{
				if (isupper(G[i][j]))
					if (!isupper(G[i][j + 1]))  //���ս����
					{
						int tag = 0;
						for (list<char>::iterator ii = Follow[findVn(G[i][j])].begin(); ii != Follow[findVn(G[i][j])].end(); ii++)
						{
							if (*ii == G[i][j + 1])
							{
								tag = 1;
								break;
							}
						}
						if (tag == 0)
							Follow[findVn(G[i][j])].push_back(G[i][j + 1]);
					}
					else  //�Ƿ��ս����
					{
						for (list<char>::iterator ii = Follow[findVn(G[i][j + 1])].begin(); ii != Follow[findVn(G[i][j + 1])].end(); ii++)
						{
							if (find(Follow[findVn(G[i][j + 1])].begin(), Follow[findVn(G[i][j + 1])].end(), *ii) == Follow[findVn(G[i][j + 1])].end())  //���ظ�
								Follow[findVn(G[i][j])].push_back(*ii);
						}
					}
			}
			if (isupper(G[i][G[i].length() - 1]))
				for (list<char>::iterator ii = Follow[findVn(G[i][0])].begin(); ii != Follow[findVn(G[i][0])].end(); ii++)
				{
					if (find(Follow[findVn(G[i][G[i].length() - 1])].begin(), Follow[findVn(G[i][G[i].length() - 1])].end(), *ii) == Follow[findVn(G[i][G[i].length() - 1])].end())  //���ظ�
						Follow[findVn(G[i][G[i].length() - 1])].push_back(*ii);
				}
		}
		previous_sum = follow_sum;
		follow_sum = 0;
		for (int ii = 0; ii < Vn.size(); ii++)
		{
			follow_sum += Follow[ii].size();
		}
	}
	list<char>::iterator iter;
	cout << "�ս���ż���";
	for (iter = Vt.begin(); iter != Vt.end(); iter++)
	{
		cout << *iter;
	}
	cout << endl;
	cout << "���ս���ż���";
	for (iter = Vn.begin(); iter != Vn.end(); iter++)
	{
		cout << *iter;
	}
	cout << endl;
	int pp;
	cout << "FIRST" << endl;
	for (pp = 0, iter = Vn.begin(); pp < Vn.size(); pp++, iter++)
	{
		cout << *iter << "     ";
		for (list<char>::iterator ch = First[pp].begin(); ch != First[pp].end(); ch++)
		{
			cout << *ch << " ";
		}
		cout << endl;
	}
	cout << "FOLLOW" << endl;
	for (pp = 0, iter = Vn.begin(); pp < Vn.size(); pp++, iter++)
	{
		cout << *iter << "     ";
		for (list<char>::iterator ch = Follow[pp].begin(); ch != Follow[pp].end(); ch++)
		{
			cout << *ch << " ";
		}
		cout << endl;
	}


}




I closure(I& dfa)
{
	list<sentence>::iterator iter;
	for (iter = dfa.l.begin(); iter != dfa.l.end(); iter++)
	{
		if ((iter->tag <= iter->str.length()))
			if ((iter->str[iter->tag] >= 'A' && iter->str[iter->tag] <= 'Z'))
			{
				for (int tt = 0; tt < 11; tt++)  //���Ҷ�Ӧ�Ĳ���ʽ
				{
					if (iter->str[iter->tag] == G[tt][0])
					{
						int q = 0;
						list<sentence>::iterator it;
						sentence x;
						x.str = G[tt];
						for (it = dfa.l.begin(); it != dfa.l.end(); it++)
						{
							if ((it->str == x.str) && (it->tag == x.tag))
							{
								q = 1;
								break;
							}
						}
						if (q == 0)
						{
							dfa.l.push_back(x);
						}
					}
				}
			}
	}
	return dfa;
}
int compare(I a, I b) //�Ƿ����
{

	if (a.l.size() != b.l.size())
		return 1;
	else
	{
		for (list<sentence>::iterator iii = a.l.begin(), jjj = b.l.begin(); iii != a.l.end() || jjj != b.l.end(); iii++, jjj++)
			if (iii->str != jjj->str || iii->tag != jjj->tag)  //�ж��Ƿ����
			{
				return 1;
			}
	}
	return 0;
}


void table()
{
	int num = 0;
	sentence first;  //��ʼ����
	first.str = G[0];

	I L;
	L.number = 0;
	L.l.push_back(first);
	DFA.push_back(L);


	list<I>::iterator dfa;
	for (dfa = DFA.begin(); dfa != DFA.end(); dfa++)
	{
		list<sentence>::iterator iter;
		closure(*dfa);
		for (iter = dfa->l.begin(); iter != dfa->l.end(); iter++)  //��״̬�е�ÿһ����ʽ�Ҳ����д���
			if (iter->finish == 0)
			{
				I C;  //��״̬
				C.number = DFA.size();
				if (iter->tag < iter->str.length())  //��δ���
				{
					sentence s;
					s.str = iter->str;
					if (iter->str[iter->tag] >= 'A' && iter->str[iter->tag] <= 'Z')
					{
						GOTO.push_back({ dfa->number,iter->str[iter->tag],C.number });
					}
					else
					{
						ACTION.push_back({ dfa->number,iter->str[iter->tag],'s',C.number });
					}
					s.tag = iter->tag + 1;
					iter->finish = 1;
					C.l.push_back(s);
					list<sentence>::iterator i;
					for (i = iter, i++; i != dfa->l.end(); i++)
						if (i->str[i->tag] == iter->str[iter->tag])
						{
							s.str = i->str;
							s.tag = i->tag + 1;
							i->finish = 1;
							C.l.push_back(s);
						}


					int judge = 0, count = 0;
					for (list<I>::iterator ii = DFA.begin(); ii != DFA.end(); ii++, count++)  //�ж��Ƿ����ظ�
					{
						judge = compare(*ii, closure(C));  //�޸�C
						if (judge == 0)
							break;
					}
					if (judge == 0)
					{
						if (iter->str[iter->tag] >= 'A' && iter->str[iter->tag] <= 'Z')
						{
							GOTO.pop_back();
							GOTO.push_back({ dfa->number,iter->str[iter->tag], count });
						}
						else
						{
							ACTION.pop_back();
							ACTION.push_back({ dfa->number,iter->str[iter->tag],'s',count });
						}
					}
					else
						DFA.push_back(C);
				}
				else  //�Ѿ����
				{
					int cc = 0, tt;
					for (tt = 0; tt < 11; tt++)
					{
						if (iter->str == G[tt])
							break;
					}
					for (list<char>::iterator c = Vn.begin(); c != Vn.end(); c++, cc++)
						if (*c == iter->str[0])
							break;
					for (list<char>::iterator c = Follow[cc].begin(); c != Follow[cc].end(); c++)
					{
						ACTION.push_back({ dfa->number,*c,'r',tt });  //��Ӧ�ĵ�j������ʽ
						for (int ss = 3; ss < G[tt].length(); ss++)  //������Ԫʽ��Ӧ�Ĳ���ʽ
							if (G[tt][ss] == '+' || G[tt][ss] == '-' || G[tt][ss] == '*' || G[tt][ss] == '/')
							{
								sys.push_back(tt);
							}
					}
				}
			}
	}


	num = 0;
	for (dfa = DFA.begin(); dfa != DFA.end(); dfa++)
	{
		cout << "״̬" << num++ << "��" << endl;
		for (list<sentence>::iterator iii = dfa->l.begin(); iii != dfa->l.end(); iii++)
		{
			print(*iii);
		}
	}
	for (list<go>::iterator g = GOTO.begin(); g != GOTO.end(); g++)
	{
		cout << "GOTO(" << g->I << ", " << g->Vn << ") = " << g->next_I << endl;
	}
	for (list<act>::iterator a = ACTION.begin(); a != ACTION.end(); a++)
	{
		cout << "ACTION(" << a->I << ", " << a->Vt << ") = " << a->tag << a->action << endl;
	}
	cout << endl;
}




int main()
{
	readfile();
	scan();
	table();
	SLR();
	return 0;
}
