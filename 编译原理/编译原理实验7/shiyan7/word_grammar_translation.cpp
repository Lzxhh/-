#include <bits/stdc++.h>
#include <string>
#include <iomanip>
#include <utility>
#include <iostream>
#include <stdexcept>
#define _GLIBCXX_USE_C99 1
#include<cstring>
using namespace std;
const int maxn = 1002;
const int n_key = 41;
const int n_oper = 21;

char c;

string key_word[n_key] = {"auto", "enum", "restrict", "unsigned", "break",
                    "extern", "return", "void", "case", "float",
                    "short", "volatile", "char", "for", "signed",
                    "while", "const", "goto", "sizeof", "_Bool",
                    "continue", "if", "static", "_Complex", "default",
                    "inline", "struct", "_Imaginary", "do", "int",
                    "switch", "double", "long", "typedef", "else",
                    "register", "union", "scanf", "printf", "cin",
                    "cout"
};

string oper[] = {"+", "-", "*", "/", "^",
                "<", ">", "++", "--", "==",
                "*=", "/=", ">=", "<=", "<<",
                ">>", ">>=", "<<=", "%", "&",
                "^"
};

char bound[] = {',', ';', '(', ')', '[', ']', '{', '}'};

struct Word{                //�ʽṹ��
    int id;
    string value;
};

struct Num{
    int id;
    int vi;
    double vd;
};
Num n[maxn];        //����
Word w[maxn];       //��
map<string, int> m;    //��ʶ��
int f = 0, ff = 0;



//�﷨����
typedef pair<string, string> P;
map<P, string> pre_list;
typedef set<string> Sets;
map<string, Sets> first, follow, wenfa, select;      //first, follow
Sets Term, Nterm;                //�ռ��������ռ���
string Start;

typedef struct node{
    string value;
    string type;
    int number;
}pro;
vector<pro> watest;





//translation����

//�ʷ�����
bool is_oper(string s){
    for(int i=0; i<n_oper; i++) if(s == oper[i]) return true;
    return false;
}

bool is_bound(char c){
    for(int i=0; i<sizeof(bound); i++) if(c == bound[i]) return true;
    return false;
}

bool is_key(string s){
    for(int i=0; i<n_key; i++) if(s == key_word[i]) return true;
    return false;
}

int stoi(string s){                         //get int
    int ans = 0;
    for(int i=0; i<s.size(); i++) ans = ans * 10 + s[i] - '0';
    return ans;
}

double stof(string s){                      //get double
    long long ans = 0;
    int fd = -1, fe = -1;
    for(int i=0; i<s.size(); i++){
        if(s[i] == '.'){
            fd = i;
            continue;
        }
        if(s[i] == 'e'){
            fe = i;
            continue;
        }
        ans = ans * 10 + s[i] - '0';
    }
    if(fd != -1 && fe == -1) return double(ans)/(pow(10, s.size() - fd - 1));
    else if(fd == -1 && fe != -1){
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //�õ�e�������
        ans /= pow(10, s.size() - fe - 1);                          //�õ�eǰ������
        return double(ans*pow(10, temp));
    }
    else{
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //�õ�e�������
        cout<<ans<<" "<<s.size() - fe - 1<<" "<<temp<<endl;
        ans /= pow(10, s.size() - fe - 1);                                //�õ�eǰ������
        cout<<ans<<endl;
        long long tt = (s.size() - fd - 1) - (s.size() - fe - 1) - 1;     //�õ�.�������
        cout<<tt<<endl;
        return (double)ans/pow(10, tt) * (pow(10, temp));
    }
}

