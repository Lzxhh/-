#include<bits/stdc++.h>
# pragma warning (disable:4819)
using namespace std;

char Stack[100],a[200];
FILE *fp;

struct GRAM
{
    //$�����
    string  a;
    string first;
    string follow;
};


//�ж��ս���ͷ��ս�� ����1�Ƿ��ս�� ����0���ս��
int Norterminal(char c)
{
    if(c>='A'&&c<='Z')
        return 1;
    else if(c!=':'&&c!='='&&c!='<'&&c!='>'&&c!=' '&&c!='|')
        return 0;

}

void analyze()
{
    GRAM gram[50];
    string grammer;
    cout<<"�������ķ�����#������"<<endl;
    std::vector<string> strN;
    cin>>grammer;//�������
    strN.push_back(grammer);

    /*std::vector<string> strN;
    int i=0,j=0;
	if((fp=fopen("wenfa.txt","r"))==NULL)
	{
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(0);
	}
	grammer[i++]=fgetc(fp);
	/*while(!feof(fp))
	{
		a1[i++]=fgetc(fp);
		if((i%5)==0)
			a[j++]=a1[i-2];
	}
	fclose(fp);*/


    char str[10][80];
    const char *ch = "|";
    char *result;
    vector<string> strN1;  //�洦�����|�������

    for(int i=0;i<grammer.length();i++)
    {
        str[0][i]=grammer[i];
    }
    for(int h=grammer.length();h<80;h++)
    {
            str[0][h]=NULL;
    }
    result = strtok(str[0],ch);
    while(result!=NULL)
    {
        strN1.push_back(result);
        result = strtok(NULL,ch);
    }
    for(int i=1;grammer!="#";i++)
    {
        cin>>grammer;
        strN.push_back(grammer);
        /*����ת���Ĺ�����ʽ���벿��*/
        for(int h=0;h<grammer.length();h++)
        {
            str[i][h]=grammer[h];
        }
        for(int h=grammer.length();h<80;h++)
        {
            str[i][h]=NULL;
        }
        result = strtok(str[i],ch);
        while(result!=NULL)
        {
            strN1.push_back(result);
            result = strtok(NULL,ch);
        }
    }
    /*
    ��ȡfirstTerm����
    */
    for(unsigned i=0;i<strN.size();i++)
    {
        gram[i].a=strN[i][0];
        gram[i].first="";
        if(!Norterminal(strN[i][4]))
        {
            gram[i].first=strN[i][4];
        }
        else
        {
            if(!Norterminal(strN[i][5]))
            {
                gram[i].first=strN[i][5];
            }
        }
        for(unsigned k=5;k<strN[i].length();k++)
        {
            if(strN[i][k]=='|')
            {

                if(!Norterminal(strN[i][k+1]))
                {

                    gram[i].first+=strN[i][k+1];
                }
                else
                {
                    if(!Norterminal(strN[i][k+2]))
                    {
                        gram[i].first+=strN[i][k+2];
                    }
                }
            }
        }

    }
    int num=strN1.size();
    //����һ���ַ��Ƿ��ս��ʱ
    for(int i=num-1;i>=0;i--)
    {
        for(unsigned l=0;l<strN.size()-1;l++)
        {
            if(gram[l].a[0]==strN1[i][0])
            {
                    for(unsigned h=0;h<strN.size()-1;h++)
                    {
                        if(strN1[i][4]==gram[h].a[0]&&strN1[i][4]!=strN1[i][0])
                        {
                            gram[l].first+=gram[h].first;
                            string ustr(gram[l].first);
                            sort(ustr.begin(), ustr.end());
                            ustr.erase(unique(ustr.begin(), ustr.end()), ustr.end() );
                            gram[l].first=ustr;
                            break;
                        }
                    }
            }
        }

    }
    cout<<"firstTerm����Ϊ��"<<endl;
    for(unsigned i=0;i<strN.size()-1;i++)
    {
        for(int k=0;k<gram[i].first.length();k++)
        {
            if(gram[i].first[k]==NULL)
            {
                gram[i].first.erase(gram[i].first.begin()+k);
            }
        }
        cout<<gram[i].a<<" "<<gram[i].first<<endl;
    }
    string term="";
    cout<<"�ս��Ϊ��"<<endl;
    for(unsigned i=0;i<strN.size()-1;i++)
    {
        for(int j=0;j<strN[i].length();j++)
        {
            if(!Norterminal(strN[i][j])&&strN[i][j]!='$')
               term+=strN[i][j];
        }
        string ustr(term);
        sort(ustr.begin(), ustr.end());
        ustr.erase(unique(ustr.begin(), ustr.end()), ustr.end() );
        term=ustr;
    }
    cout<<term<<endl;
    /*ת��������ʽ*/
    cout<<"ת����Ĺ�����ʽΪ��"<<endl;
    int listnumber =0;
    for(unsigned i=0;i<strN1.size()-1&&i-1>=0;i++)
    {

        if(strN1[i].find("::")==-1)
        {
            string add="";
            add=add+strN1[i-1][0]+"::="+strN1[i];
            strN1[i]=add;

        }
        cout<<strN1[i]<<endl;
    }
    /*��ȡlastTerm*/
    //ֱ�ӻ�ȡlastTerm
    for(int i=0;i<num-1;i++)
    {
        for(unsigned l=0;l<strN.size()-1;l++)
        {
                if(gram[l].a[0]==strN1[i][0])
                {
                        for(unsigned h=0;h<strN.size()-1;h++)
                        {
                            int k=strN1[i].length();
                            if(!Norterminal(strN1[i][k-1]))
                            {

                                gram[l].follow+=strN1[i][k-1];
                                string ustr(gram[l].follow);
                                sort(ustr.begin(), ustr.end());
                                ustr.erase(unique(ustr.begin(), ustr.end()), ustr.end() );
                                gram[l].follow=ustr;
                                break;
                            }
                            else
                            {
                                if(!Norterminal(strN1[i][k-2]))
                                {
                                    gram[l].follow+=strN1[i][k-2];
                                    string ustr(gram[l].follow);
                                    sort(ustr.begin(), ustr.end());
                                    ustr.erase(unique(ustr.begin(), ustr.end()), ustr.end() );
                                    gram[l].follow=ustr;
                                    break;
                                }
                            }
                        }
                }
        }
   }
   for(int i=num-1;i>=0;i--)
   {
        for(unsigned l=0;l<strN.size()-1;l++)
        {
            if(gram[l].a[0]==strN1[i][0])
            {
                    for(unsigned h=0;h<strN.size()-1;h++)
                    {
                            int k=strN1[i].length();
                            if(Norterminal(strN1[i][k-1]))
                            {

                                 for(unsigned h1=0;h1<strN.size()-1;h1++)
                                 if(gram[h1].a[0]==strN1[i][k-1])
                                 {
                                        gram[l].follow+=gram[h1].follow;
                                        string ustr(gram[l].follow);
                                        sort(ustr.begin(), ustr.end());
                                        ustr.erase(unique(ustr.begin(), ustr.end()), ustr.end() );
                                        gram[l].follow=ustr;
                                        break;
                                 }

                            }
                    }
            }
        }

    }
    cout<<"lastTerm����Ϊ��"<<endl;
    for(unsigned i=0;i<strN.size()-1;i++)
    {
        cout<<gram[i].a<<" "<<gram[i].follow<<endl;
    }
    int list[term.length()][term.length()];
    for(int i=0;i<term.length();i++)
    {
        for(int j=0;j<term.length();j++)
        {
            list[i][j]=0;
        }
    }
    cout<<"��ȵĹ�ϵΪ��"<<endl;
   for(int i=num-1;i>=0;i--)
   {
        for(int k=0;k<strN1[i].length()&&k+1<strN1[i].length();k++)
        {
            if(Norterminal(strN1[i][k]))
            {
                if(!Norterminal(strN1[i][k-1])&&!Norterminal(strN1[i][k+1]))
                {
                    cout<<strN1[i][k-1]<<strN1[i][k+1]<<endl;;
                    for(int h=0;h<term.length();h++)
                    {
                        if(strN1[i][k-1]==term[h])
                        for(int g=0;g<term.length();g++)
                        {
                            if(strN1[i][k+1]==term[g])
                            {
                                list[h][g]=1;
                            }
                        }
                    }
                }
            }
            else
            {
                if(!Norterminal(strN1[i][k+1]))
                {
                    cout<<strN1[i][k]<<strN1[i][k+1]<<endl;
                    for(int h=0;h<term.length();h++)
                    {
                        if(strN1[i][k]==term[h])
                        for(int g=0;g<term.length();g++)
                        {
                            if(strN1[i][k+1]==term[g])
                            {
                                list[h][g]=1;
                            }
                        }
                    }
                }
            }
        }
   }
   //cout<<"С�ڹ�ϵΪ��"<<endl;
   for(int i=0;i<num-1;i++)
   {
        for(int k=0;k<strN1[i].length();k++)
        {
            if(!Norterminal(strN1[i][k]))
            {
                if(Norterminal(strN1[i][k+1]))
                {
                    for(int h=0;h<term.length();h++)
                    {
                        if(strN1[i][k]==term[h])
                        {
                            for(int j=0;j<num-1;j++)
                            {
                                if(strN1[i][k+1]==gram[j].a[0])
                                {
                                    for(int g=0;g<term.length();g++)
                                    {
                                        for(int f=0;f<gram[j].first.length();f++)
                                        if(gram[j].first[f]==term[g])
                                        {
                                            list[h][g]=2;
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }


   }
   //���ڹ�ϵ
   for(int i=0;i<num-1;i++)
   {
        for(int k=0;k<strN1[i].length();k++)
        {
            if(Norterminal(strN1[i][k]))
            {
                if(!Norterminal(strN1[i][k+1]))
                {
                    for(int h=0;h<term.length();h++)
                    {
                        if(strN1[i][k+1]==term[h])
                            for(int j=0;j<num-1;j++)
                            {
                                if(strN1[i][k]==gram[j].a[0])
                                {
                                    for(int g=0;g<term.length();g++)
                                    {
                                        for(int f=0;f<gram[j].first.length();f++)
                                        if(gram[j].follow[f]==term[g])
                                        {
                                            list[g][h]=3;
                                        }
                                    }
                                }
                            }
                    }

                }
            }
        }
   }
    for(int i=0;i<term.length();i++)
    {
        for(int j=0;j<term.length();j++)
        {
            cout<<list[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"������Ⱦ���Ϊ��"<<endl;
    cout<<"    |";
    for(int i=0;i<term.length();i++)
    {
        cout<<setw(10)<<term[i];
        cout<<"|";
    }
    cout<<endl;
    cout<<"____________________________________________________________________________"<<endl;

    for(int i=0;i<term.length();i++)
    {
        printf("%2c",term[i]);
        cout<<"  "<<"|";
        for(int j=0;j<term.length();j++)
        {
            if(list[i][j]==0)
            cout<<setw(10)<<" "<<"|";
            if(list[i][j]==1)
            cout<<setw(10)<<"="<<"|";
            if(list[i][j]==2)
            cout<<setw(10)<<"<"<<"|";
            if(list[i][j]==3)
            cout<<setw(10)<<">"<<"|";

        }
        cout<<endl;
        cout<<"____________________________________________________________________________"<<endl;

    }
}
int array1[8][8]={	//���Ⱦ���1='>',-1='<',0='='
	{ 1, 1,-1,-1,-1, 1,-1, 1},
	{ 1, 1,-1,-1,-1, 1,-1, 1},
	{ 1, 1, 1, 1,-1, 1,-1, 1},
	{ 1, 1, 1, 1,-1, 1,-1, 1},
	{-1,-1,-1,-1,-1, 0,-1, 1},
	{ 1, 1, 1, 1,' ',1,' ',1},
	{ 1, 1, 1, 1,' ',1,' ',1},
	{-1,-1,-1,-1,-1,-1,-1, 0}
};


void init()
{
	char a1[200];
	int i=0,j=0;
	if((fp=fopen("input1.txt","r"))==NULL)
	{
		cout<<"�ļ���ʧ�ܣ�"<<endl;
		exit(0);
	}
	a1[i++]=fgetc(fp);
	while(!feof(fp))
	{
		a1[i++]=fgetc(fp);
		if((i%5)==0)
			a[j++]=a1[i-2];
	}
	fclose(fp);
	cout<<"�����ַ���Ϊ��";
	cout<<a<<endl;
}
void error()
{
	cout<<"������ȷ���ʧ��!"<<endl;
	exit(0);
}
int getVtNumber(char ch)//��ȡ�ս����ch�����
{
	char vt[8]={'+','-','*','/','(',')','i','#'};
	int i;
	for(i=0;i<8;i++)
		if(ch==vt[i])
			break;
	return i;
}
bool Higher(char ch1,char ch2)
{
	int c1,c2;
	c1=getVtNumber(ch1);
	c2=getVtNumber(ch2);
	if(array1[c1][c2]==1)
		return true;
	return false;
}
bool Lower(char ch1,char ch2)
{

	int c1,c2;
	c1=getVtNumber(ch1);
	c2=getVtNumber(ch2);
	if(array1[c1][c2]==-1)
		return true;
	return false;

}
bool Equal(char ch1,char ch2)
{
	int c1,c2;
	c1=getVtNumber(ch1);
	c2=getVtNumber(ch2);
	if(array1[c1][c2]==0)
		return true;
	return false;
}


//Ѱ���������ض���ƥ��Ĳ���ʽ
bool IsReduce(int begin,int end,int len)//begin,end,len�ֱ��ʾ�����ض���Ŀ�ͷ�ͽ���λ�ü������ض���ĳ���
{
	if(len==1)
		if(Stack[begin]=='i')
			return true;
	if(len==3)
		if(Stack[begin]=='(' && Stack[end]==')' && Norterminal(Stack[begin+1]))
			return true;
		else
			if(Norterminal(Stack[begin]))
				if(Norterminal(Stack[end]))
					if(Stack[begin+1]=='+' || Stack[begin+1]=='-' || Stack[begin+1]=='*' || Stack[begin+1]=='/')
						return true;
	return false;
}


//Ѱ�������ض�����ƹ�Լ���ƽ�
void parser(void)
{
	int i,k;
	char r;
	i=0;k=0;
	Stack[0]='#';
	do
	{
		int j;
		r=a[i++];
		if(!Norterminal(Stack[k]))
			j=k;
		else
			j=k-1;
		while(Higher(Stack[j],r))
		{
			char q;
			do
			{
				q=Stack[j];
				if(!Norterminal(Stack[j-1]))
					j--;
				else
					j-=2;
			}while(!Lower(Stack[j],q));
			if(IsReduce(j+1,k,k-j))
			{
				k=j+1;						//��Լ
				Stack[k]='N';
			}
			else
				error();
		}
		if(Lower(Stack[j],r) || Equal(Stack[j],r))
			Stack[++k]=r;					//�ƽ�
		else
			error();
	}while(r != '#');
	cout<<"������ȷ����ɹ�!"<<endl;

}

int main()
{
    analyze();
    init();
	parser();
}

