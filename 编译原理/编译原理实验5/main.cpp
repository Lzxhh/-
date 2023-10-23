#define  _CRT_SECURE_NO_WARNINGS  1
#include <string>
#include <list>
#include <stack>
#include <iostream>
#include<algorithm>
#include<fstream>
#include <bits/stdc++.h>
#include <unordered_set>

using namespace std;

typedef struct s
{
    int finish=0;  //是否结束
    int tag=3;  //当前圆点的位置
    string str;
}sentence;

typedef struct
{
    int I;  //当前状态
    char Vt;  //终结符号
    char tag;  //r||s
    int action;  //动作
}act;

typedef struct
{
    int I;  //当前状态
    char Vn;  //非终结符
    int next_I;  //转移状态
}go;

typedef struct
{
    int number;  //编号
    list<sentence> l;
}I;  //状态

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
}var;  //变量


list<I> DFA;  //状态集
list<char>* First;  //FIRST集
list<char>* Follow;  //FOLLOW集
list<act> ACTION;  //ACTION[i][j]
list<go> GOTO;  //GO[i][j]
list<char> Vt;  //终结符号集
list<char> Vn;  //非终结符号集


char input[100];//读取词法分析的输入
bool hasSameLetters(string str);

int readfile()
{
    cout << "-------------------------------------------------------" << endl;
    cout << "读取文件可知输入语句：" << endl;
    FILE* fp;
    char buf[1000];
    if ((fp = fopen("wrong2.txt", "r")) != NULL) {
        while (fgets(buf, 1000, fp) != NULL)
        {
            int len = strlen(buf);
            printf("%s \n", buf);
            int flag = 0;

            for (int i = 0; i < len; i++) {
                if (buf[i] == '(' && flag == 0) {
                    flag = 1;//识别符号
                    for (int j = i + 1; j < len; j++) {
                        if (buf[j] == ',') {
                            input[strlen(input)] = buf[j + 1];
                            i = j + 1;
                            break;
                        }
                    }
                }
                else if (flag == 1 && buf[i] == ')') {
                    flag = 0;
                }
            }
        }
    }
    input[strlen(input)] = '#';
    fclose(fp);
    cout << endl;
    cout << endl << "输入串为：" << input << endl << endl;
    int checkstr =hasSameLetters(input);
    if (checkstr) {
       // cout << "Equal sides contain same letters" << endl;
        return 3;
    } else {
        //cout << "Equal sides do not contain same letters" << endl;
        return 2;
    }
    //return 0;
}


int S[100];  //分析栈
int S_top = 0;  //分析栈栈顶
char T[100];  //符号栈 栈顶与分析栈总相同
string value[100];  //存储符号的属性
list<siyuanshi> L_sys;  //四元式
list<int> sys;  //产生四元式的状态集
list<var> V;  //变量表


int result_count = 0;
string G[12] = { "V->S","S->f[A]=E","E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->i","A->i","A->i,A"};