void getword(char filename[]){
    freopen(filename, "r", stdin);              //�ض���

    string str = "";
    int flag, is_end;
    is_end = scanf("%c", &c);
    while(is_end != EOF){
        flag = 0;
        if(isspace(c)){
            while(isspace(c) && is_end != EOF){   //�˿ո�
                is_end = scanf("%c", &c);
            }
        }
        if(isalpha(c) || c == '_'){                         //����ĸ���»��߿�ͷ
            while(isalnum(c) || c == '_' || isalpha(c)){
                str += c;
                is_end = scanf("%c", &c);
            }
            w[++f].value = str;
            if(is_key(str)){
                w[f].id = 1;
                if(str == "int" || str == "float" || str == "long" || str == "double" || str == "char" || str == "bool")
                    watest.push_back(pro{str, "type", 1});
                else watest.push_back(pro{str, str, 1});
            }         //������
            else{
                w[f].id = 2;                     //��ʶ��
                m[str] ++;
                watest.push_back(pro{str, "id", 2});
            }
            str = "";
            flag = 1;
        }
        if(isdigit(c)){                          //����
            int fd = 0, fe = 0, fflag = 0;
            while(isdigit(c) || c == '.' || c == 'e'){
                if(c == '.') fd ++;
                if(c == 'e') fe ++;
                if(c == '.' && fe) fflag = 1;
                str += c;
                is_end = scanf("%c", &c);
            }
            if(str[str.size()-1] == '.' || str[str.size()-1] == 'e') fflag = 1;
            if(fflag){
                cout<<"����-->���Ϸ����֣�"<<str<<endl;    //���Բ��Ϸ�����
            }
            else{
                watest.push_back(pro{str, "number", 3});
                if(!fd && !fe){
                    n[++ff].vi = stoi(str);
                    n[ff].id = 1;
                }
                else{
                    n[++ff].vd =stof(str);
                    n[ff].id = 2;
                }
                w[++f].id = 3;  w[f].value = str;
            }
            str = "";
            flag = 1;
        }
        if(is_bound(c)){            //���
            str += c;
            w[++f].value = str;
            w[f].id = 4;

            watest.push_back(pro{str, str, 4});

            is_end = scanf("%c", &c);
            str = "";
            flag = 1;
        }
        string ss = "";
        ss += c;
        if(is_oper(ss)){
            while(is_oper(ss)){
                str += c;
                is_end = scanf("%c", &c);
                ss += c;
            }
            if(is_oper(str)){
                w[++f].value = str;
                w[f].id = 5;

                watest.push_back(pro{str, str, 5});
            }
            str = "";
            flag = 1;
        }
        if(!flag && is_end != EOF){
            str += c;
            w[++f].value = str;
            w[f].id = 6;

            watest.push_back(pro{str, str, 6});

            is_end = scanf("%c", &c);
            str = "";
        }
    }
    freopen("CON", "r", stdin);           //�ر��ض���,�ָ���׼
}

void to_file1(){                        //����
//    char filename[20];
//    puts("������ʷ���������Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    fstream out;
    out.open("main_table.txt", ios::out);
    out<<"1.������   2.��ʶ��   3.����   4.���   5.������   6.����"<<endl<<endl;
    for(int i=1; i<=f; i++) out<<w[i].id<<"  "<<w[i].value<<endl;
    out.close();
}

void to_file2(){                        //��ʶ����
//    char filename[20];
//    puts("������ʷ�������ʶ����Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    ofstream out;
    out.open("id_table.txt", ios::out);
    map<string, int>::iterator it;
    for(it=m.begin(); it!=m.end(); it++) out<<it->first<<endl;
    out.close();
}

void to_file3(){                        //���ֱ�
//    char filename[20];
//    puts("������ʷ��������ֱ�Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    ofstream out;
    out.open("number_table.txt", ios::out);
    out<<"1.int   2.double"<<endl<<endl;
    for(int i=1; i<=ff; i++) out<<n[i].id<<"  "<<(n[i].id == 1 ? n[i].vi : n[i].vd)<<endl;
    out.close();
}

void to_file4(){
    ofstream out;
    out.open("twox.txt", ios::out);   //�ʷ������������Ԫ�飩
    int len = watest.size();
    for(int i=0; i<len-1; i++) out<<watest[i].value<<"  "<<watest[i].type<<"  "<<watest[i].number<<endl;
    out<<watest[len-1].value<<"  "<<watest[len-1].type<<"  "<<watest[len-1].number<<endl;
    out.close();
}


