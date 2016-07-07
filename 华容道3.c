/*-------------华容道优化版------------------------*/
/*------------时间：2016.7.6----------------------*/
/*----方法：将坐标压缩成unsign long--------------
-----并存为对应的哈希数以查重，节省时间----------
----【其实用AVL也可，但是rotate()写起来比较麻烦--*/
#include<stdio.h>
#include<memory.h>
#include<stdlib.h>
#define SSIZE 30000
#define N_PIECES 12
#define EMPTY_1 10
#define EMPTY_2 11
long hash=29611;
int tail=1,head=0,k=0;
typedef struct piece{
    short x,y;//location
    short w,h;//width & height
    char name[8];
}piece;
typedef struct hashing{
	int tag;
	long p1;
	long p2;
	struct hashing *next;
}hashing;
hashing table[SSIZE];

piece pieces[]={
   1, 3, 2, 2, "曹操",
	1, 2, 2, 1, "关羽",
	0, 3, 1, 2, "张飞",
	0, 1, 1, 2, "赵云",
	3, 3, 1, 2, "马超",
	3, 1, 1, 2, "黄忠",
	0, 0, 1, 1, "兵 1",
	1, 1, 1, 1, "兵 2",
	2, 1, 1, 1, "兵 3",
	3, 0, 1, 1, "兵 4",
	1, 0, 1, 1, "空_1",
	2, 0, 1, 1, "空_2"

};
typedef struct pos{
    short x;
    short y;
}pos;

typedef struct state{
	unsigned long p1, p2;//10个棋子的x,y坐标压缩成32位整数
	long parent;
} state;
state queue[SSIZE];	 //状态空间存储队列
state code[SSIZE];
typedef struct pattern{
    unsigned long p1;
    unsigned long p2;
}pattern;
pattern patt[SSIZE];
pattern cur_pat;
//--------除留余数 && 链地址法---------//
void hashfun(long p1,long p2){
	hashing *p,*q;
	int tmp=(p1+p2)%hash;
	p=&table[tmp];
	if(!table[tmp].tag){
		table[tmp].tag=1;
		table[tmp].p1=p1;
		table[tmp].p2=p2;
		table[tmp].next=NULL;
	}
	else{
		while(p->next){
			p=p->next;
		}
		q=(hashing *)malloc(sizeof(hashing));
		q->tag=1;
		q->p1=p1;
		q->p2=p2;
		q->next=NULL;
		p->next=q;
	
	}
//	printf("%ld %ld %d\n",p1,p2,tmp);
	return;
}

