//前四个任务需要最后输出计数结果
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
typedef vector<string> S;
vector<string> v1;
vector<string> v2;
string::size_type position;
//--------找到包含所有关键字的行并输出-------//
int task1(string s1,S v1){
    vector<string>::iterator tmp=v1.begin();
    int cnt1=0,cnt2=0;
    while(tmp!=v1.end()){
        //cout<<*tmp;
        position=s1.find(*tmp);
        if(position!=s1.npos){
                cnt1++;
        }
        cnt2++;tmp++;
    }
	if(cnt1==cnt2&&cnt1!=0){
        cout<<s1<<endl;
        return 1;
	}
	return 0;
}
int main(int argc,char *argv[])
{
    char *ch=" -:#_";
    char *p;
    int flag=0;
   // system("cls");
    v1.push_back(strtok(argv[1],ch));
    while((p=strtok(NULL,ch))){
        v1.push_back(p);
    }
    //将每个用分隔符隔开的字符串依次放入v1中
    ifstream in;
    string s1,s2;
    in.open(argv[2]);
    if(!in){
    	cout<<"error"<<endl;
    	return -1;
	}
	while(getline(in,s1)){
		//按行读取
        if(task1(s1,v1)){
            flag++;
        }
	}
    in.close();
    if(!flag)cout<<"No exist"<<endl;

	return 0;
}
