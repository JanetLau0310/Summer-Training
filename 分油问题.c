#include<stdio.h> 
#define MAXSTATE 10000
int tail=1,hd=0, target,count=0;			//队列尾端，B罐目标油量
#define put_node(i) queue[tail++] = i
#define get_node() queue[hd++]    
#define queue_not_empty (hd < tail)    
   
/*---idea-- 用BFS，列举出每一个状态，求最优解----------
------实际上这种题用不定方程解，依次枚举是最简单的-----*/ 
/*---难点--要去重作比较---*/ 
//油罐
typedef struct Jar{
	int cap;		//油罐的容量
	int val;		//油罐当前的存油量
	int id;			//油罐标识0或者1，分别表示A和B
} Jar;
//状态
typedef struct State{
	int ca, cb;		//油罐A,B当前的储油量
	int parent;		//本节点的父亲节点在数组state[]中的下标
	char *act;		//生成本节点的操作名称
} State;
Jar Ja, Jb;				//分别表示A罐和B罐

State queue[MAXSTATE];		//最大状态数是(Ca+1)*(Cb+1)
State ans[MAXSTATE];


char *name[ ] = {"Fill A", "Fill B","Empty A", "Empty B","A->B", "B->A"};	//操作名称数组

int fill_a(), fill_b(), empty_a(), empty_b(), a_to_b(), b_to_a();	//定义判断函数
int (*action[])() = { fill_a, fill_b, empty_a, empty_b, a_to_b, b_to_a };	//指针数组，指向操作函数
//--------------判断去重--------------// 
int exist(int j){
	int i;
	for (i = 0; i < tail; i++){
		if (queue[i].ca == Ja.val && queue[i].cb == Jb.val)
			return 1;
	}
	return 0;
}
//------------查找是否成功-----------// 
int succeed(int i){
	if(queue[i].cb==target)return 1;
	return 0;
}
//---------各个操作------------// 
int fill_a(){
	if(Ja.val==Ja.cap)return 0;
   	else Ja.val=Ja.cap;
	return 1;
 } 
int fill_b(){
	if(Jb.val==Jb.cap)return 0;
	else Jb.val=Jb.cap;
    return 1;
}
int empty_a(){
	if(Ja.val==0)return 0;
 	else{Ja.val=0;}
	return 1; 
}
int empty_b(){
	if(Jb.val==0)return 0;
    else Jb.val=0;
    return 1; 
}
int a_to_b(){
	if(Ja.val+Jb.val>Jb.cap){
		Ja.val-=(Jb.cap-Jb.val);
		Jb.val=Jb.cap;
	}
	else{
		Jb.val+=Ja.val;
		Ja.val=0;
	}
    return 1;
}
int b_to_a(){
	if(Ja.val+Jb.val>Ja.cap){
		Jb.val-=(Ja.cap-Ja.val);
		Ja.val=Ja.cap;
	}
	else{
		Ja.val+=Jb.val;
		Jb.val=0;
	}
    return 1;
}
//----------打印结果---------// 
void show(int i){
	if(queue[i].parent==-1){
	//	printf("%d %d",queue[i].ca,queue[i].cb);
		printf("%d",count);
		return;
	}
	 while(queue[i].parent!=-1){
	 	//printf("%d %d %s",queue[i].ca,queue[i].cb,queue[i].act);
	 	i=queue[i].parent;
	 	count++;
	 }	
	 printf("%d",count);
	 return;
 }
 //-------------核心代码--------------// 
int solve()
{
	int i, j;
	queue[0].ca=Ja.val;
    queue[0].cb=Jb.val;   
	queue[0].parent=-1;
	for (i = 0; i<tail; i++){
		if (succeed(i)){	//找到目标状态
			show(i);	//递归打印出达到目标状态的操作组序列
			return 1;
		}
		for (j = 0; j<6; j++){
			Ja.val=queue[i].ca;
            Jb.val=queue[i].cb;//将油罐油量设置为节点i的状态            
			if (action[j]() && !exist(i)) {//执行j判断并检查是否重复
				ans[i].parent=i;	
				ans[i].ca=Ja.val;	ans[i].cb=Jb.val; 
                ans[i].act=name[j];   
				put_node(ans[i]); //j操作后的状态加入队列中
			}    
		}
	}
	return 0;
}
int main()
{
	int a,b;
	scanf("%d%d%d",&a,&b,&target);
	Ja.cap=a;Ja.val=0;Ja.id=0;
	Jb.cap=b;Jb.val=0;Jb.id=1;

	if(!solve())printf("impossible");
	return 0;
 } 