act getACTION(int i, char b)
{
    char c = b;
    if (b >= 'a' && b <= 'z'&&b!='f')
        b = 'i';
    if(b=='f')
        b='f';
    if(b >= 'A' && b <= 'Z'){
        cout<<"Error: In line 1 at position 1: Lexer error occurs after token [, the following token could not be recognized to be any type of token."<<endl;
        exit(0);}
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
    int V_num = 0;  //符号表
    int index = 0;
    S[0] = 0;  //init
    cout << "分析栈：(| |中的为非终结符号的属性)" << endl;
    while (getACTION(S[S_top], input[index]).tag != 'r' || getACTION(S[S_top], input[index]).action != 0)  //acc
    {
        if (getACTION(S[S_top], input[index]).tag == 0)  //ERROR
        {
            cout << "Error: In line 1, at position 1: Could not analyze the grammar." << endl;  //出错处理
            return;
        }
        else if (getACTION(S[S_top], input[index]).tag == 's')
        {   //        push
            S[S_top + 1] = getACTION(S[S_top], input[index]).action;  //push操作
            T[S_top + 1] = input[index];
            value[S_top + 1] = input[index];

            S_top++;
            index++;  //advance();读取下一个符号
        }
        else if (getACTION(S[S_top], input[index]).tag == 'r')
        {
            char arg1, arg2, result;  //四元式
            int i = getACTION(S[S_top], input[index]).action;  //要规约的产生式
            string str = "";
            for (int j = 0; j < G[i].length() - 3; j++)
            {
                //        pop操作
                if (value[S_top] != "(" && value[S_top] != ")")
                {
                    string ss = "";
                    ss += value[S_top];
                    ss += str;
                    str = string(ss);
                }
                if (j == 0)
                    if (value[S_top].length() == 1)  //只是单个符号
                        arg1 = value[S_top][0];
                    else  //查表，查找符号
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
            for (list<int>::iterator ii = sys.begin(); ii != sys.end(); ii++)  //生成四元式
                if (i == *ii)
                {
                    result = newTemp();
                    V.push_back({ result,str });
                    /*if(G[i][4]=='+')
                    L_sys.push_back({ 'ADD',arg1,arg2,result });
                    if(G[i][4]=='-')
                    L_sys.push_back({ 'SUB',arg1,arg2,result });
                    if(G[i][4]=='*')
                    L_sys.push_back({ 'MUL',arg1,arg2,result });
                    if(G[i][4]=='/')
                    L_sys.push_back({ 'DIV',arg1,arg2,result });
                    if(G[i][4]=='=')
                    L_sys.push_back({ 'ASSIGN',arg1,arg2,result });*/
                    L_sys.push_back({ G[i][4],arg1,arg2,result });
                    break;
                }
            //push
            S[S_top + 1] = getGOTO(S[S_top], G[i][0]);
            T[S_top + 1] = G[i][0];
            value[S_top + 1] = str;  //属性传递
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
    cout << endl << "匹配成功！" << endl;
    cout << endl << "四元式：" << endl;
    cout << "(DEF_FUNC,f,,)" << endl;
    for (list<siyuanshi>::iterator s = L_sys.begin(); s != L_sys.end(); s++)
    {
        if(s->op=='+')
            cout << '(' << "ADD" << "," << s->arg2 << "," << s->arg1 << ',' << s->result << ')' << endl;
        if(s->op=='-')
            cout << '(' << "SUB" << "," << s->arg2 << "," << s->arg1 << ',' << s->result << ')' << endl;
        if(s->op=='*')
            cout << '(' << "MUL" << "," << s->arg2 << "," << s->arg1 << ',' << s->result << ')' << endl;
        if(s->op=='/')
            cout << '(' << "DIV" << "," << s->arg2<< "," << s->arg1 << ',' << s->result << ')' << endl;
        //if(s == L_sys.end())
            //cout << '(' << "ASSIGN" << "," << s->result << "," << " " << ',' << "R" << ')' << endl;
        //cout << '(' << s->op << "," << s->arg1 << "," << s->arg2 << ',' << s->result << ')' << endl;
    }
    list<siyuanshi>::iterator s  = L_sys.end();
    cout << '(' << "ASSIGN" << "," << s->result << "," << " " << ',' << "R" << ')' << endl;

    cout  << "(END_FUNC,,,)" << endl;
}


void print(sentence s)
{
    int r;
    cout << "  ";
    for (r = 0; r < s.str.length(); r++)
        if (r == s.tag)
            cout << "·" << s.str[r];
        else
            cout << s.str[r];
    if (r == s.tag)
        cout << "·";
    cout << endl;
}




void find(char ch, int tag_Vn)
{
    for (int qq = 0; qq < 12; qq++)
    {
        if (ch == G[qq][0])
        {
            if (G[qq][3] >= 'A' && G[qq][3] <= 'Z')
                if (G[qq][3] == ch)  //避免死循环重复查找
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
void scan()  //构造符号集
{
    for (int i = 0; i < 12; i++)//文法长度
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
        for (int qq = 0; qq < 12; qq++)
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
    int follow_sum = 0;  //follow集数量总和
    int previous_sum = -1;  //之前的总和
    while (follow_sum != previous_sum)
    {
        for (int i = 0; i < 12; i++)
        {
            for (int j = 3; j < G[i].length() - 1; j++)
            {
                if (isupper(G[i][j]))
                    if (!isupper(G[i][j + 1]))  //是终结符号
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
                    else  //是非终结符号
                    {
                        for (list<char>::iterator ii = Follow[findVn(G[i][j + 1])].begin(); ii != Follow[findVn(G[i][j + 1])].end(); ii++)
                        {
                            if (find(Follow[findVn(G[i][j + 1])].begin(), Follow[findVn(G[i][j + 1])].end(), *ii) == Follow[findVn(G[i][j + 1])].end())  //不重复
                                Follow[findVn(G[i][j])].push_back(*ii);
                        }
                    }
            }
            if (isupper(G[i][G[i].length() - 1]))
                for (list<char>::iterator ii = Follow[findVn(G[i][0])].begin(); ii != Follow[findVn(G[i][0])].end(); ii++)
                {
                    if (find(Follow[findVn(G[i][G[i].length() - 1])].begin(), Follow[findVn(G[i][G[i].length() - 1])].end(), *ii) == Follow[findVn(G[i][G[i].length() - 1])].end())  //不重复
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
    cout << "终结符号集：";
    for (iter = Vt.begin(); iter != Vt.end(); iter++)
    {
        cout << *iter;
    }
    cout << endl;
    cout << "非终结符号集：";
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
                for (int tt = 0; tt < 12; tt++)  //查找对应的产生式
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
int compare(I a, I b) //是否相等
{

    if (a.l.size() != b.l.size())
        return 1;
    else
    {
        for (list<sentence>::iterator iii = a.l.begin(), jjj = b.l.begin(); iii != a.l.end() || jjj != b.l.end(); iii++, jjj++)
            if (iii->str != jjj->str || iii->tag != jjj->tag)  //判断是否相等
            {
                return 1;
            }
    }
    return 0;
}


void table()
{
    int num = 0;
    sentence first;  //起始符号
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
        for (iter = dfa->l.begin(); iter != dfa->l.end(); iter++)  //对状态中的每一产生式右部进行处理
            if (iter->finish == 0)
            {
                I C;  //新状态
                C.number = DFA.size();
                if (iter->tag < iter->str.length())  //尚未完成
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
                    for (list<I>::iterator ii = DFA.begin(); ii != DFA.end(); ii++, count++)  //判断是否有重复
                    {
                        judge = compare(*ii, closure(C));  //修改C
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
                else  //已经完成
                {
                    int cc = 0, tt;
                    for (tt = 0; tt < 12; tt++)
                    {
                        if (iter->str == G[tt])
                            break;
                    }
                    for (list<char>::iterator c = Vn.begin(); c != Vn.end(); c++, cc++)
                        if (*c == iter->str[0])
                            break;
                    for (list<char>::iterator c = Follow[cc].begin(); c != Follow[cc].end(); c++)
                    {
                        ACTION.push_back({ dfa->number,*c,'r',tt });  //对应的第j条产生式
                        for (int ss = 3; ss < G[tt].length(); ss++)  //查找四元式对应的产生式
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
        cout << "状态" << num++ << "：" << endl;
        for (list<sentence>::iterator iii = dfa->l.begin(); iii != dfa->l.end(); iii++)
        {
            cout << "    ";
            print(*iii);
        }
    }
    cout << endl;
    int kkk = 0;
    for (list<go>::iterator g = GOTO.begin(); g != GOTO.end(); g++)
    {
        cout << "GOTO(" << g->I << ", " << g->Vn << ") = " << g->next_I<<"       ";
        kkk++;
        if (kkk % 4 == 0) {
            cout << endl;
        }
    }
    cout << endl << endl;
    kkk = 0;
    for (list<act>::iterator a = ACTION.begin(); a != ACTION.end(); a++)
    {
        cout << "ACTION(" << a->I << ", " << a->Vt << ") = " << a->tag << a->action << "       ";
        kkk++;
        if (kkk % 4 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

int main()
{
    //readfile();
    int p;
    p=readfile();
    scan();
    table();
    if(p==2){
         cout << "Error: Used undeclared variable y" <<endl;
        return 0;}
    SLR();
    cout << "---------------------20231045------------------------" << endl;
    return 0;
}


bool hasSameLetters(string str) {
    unordered_set<char> left, right;
    int equals_index = str.find("=");
    for (int i = 0; i < equals_index; i++) {
        left.insert(str[i]);
    }
    for (int i = equals_index + 1; i < str.length(); i++) {
        right.insert(str[i]);
    }
    for (auto letter : left) {
        if (right.count(letter) > 0) {
            return true;
        }
    }
    return false;
}