bool Word_analysis(){
//    char filename[20];
//    puts("��������Ҫ�򿪵��ļ���ַ��");
//    scanf("%s", filename);

    getword("programe.txt");

    to_file1();
    to_file2();
    to_file3();
    to_file4();

    return true;
}






//�﷨����
void get_wenfa(){
    freopen("grammar.txt", "r", stdin);

    string line, s, foo;
    int cnt = 0;
    while(getline(cin, line)){
        cnt ++;
        int len = line.size();
        int i = 0;
        while(line[i] != ' ' && i < len) i ++;
        foo = line.substr(0, i);
        if(cnt == 1) Start = foo;
        i++;
        while(line[i] != ' ' && i < len) i ++;   //��ȥ ->
        for(; i<len; i++){
            if(line[i] != ' '){
                s = "";
                while(!(line[i] == '|' && line[i-1] == ' ' && line[i+1] == ' ') && i < len){
                    s += line[i++];
                }
                while(s[s.size()-1] == ' ') s.erase(s.size()-1, 1);
                wenfa[foo].insert(s);
            }
        }
    }

//    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //  ����ķ�
//        cout<<it->first<<"->";
//        for(Sets::iterator ti = (it->second).begin(); ti != (it->second).end(); ti ++){
//            if(ti == it->second.begin()) cout<<*ti;
//            else cout<<" | "<<*ti;
//        }
//        puts("");
//    }

    freopen("CON", "r", stdin);
}

void get_ch(){
    fstream in;
    in.open("grammar.txt", ios::in);

    string str;
    while(in >> str){
        if(str == "->") continue;
        if(str[0] == '<' && str[str.size()-1] == '>') Nterm.insert(str);
        else Term.insert(str);
    }
    Term.insert("#");
    in.close();
}

void get_first(){
    int flag = 1;
    Sets tt;
    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //���з��ռ���
            string X = it->first;       //���ڷ��ռ���X

            if(wenfa[X].count("@") && !first[X].count("@")){   //����@������FIRST
                first[X].insert("@");
                flag = 1;
            }

            for(Sets::iterator f = wenfa[X].begin(); f != wenfa[X].end(); f ++){    //����s���в���ʽ
                string foo = *f, str = "", last = "";                               //���ڲ���ʽX->Y1Y2Y3...

                int i = 0;
                while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];    //�ҵ�Y1
                if(str == "") continue;

                if(!(str[0] == '<' && str[str.size()-1] == '>')){     //Y1���ռ���
                    if(!first[X].count(str)){
                        first[X].insert(str);
                        flag = 1;
                    }
                    if(str != "@") continue;      //���ռ���������@
                }
                else{     //Y1�Ƿ��ռ���
                    for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                        if(*ii != "@" && !first[X].count(*ii)){
                            first[X].insert(*ii);
                            flag = 1;
                        }
                    }
                    if(!first[str].count("@")) continue;
                }

                for( ; i<foo.size(); i++){
                    if(foo[i] != ' '){
                        last = str;
                        str = "";
                        while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];

                        if(last == "@" || first[last].count("@")){
                            for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                                if(*ii != "@" && !first[X].count(*ii)){
                                    first[X].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }
                        else break;
                    }
                }

                if(i >= foo.size() && (str == "@" || first[str].count("@"))){                          //Y1Y2Y3...Yn��FIRST���ɲ���@
                    if(!first[X].count("@")){
                        first[X].insert("@");
                        flag = 1;
                    }
                }
            }
        }
    }

