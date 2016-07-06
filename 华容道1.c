/*-------------华容道优化版------------------------*/
/*------------时间：2016.7.6-----------------------*/
/*----------运行时间：3.3ms【只测试过“横刀立马”】--*/
/*----方法：将坐标压缩成unsign long--------------------
------------棋盘状态也压缩成另外的unsign long----------------*/ 
#include<stdio.h>
#include<memory.h>
#include<stdlib.h>
#define SSIZE 200000
#define N_PIECES 12
#define EMPTY_1 10
#define EMPTY_2 11
int tail=1,head=0,k=0;
typedef struct piece{
    short x,y;//location
    short w,h;//width & height
    char name[8];
}piece;
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
typedef struct pattern{
    unsigned long p1;
    unsigned long p2;
}pattern;
pattern patt[SSIZE];
pattern cur_pat;
//----对pos数组（表示棋子位置）进行编码压缩至state结构----//
void putpos(int n, pos *ps){
	int i;
	unsigned long m1 = 0, m2 = 0;

	for (i = 0; i < N_PIECES / 2; i++){
		m1 = (m1 << 5) | (ps[i].x << 3 | ps[i].y );
		m2 = (m2 << 5) |
			(ps[i + N_PIECES / 2].x << 3 | ps[ i + N_PIECES / 2].y );
	}

	queue[n].p1 = m1;
	queue[n].p2 = m2;
	//printf("%ld %ld\n",m1,m2);

}
//----节点解码得到表示棋子位置的pos数组----//
void getpos(int n , pos *ps){
	int i;
	unsigned long m1, m2;

	m1 = queue[n].p1;
	m2 = queue[n].p2;

	for (i = N_PIECES / 2 - 1; i >= 0; i--){
		ps[i].y = m1 & 07;
		ps[i].x = (m1 >> 3) & 03;
		m1 >>= 5;

		ps[i + N_PIECES / 2].y = m2 & 07;
		ps[i + N_PIECES / 2].x = (m2 >> 3) & 03;
		m2 >>= 5;
	}
//	printf("m1=%ld\n",m1);
//	printf("m2=%ld\n",m2);
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
int (*actions[ ])(int, pos *) = {go_left, go_right, go_up, go_down};
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
void getpatt(int n, pattern *patt){
	pos t[N_PIECES];
	unsigned long pat[2] = {0, 0};
	int i, id, p, off;

	getpos(n, t);
	for (i = 0; i < N_PIECES - 2; i++){
		p = pieces[i].w << 1 | pieces[i].h;
		id = t[i].x >> 1;
		off = t[i].y * 3 + ((t[i].x & 0x1) << 4);
		pat[id] |= p << off;
	}
	if(k<100)printf("%d %d\n",pat[0],pat[1]);
	k++;
	patt->p1 = pat[0];
	patt->p2 = pat[1];
}
//-------------查重----------------//
int exist(int n){
	int i;
	getpatt(n, &cur_pat);
	for (i = 0; i < tail; i++){
		if (cur_pat.p1 == patt[i].p1 && cur_pat.p2 == patt[i].p2)
			return 1;
	}
	return 0;
}
void insert(pattern *pt){  patt[tail]=*pt; }
//--------移动棋子即节点扩展--------------//
void move_piece(int n, int p, pos ps[]){
	int i;
	//buffer_check();  //检查queue[]是否有足够空间保存可能节点
	for (i = 0; i<4; i++){
		if (actions[i](p, ps) && !exist(tail)){
            insert(&cur_pat);             
			queue[tail++].parent = n;
		}
	}       
	return;
}
//--------判断曹操是否到了指定位置---//
int reached(pos ps[]){
    return (ps[0].x==1&&ps[0].y==0)?1:0;
}
//-------------打印结果--------------//
int show_res(int n)
{
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
int main(){
	pos p[N_PIECES];
	int i,j;
    for(i=0;i<12;i++){
        p[i].x=pieces[i].x;
        p[i].y=pieces[i].y;
      //  printf("%d %d\n",pieces[i].x,pieces[i].y);
    }
  //  print(p);
  // printf("\n");
    putpos(0,p);
    
    
    if(!search(200,p))printf("impossible");
    
    return 0;
}