//----对pos数组（表示棋子位置）进行编码压缩至state结构----//
void putpos(int n, pos *ps){
	int i;
	unsigned long m1 = 0, m2 = 0;

	for (i = 0; i < N_PIECES / 2; i++){
		m1 = (m1 << 5) | (ps[i].x << 3 | ps[i].y );
		m2 = (m2 << 5) |
			(ps[i + N_PIECES / 2].x << 3 | ps[ i + N_PIECES / 2].y );
	}

	code[n].p1 = m1;
	code[n].p2 = m2;
}
//----节点解码得到表示棋子位置的pos数组----//
void getpos(int n , pos *ps){
	int i;
	unsigned long m1, m2;

	m1 = code[n].p1;
	m2 = code[n].p2;

	for (i = N_PIECES / 2 - 1; i >= 0; i--){
		ps[i].y = m1 & 07;
		ps[i].x = (m1 >> 3) & 03;
		m1 >>= 5;

		ps[i + N_PIECES / 2].y = m2 & 07;
		ps[i + N_PIECES / 2].x = (m2 >> 3) & 03;
		m2 >>= 5;
	}

}
//----------将参数中的pos数组ps[]临时储存到一个t[]数组中------//
void copy(pos *ps,pos *tps){
    memcpy(tps,ps,sizeof(pos)*N_PIECES);
}
void print(pos *t){
	int i;
	for(i=0;i<12;i++){
		printf("%s [%d %d ] ",pieces[i].name,t[i].x,t[i].y);
		if((i+1)%3==0)printf("\n");
	}
	printf("\n");
	return;
}
int go_left(), go_right(), go_up(), go_down();
//------------棋子移动操作------------//
int (*actions[ ])(int, pos*) = {go_left, go_right, go_up, go_down};
//----------系列操作函数如下------------//
int go_left(int i, pos ps[]){
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].y == ps[d].y && (ps[i].x - ps[d].x) == 1){
			if (pieces[i].h == 1){
				copy(ps, t);
				t[i].x -= 1;
				t[d].x += pieces[i].w;
				putpos(tail, t);
				//print(t);
				return 1;
			}
			else if (ps[EMPTY_1].x == ps[EMPTY_2].x
			&& ps[EMPTY_1 + EMPTY_2 - d].y - ps[d].y == 1){
				copy(ps, t);
				t[i].x -= 1;
				t[EMPTY_1].x = t[EMPTY_2].x = ps[d].x + pieces[i].w;
				putpos(tail, t);
				//print(t);
				return 1;
			}
		}
	}
	return 0;
}
int go_right(int i, pos ps[]){
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if ((ps[i].y == ps[d].y )&& (ps[d].x-ps[i].x== pieces[i].w)){
			if (pieces[i].h == 1){
				copy(ps, t);
				t[i].x += 1;
				t[d].x -= pieces[i].w;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
			else if (ps[EMPTY_1].x == ps[EMPTY_2].x
            && ps[EMPTY_1 + EMPTY_2 - d].y - ps[d].y == 1){
				copy(ps, t);
				t[i].x += 1;
				t[EMPTY_1].x = t[EMPTY_2].x = ps[d].x - pieces[i].w;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
		}
	}
	return 0;
}
int go_up(int i, pos ps[]){
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].x == ps[d].x && (ps[d].y - ps[i].y) == pieces[i].h){
			if (pieces[i].w == 1){
				copy(ps, t);
				t[i].y += 1;
				t[d].y -= pieces[i].h;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
			else if (ps[EMPTY_1].y == ps[EMPTY_2].y
			&& ps[EMPTY_1 + EMPTY_2 - d].x - ps[d].x == 1){
				copy(ps, t);
				t[i].y += 1;
				t[EMPTY_1].y = t[EMPTY_2].y = ps[d].y - pieces[i].h;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
		}
	}
	return 0;
}
int go_down(int i, pos ps[]){
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].x == ps[d].x && (ps[i].y - ps[d].y) == 1){
			if (pieces[i].w == 1){
				copy(ps, t);
				t[i].y -= 1;
				t[d].y += pieces[i].h;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
			else if (ps[EMPTY_1].y == ps[EMPTY_2].y
			&& ps[EMPTY_1 + EMPTY_2 - d].x - ps[d].x == 1){
				copy(ps, t);
				t[i].y -= 1;
				t[EMPTY_1].y = t[EMPTY_2].y = ps[d].y + pieces[i].h;
				putpos(tail, t);
			//	print(t);
				return 1;
			}
		}
	}
	return 0;
}
//----根据棋盘局面生成盘面模式-------//
int getpatt(int n, pattern *patt){
	pos t[N_PIECES];
	unsigned long pat[2] = {0, 0};
	int i, id, p, off;

	getpos(n, t);
	if(n==0){
		//初始化 
		for (i = 0; i < N_PIECES - 2; i++){
		p = pieces[i].w << 1 | pieces[i].h;
		id = t[i].x >> 1;
		off = t[i].y * 3 + ((t[i].x & 0x1) << 4);
		pat[id] |= p << off;
		}
		patt->p1 = pat[0];
		patt->p2 = pat[1];
		
		queue[0].p1=pat[0];
		queue[0].p2=pat[1];
		
		hashfun(pat[0],pat[1]);
		return 1; 
	}
	for (i = 0; i < N_PIECES - 2; i++){
		p = pieces[i].w << 1 | pieces[i].h;
		id = t[i].x >> 1;
		off = t[i].y * 3 + ((t[i].x & 0x1) << 4);
		pat[id] |= p << off;
		}
	patt->p1 = pat[0];
	patt->p2 = pat[1];
	
	return (pat[0]+pat[1])%hash;
}
//-------------查重----------------//
int exist(int n){
	hashing *p;
	int tmp=getpatt(n, &cur_pat);//对生成的盘面格局使用hash 
	p=&table[tmp];
    if(p->tag){
         while(p){
            if((p->p1==cur_pat.p1)&&(p->p2==cur_pat.p2))
                return 1;
            p=p->next;
        }//看之前table中是否有相同的局面 
        return 0;
    }
    return 0;
}
void insert(pattern *pt){  patt[tail]=*pt; }
//--------移动棋子即节点扩展--------------//
void move_piece(int n, int p, pos ps[]){
	int i;
	for (i = 0; i<4; i++){
		if (actions[i](p, ps) && !exist(tail)){
			insert(&cur_pat);  
            hashfun(cur_pat.p1,cur_pat.p2);
    
			queue[tail].p1=cur_pat.p1;
			queue[tail].p2=cur_pat.p2;         
			queue[tail].parent = n;
			tail++;
		}
	}       
	return;
}
//--------判断曹操是否到了指定位置---//
int reached(pos ps[]){
    return (ps[0].x==1&&ps[0].y==0)?1:0;
}
//-------------打印结果--------------//
int show_res(int n){
    int i=n,length=0;
	while(queue[i].parent){
		length++;
		i=queue[i].parent;
	}//结束时path中共有f个元素，下标0到f-1 ,其中不包括初始棋盘
	printf("%d",length+1);
}
//---------搜索代码：原理是BFS--------//
int search(int max_step,pos *p){
    //max_step是最大搜索深度
	int i, j, end, level;
	for (level = i = 0; i < tail && level <= max_step; level++){//带深度控制的bfs
		for (end = tail; i < end; i++){
			getpos(i, p);
			if (reached(p)){
				show_res(i);
				return 1;
			}
			for (j = 0; j < 10; j++){
				move_piece(i, j, p);//节点扩展	
			}
		}
	}
	return 0;
}

void init(){
	int i=0;
	while(i<SSIZE){
		table[i].next=NULL;
		table[i].p1=table[i].p2=table[i].tag=0;
		i++;
	} 
	getpatt(0,&cur_pat);//初始化
	return;
}
int main(){
	pos p[N_PIECES];
	int i,j;
    for(i=0;i<12;i++){
        p[i].x=pieces[i].x;
        p[i].y=pieces[i].y;
      //  printf("%d %d\n",pieces[i].x,pieces[i].y);
    }
   // print(p);
  // printf("\n");
    putpos(0,p);
    init();
	
    if(!search(200,p))printf("impossible");

    return 0;
}