//���FIRST��
    fstream out;
    out.open("FIRST.txt", ios::out);       //�������FIRST
    for(map<string, Sets>::iterator t = first.begin(); t != first.end(); t ++){
        out<<"FIRST("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_follow(){
    int flag = 1;
    int cnt;
    string str, foo, s, last, tr;
    Sets tt;

    follow[Start].insert("#");   //��#����follow(Start)

    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
            str = it->first;                                           //����FOLLOW(str)

            for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
                foo = *ti;                                      //���ڲ���ʽstr->foo��str->ABC

                int len = foo.size(), i = 0;
                s = "";
                while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                if(s == "") continue;

                for( ; i<len; i++){
                    if(foo[i] != ' '){
                        last = s;
                        s = "";
                        while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                        if(last[0] != '<' || last[last.size()-1] != '>') continue;  //B���ս��������

                        if(s[0] != '<' || s[s.size()-1] != '>'){   //C���ս��
                            if(s != "@" && !follow[last].count(s)){
                                follow[last].insert(s);
                                flag = 1;
                            }
                            if(s != "@") continue;
                        }
                        else{    //C�Ƿ��ս�����Ȱ�first(C) - {@}����
                            for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                                if(*ii != "@" && !follow[last].count(*ii)){
                                    follow[last].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }

                        if(first[s].count("@") || s == "@"){   //C->*@,�Ϳ������
                            string next = s;
                            int k = i;
                            for( ; k < len; k++){
                                if(foo[k] != ' '){
                                    next = "";
                                    while(foo[k] != ' ' && k < len){ next += foo[k ++]; }

                                    if(next[0] != '<' || next[next.size()-1] != '>'){    //next���ս��
                                        if(next != "@" && !follow[last].count(next)){
                                            follow[last].insert(next);
                                            flag = 1;
                                        }
                                        if(next != "@") break;
                                    }
                                    else{    //next�Ƿ��ս��
                                        for(Sets::iterator ii = first[next].begin(); ii != first[next].end(); ii ++){
                                            if(*ii != "@" && !follow[last].count(*ii)){
                                                follow[last].insert(*ii);
                                                flag = 1;
                                            }
                                        }
                                        if(!first[next].count("@")) break;
                                    }
                                }
                            }
                            if(k >= len && (next == "@" || first[next].count("@"))){    //����str->ABC,C->*@,��follow(str) - {@}�ӵ�follow(B)��
                                for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                                    if(!follow[last].count(*ii)){
                                        follow[last].insert(*ii);
                                        flag = 1;
                                    }
                                }
                            }
                        }
                    }
                }
                if(i >= len && (s[0] == '<' && s[s.size()-1] == '>')){    //����str->ABC,��follow(str) - {@}�ӵ�follow(C)��
                    for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                        if(!follow[s].count(*ii)){
                            follow[s].insert(*ii);
                            flag = 1;
                        }
                    }
                }
            }
        }
    }

//���FOLLOW��
    fstream out;
    out.open("FOLLOW.txt", ios::out);
    for(map<string, Sets>::iterator t = follow.begin(); t != follow.end(); t ++){
        out<<"FOLLOW("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_select(){
    int flag = 1;
    string str, foo, s, fxx;
    Sets tt;
    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
        str = it->first;                                           //����str

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            foo = *ti;
            fxx = str + " " + foo;            //����select(str->foo)

            tt.clear();
            int len = foo.size();
            for(int i=0; i<len; i++){
                if(foo[i] != ' '){
                    s = "";
                    while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                    if(s[0] != '<' || s[s.size()-1] != '>'){  //s���ս��
                        if(s != "@"){
                            tt.insert(s);
                            break;
                        }
                    }else{
                        for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                            if(*ii != "@") tt.insert(*ii);
                        }
                        if(!first[s].count("@")) break;
                    }
                }
            }
            if(s == "@" || first[s].count("@")){
                set_union(select[fxx].begin(), select[fxx].end(), follow[str].begin(), follow[str].end(), inserter(select[fxx], select[fxx].begin()));
                set_union(select[fxx].begin(), select[fxx].end(), tt.begin(), tt.end(), inserter(select[fxx], select[fxx].begin()));
            }
            else set_union(tt.begin(), tt.end(), select[fxx].begin(), select[fxx].end(), inserter(select[fxx], select[fxx].begin()));
        }
    }

//���SELECT��
    fstream out;
    out.open("SELECT.txt", ios::out);
    for(map<string, Sets>::iterator t = select.begin(); t != select.end(); t ++){
        out<<"SELECT("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_pre_list(){
    get_ch();
    pre_list.clear();
    string str, foo;
    for(map<string, Sets>::iterator it = select.begin(); it != select.end(); it ++){
        int i = 0;
        str = foo = "";
        while(it->first[i] != ' ' && i < it->first.size()) str += it->first[i ++];
        foo = it->first.substr(i+1, it->first.size() - i - 1);

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            pre_list[P(str ,*ti)] = foo;
        }
    }
    fstream out;
    out.open("pre_list.txt", ios::out);

    out<<"        ";
    for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
        out<<setw(8)<<*ti;
    }
    out<<endl;
    for(Sets::iterator it = Nterm.begin(); it != Nterm.end(); it ++){
        out<<setw(8)<<*it;
        for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
            out<<setw(8)<<(pre_list[P(*it, *ti)] == "" ? "$" : pre_list[P(*it, *ti)]);
        }
        out<<endl;
    }
    out.close();
}

bool analysis(ofstream &out){         //��������
    vector<string> p;
    string t, foo, str, fxx;

    p.push_back("#");       //Ԥ����
    p.push_back(Start);
    watest.push_back(pro{"#", "#", 0});

    out<<"------------------------------------------------------------------------------\n";
    out<<"  ���� |    ��ǰջ��    |     ��ǰ����     |   �Ƶ����ò���ʽ                 \n";
    out<<"------------------------------------------------------------------------------\n";
    int step = 1, pos = 0;
    while(!p.empty() && pos < watest.size()){
        out<<setw(6)<<(step ++)<<setw(18)<<p.back()<<setw(19)<<watest[pos].type;
        if(p.back() == "#" && watest[pos].type == "#"){
            out<<"    "<<"����"<<endl;
            return true;
        }

        if(p.back() == watest[pos].type){
            out<<"    "<<"��"<<watest[pos].type<<"��"<<"ƥ��"<<endl;
            p.pop_back();
            pos ++;
        }
        else{
            t = pre_list[P(p.back(), watest[pos].type)];
            if(t == "") return false;
            out<<"    "<<p.back()<<" -> "<<t<<endl;
            if(t == "@") t = "";

            p.pop_back();

            int i = t.size()-1;
            for( ; i >= 0; i--){
                if(t[i] != ' '){
                    foo = "";
                    while(t[i] != ' ' && i >= 0) foo += t[i --];
                    reverse(foo.rbegin(), foo.rend());
                    p.push_back(foo);
                }
            }
        }
    }
    return false;
}

bool grammar_analysis(){
    get_wenfa();
    get_first();
    get_follow();
    get_select();
    get_pre_list();

//�����������
    ofstream out;
    out.open("process.txt", ios::out);
    bool flag = analysis(out);
    out<<(flag ? "\n\n-->LL(1)�Ϸ�����\n\n" : "\n\n-->LL(1)�Ƿ�����\n\n");
    out.close();
    return flag;
}




//����
int cnt = 0;
pro now, last;
vector<string> var;
struct Four_exp{   //��Ԫʽ�ṹ��
    string op;
    string r1, r2;
    string left;
};
vector<Four_exp> four_exp, assemble;   //��Ԫʽ�����ָ���

int cnt_temp = 0;

map<string, int> pri;   //���ȼ�����

void init_pri(){    //���ȼ�����
    pri["("] = 10;
    pri["++"] = pri["--"] = 9;
    pri["*"] = pri["/"] = pri["%"] = 8;
    pri["+"] = pri["-"] = 7;
    pri[">"] = pri["<"] = pri[">="] = pri["<="] = 6;
    pri["!="] = pri["=="] = 5;
    pri["&&"] = 4;
    pri["||"] = 3;
    pri["="] = 2;
    pri[")"] = pri["@"] = 1;
}

void get_token(){    //��ȡ��Ԫʽ
    last = now;
    now = watest[cnt ++];
}

bool expect(string Expect){   //ƥ�亯��
    if(now.type == Expect || now.value == Expect){
        get_token();
        return true;
    }
    else return false;
}

string itos(int x){    //int to string
    stringstream ss;
    ss<<x;
    return ss.str();
}

string Caculate(string stop){   //���ʽ
    int             bra = 0;
    string          op;
    string          arg1, arg2, t = "";
    string          v;
    stack<string>   num;
    stack<string>   opr;
    opr.push("@");
    while(true) {
        v = now.value;
        //������ʽ�Ƿ����
        if(v == "(")    bra--;
        if(v == ")")    bra++;
        if(bra > 0 || expect(stop))  v = "@";

        //���ʽ����
        if(v != "@" && (now.type == "id" || now.type == "number")) {//��Ϊ��������ʱ
            num.push(v);
            if(expect("id"))         t = "";
            else if(expect("number")) t = "";
        }
        else {//��Ϊ�����ʱ
            op = opr.top();
            while(pri[v] <= pri[op] && !(op == "(" && v != ")")) {
                if(op == "@" && v == "@"){
                    return num.top();
                }
                opr.pop();

                if(op == "(" && v == ")") {
                    break;
                }
                else if(op == "++" || op == "--") {//��Ϊ��Ŀ�����
                    arg1 = num.top();   num.pop();
                    four_exp.push_back(Four_exp{op, arg1, "_", "t" + itos(cnt_temp ++)});
                }
                else {//��Ϊ˫Ŀ�����
                    arg2 = num.top();   num.pop();
                    arg1 = num.top();   num.pop();

                    four_exp.push_back(Four_exp{op, arg1, arg2, "t" + itos(cnt_temp ++)});
                }

                num.push("t" + itos(cnt_temp - 1));
                op = opr.top();
            }
            if(v != ")" && v != "@")    opr.push(v);
            expect(v);
        }
    }
}

void fuzhi(pro left, string stop){   //��ֵ���
    expect("=");
    string tt = Caculate(stop);
    four_exp.push_back(Four_exp{"=", tt, "_", left.value});
}

bool body(){    //������
    while(true){
        if(expect("type")){        //�������
            if(expect("id")){
                var.push_back(last.value);
                expect(";");
            }
        }
        else if(expect("cin")){
            while(expect(">>") && expect("id")) four_exp.push_back(Four_exp{"cin", "_", "_", last.value});
            expect(";");
        }
        else if(expect("cout")){
            while(expect("<<") && expect("id")) four_exp.push_back(Four_exp{"cout", "_", "_", last.value});
            expect(";");
        }
        else if(expect("id")){     //��ֵ���
//            if(find(var.begin(), var.end(), now.value) == var.end()){
//                cout<<"Variable"<<now.value<<"is not declare in here��"<<endl;
//                return false;
//            }
            fuzhi(last, ";");
        }
        else if(expect("if") && expect("(")){
            string tt = Caculate(")");
            expect("{");
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+2) + ")"});
            four_exp.push_back(Four_exp{"J", "_", "_", ""});
            int ad = four_exp.size() - 1;
            body();
            if(expect("else") && expect("{")){
                four_exp.push_back(Four_exp{"J", "_", "_", ""});
                int sd = four_exp.size() - 1;
                four_exp[ad].left += "(" + itos(four_exp.size()) + ")";
                body();
                four_exp[sd].left += "(" + itos(four_exp.size()) + ")";
            }
            else four_exp[ad].left += "(" + itos(four_exp.size()) + ")";
        }
        else if(expect("while") && expect("(")){
            string tt = Caculate(")");
            expect(")");
            expect("{");
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+2) + ")"});
            four_exp.push_back(Four_exp{"J", "_", "_", ""});
            int ad = four_exp.size() - 1;
            body();
            four_exp.push_back(Four_exp{"J", "_", "_", "(" + itos(ad - 1) + ")"});
            four_exp[ad].left += "(" + itos(four_exp.size()) + ")";
        }
        else if(expect("for") && (expect("("))){
            expect("id");
//            if(find(var.begin(), var.end(), now.value) == var.end()){
//                cout<<"Variable"<<now.value<<"is not declare in here��"<<endl;
//                return false;
//            }
            fuzhi(last, ";");

            string tt = Caculate(";");
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+3) + ")"});

            int ft = four_exp.size() + 2;
            expect("id");
//            if(find(var.begin(), var.end(), now.value) == var.end()){
//                cout<<"Variable"<<now.value<<"is not declare in here��"<<endl;
//                return false;
//            }
            fuzhi(last, ")");

            body();
            four_exp.push_back(Four_exp{"J", "_", "_", "(" + itos(ft) + ")"});

        }
        else{
            expect("}");
            return true;
        }
    }
    return false;
}

void four_exp_to_file(){     //�����Ԫʽ
    fstream out;
    out.open("four_exp.txt", ios::out);

    int n = four_exp.size();
    out<<"============================================================="<<endl;
    out<<"                         Four_expression                     "<<endl;
    out<<"============================================================="<<endl;
    for(int i=0; i<n; i++){
        out<<"("<<i<<")"<<"  ( "<<four_exp[i].op<<" , "<<four_exp[i].r1<<" , "<<four_exp[i].r2<<" , "<<four_exp[i].left<<" )"<<endl;
    }
}

void to_assemble(){     //��Ԫʽ�����
    int fxx = 0;
    int n = four_exp.size();
    int cnt = 0, temp = 0;
    for(int i=0; i<n; i++){
        if(four_exp[i].op == "="){
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, four_exp[i].r1, "_"});
        }
        else if(four_exp[i].op == "cin"){
            assemble.push_back(Four_exp{"IN", four_exp[i].left, "_", "_"});
        }
        else if(four_exp[i].op == "cout"){
            assemble.push_back(Four_exp{"OUT", four_exp[i].left, "_", "_"});
        }
        else if(four_exp[i].op == "+"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});
            assemble.push_back(Four_exp{"ADD", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "-"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});
            assemble.push_back(Four_exp{"SUB", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "*"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});
            assemble.push_back(Four_exp{"MUL", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "/"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});
            assemble.push_back(Four_exp{"DIV", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "%"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});
            assemble.push_back(Four_exp{"MOD", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == ">"){
            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"JG", "(" + itos(assemble.size() + 2) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "0", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "1", "_"});
            temp ++;
            cnt += 3;
        }
        else if(four_exp[i].op == "<"){
            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"JNG", "(" + itos(assemble.size() + 2) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "0", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "1", "_"});
            temp ++;
            cnt += 3;
        }
        else if(four_exp[i].op == "J"){
            string s = four_exp[i].left;
            s.erase(0, 1);
            s.erase(s.size()-1, 1);
            int foo = stoi(s) + cnt;
            if(++fxx == 1) foo += 8;
            else foo -= cnt;

            assemble.push_back(Four_exp{"JMP", "(" + itos(foo) + ")", "_", "_"});
        }
        else if(four_exp[i].op == "J="){
            string s = four_exp[i].left;
            s.erase(0, 1);
            s.erase(s.size()-1, 1);
            int foo = stoi(s) + cnt;

            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"JZ", "(" + itos(foo) + ")", "_", "_"});
            cnt ++;
        }
    }

//������ָ��
    fstream out;
    out.open("assemble.txt", ios::out);

    int len = assemble.size();
    out<<"==================================================================="<<endl;
    out<<"                            Assemble_code                          "<<endl;
    out<<"==================================================================="<<endl;
    for(int i=0; i<len; i++){
        if(assemble[i].op[0] == 'J' || assemble[i].op == "IN" || assemble[i].op == "OUT") out<<"("<<i<<")"<<"   "<<assemble[i].op<<"   "<<assemble[i].r1<<endl;
        else out<<"("<<i<<")"<<"   "<<assemble[i].op<<"   "<<assemble[i].r1<<" , "<<assemble[i].r2<<endl;
    }
    out.close();
}


bool translation(){
    init_pri();
    int tt = 0;
    get_token();
    if(expect("int") && expect("main") && expect("(") && expect(")") && expect("{")){
        if(body()){
            four_exp_to_file();
            to_assemble();
            return true;
        }
    }
    four_exp_to_file();
    return false;
}




int main(){
    if(Word_analysis()){
        puts("�ʷ��������ɹ�!");
        if(grammar_analysis()){
            puts("�﷨�������ɹ�!");
            if(translation()) puts("���룺�ɹ���");
            else puts("���룺ʧ�ܣ�");
        }
        else puts("�﷨������ʧ�ܣ�");
    }
    else puts("�ʷ�������ʧ�ܣ�");

    return 0;
}


