/*-------------华容道优化版------------------------*/
/*------------创作人：梁晓周----------------------*/ 
/*------------时间：2016.7.6----------------------*/
/*------------方法：搜索二叉树--------------------*/ 
#include<stdio.h>
#include<stdlib.h>
#define SSIZE 50000
#define INCREMENT 4
#define N_PIECES 12
#define EMPTY_1 10
#define EMPTY_2 11
#define newnode ((tnode*)malloc(sizeof(tnode)))
//棋盘为5*4的 数组下标[0-4][0-3] 
typedef struct piece{
	short x, y;   //棋子的位置(左下角）
	short w, h;       //棋子的宽度和高度
	char name[8];	//棋子的名称
} piece;
piece pieces[] = {
	1, 3, 2, 2, "曹操",
	1, 2, 2, 1, "赵云",
	0, 3, 1, 2, "马超",
	0, 1, 1, 2, "张飞",
	3, 3, 1, 2, "黄忠",
	3, 1, 1, 2, "关羽",
	0, 0, 1, 1, "兵 1",
	1, 1, 1, 1, "兵 2",
	2, 1, 1, 1, "兵 3",
	3, 0, 1, 1, "兵 4",
	1, 0, 1, 1, "空_1",
	2, 0, 1, 1, "空_2"
};

typedef struct pos {
	short x;
	short y;
} pos;
typedef struct state {
	unsigned long p1, p2;
	long parent;
} state;
typedef struct pattern {
	unsigned long p1, p2;
} pattern;
typedef struct tnode{
	long p1,p2;
	struct tnode *left,*right;
}tnode;
tnode *ptree; 
int queue_size=SSIZE;
state *queue=(state*)malloc(sizeof(state)*queue_size);//状态空间存储队列 
int k=0;
//对pos数组（表示棋子位置）进行编码
void putpos(int n, pos *ps)
{
	int i;
	unsigned long m1 = 0, m2 = 0;
	for (i = 0; i < N_PIECES / 2; i++){
		m1 = (m1 << 5) | (ps[i].x << 3 | ps[i].y );
		m2 = (m2 << 5) | 
			(ps[i + N_PIECES / 2].x << 3 | ps[ i + N_PIECES / 2].y );
	}

	queue[n].p1 = m1;
	queue[n].p2 = m2;
}
//节点解码得到表示棋子位置的pos数组
void getpos(int n , pos *ps)
{
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
}
int reached(pos *p)
{
	if(p[0].x==1&&p[0].y==0)
	return 1;
	else
	return 0; 
}
int show_res(int n)
{
    int i,j,f=1,k=n,path[1000];
	//还原路径 
	path[0]=n;
	while(queue[k].parent!=0)
	{
		k=queue[k].parent;
		path[f++]=k;
	}//结束时path中共有f个元素，下标0到f-1 ,其中不包括初始棋盘 
	printf("%d",f);
}
int tail=0;
int head=0;
void print(pos *t){
	int i;
	for(i=0;i<12;i++){
		printf("%s [%d %d ] ",pieces[i].name,t[i].x,t[i].y);
		if((i+1)%3==0)printf("\n");
	}
	printf("\n");
	return;
}
//copy
void copy(pos*p1,pos*p2)
{
	int i;
	for(i=0;i<N_PIECES;i++)
	{
		p2[i]=p1[i];
	}
} 
//板块移动函数 
int go_left(int i, pos ps[])
{
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].y == ps[d].y && (ps[i].x - ps[d].x) == 1){
			if (pieces[i].h == 1){
				copy(ps, t);
				t[i].x -= 1;
				t[d].x += pieces[i].w;
				putpos(tail, t);
				return 1;
			}
			else if (ps[EMPTY_1].x == ps[EMPTY_2].x && ps[d].y - ps[EMPTY_1 + EMPTY_2 - d].y == -1){
				copy(ps, t);
				t[i].x -= 1;
				t[EMPTY_1].x = t[EMPTY_2].x = ps[d].x + pieces[i].w;
				putpos(tail, t);
				return 1;
			}
		}
	}
	return 0;
}
int go_right(int i, pos ps[])
{
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].y == ps[d].y && (ps[i].x - ps[d].x) == -pieces[i].w){
			if (pieces[i].h == 1){
				copy(ps, t);
				t[i].x += 1;
				t[d].x -= pieces[i].w;
				putpos(tail, t);
				return 1;
			}
			else if (ps[EMPTY_1].x == ps[EMPTY_2].x && ps[EMPTY_1 + EMPTY_2 - d].y - ps[d].y == 1){
				copy(ps, t);
				t[i].x += 1;
				t[EMPTY_1].x = t[EMPTY_2].x = ps[d].x - pieces[i].w;
				putpos(tail, t);
				return 1;
			}
		}
	}
	return 0;
}
int go_up(int i, pos ps[])
{
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].x == ps[d].x && (ps[i].y - ps[d].y) == -pieces[i].h){
			if (pieces[i].w == 1){
				copy(ps, t);
				t[i].y += 1;
				t[d].y -= pieces[i].h;
				putpos(tail, t);
				return 1;
			}
			else if (ps[EMPTY_1].y == ps[EMPTY_2].y && ps[EMPTY_1 + EMPTY_2 - d].x - ps[d].x == 1){
				copy(ps, t);
				t[i].y += 1;
				t[EMPTY_1].y = t[EMPTY_2].y = ps[d].y - pieces[i].h;
				putpos(tail, t);
				return 1;
			}
		}
	}
	return 0;
}
int go_down(int i, pos ps[])
{
	int d;
	pos t[N_PIECES];

	for (d = EMPTY_1; d <= EMPTY_2; d++){
		if (ps[i].x == ps[d].x && (ps[i].y - ps[d].y) == 1){
			if (pieces[i].w == 1){
				copy(ps, t);
				t[i].y -= 1;
				t[d].y += pieces[i].h;
				putpos(tail, t);
				return 1;
			}
			else if (ps[EMPTY_1].y == ps[EMPTY_2].y && ps[EMPTY_1 + EMPTY_2 - d].x - ps[d].x == 1){
				copy(ps, t);
				t[i].y -= 1;
				t[EMPTY_1].y = t[EMPTY_2].y = ps[d].y + pieces[i].h;
				putpos(tail, t);
				return 1;
			}
		}
	}
	return 0;
}
int (*actions[ ])(int, pos *) = {
	go_left, go_right, go_up, go_down
};
//buffer_check
void buffer_check()
{
	if(tail>=SSIZE)
	{
		queue_size+=INCREMENT; 
		queue=(state*)realloc(queue,sizeof(state)*queue_size);
	}
	
}
void getpatt(int n, pattern *patt1)
{
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
	patt1->p1 = pat[0];
	patt1->p2 = pat[1];
}
//pattern patt[SSIZE], cur_pat;
int comp(pattern *patt2,tnode *tree)
{
	if(patt2->p1>tree->p1)
	return 1;
	if(patt2->p1<tree->p1)
	return -1;
	return patt2->p2-tree->p2;
}
pattern cur_pat;
int exist(int n)
{
	int found=0,cmp;
	tnode *t=ptree;
	getpatt(n,&cur_pat);
	while(t!=NULL&&!found)
	{
		cmp=comp(&cur_pat,t);
		if(cmp==0)
		found=1;
		else if(cmp<0)
		t=t->left;
		else
		t=t->right;
	}
	return found;
}
void insert(pattern *x)
{
	int cmp;
	tnode **tp;
	tp=&ptree;
	while(*tp!=NULL)
	{
		cmp=comp(x,*tp);
		if(cmp<0)
		  tp=&((*tp)->left);
		else if(cmp>0)
		  tp=&((*tp)->right);
	}
	*tp=newnode;
	if(*tp==NULL)
	{
		printf("OUT OF MEMORY FOR NEW NODE!");
		return;
	}
	(*tp)->p1=x->p1;
	(*tp)->p2=x->p2;
	(*tp)->left=(*tp)->right=NULL;
}
//移动棋子即节点扩展
void move_piece(int p_node, int p, pos ps[ ])
{
	int i;

	buffer_check();  //检查queue[ ]是否有足够空间保存可能节点
	for (i = 0; i <  4; i++){
		if (actions[i](p, ps) && !exist(tail)){
			insert(&cur_pat);
			queue[tail++].parent = p_node;
		}
	}
}

int search(int max_step,pos *p) //这里设为1000 
{
	int i, j, end, level;
	for (level = i = 0; i < tail && level <= max_step; level++){
		for (end = tail; i < end; i++){
			getpos(i, p);
			if (reached(p)){
				show_res(i);
				return 1;
			}
			for (j = 0; j < 10; j++)
				move_piece(i, j, p);		//节点扩展
		}
	}
	return 0;
}

int main()
{
	/*int a[6][5],b[12][4][2],c[12],i,j,u=10;//a用于存储读入的数据 b用于存储各个板块的像素位置 c用于记录某板块的第几个像素 x、y轴方向分别向右和向上 
	for(i=0;i<=11;i++)
	c[i]=0;
	for(i=0;i<=4;i++)
	{
		for(j=0;j<=3;j++)
		{
			scanf("%d",&a[i][j]);
			if(a[i][j]>0)//非空白块 
			a[i][j]--;//对应上各板块的下标0-9
			else //空白块下标分别对应10、11 
			{
				a[i][j]=u;
				u++;
			}		 
		}
	}
	for(i=0;i<=4;i++)
	{
		for(j=0;j<=3;j++)
		{
			b[a[i][j]][c[a[i][j]]][0]=j;
			b[a[i][j]][c[a[i][j]]][1]=4-i;
			c[a[i][j]]++;
		}
	}
	//0曹操
	pieces[0].h=2;
	pieces[0].w=2;
	pieces[0].x=b[0][2][0]; 
	pieces[0].y=b[0][2][1];
	//1-5大将  
	for(i=1;i<=5;i++)
	{
		if(b[i][1][1]-b[i][0][1]==-1)//竖版
		{
			pieces[i].h=2;
			pieces[i].w=1;
			pieces[i].x=b[i][1][0];
			pieces[i].y=b[i][1][1];
		} 
		else //横板
		{
			pieces[i].h=1;
			pieces[i].w=2;
			pieces[i].x=b[i][0][0];
			pieces[i].y=b[i][0][1];
		} 
	}
	//6-9小兵 10-11空白
	for(i=6;i<=11;i++)
	{
		pieces[i].h=1;
		pieces[i].w=1;
		pieces[i].x=b[i][0][0];
		pieces[i].y=b[i][0][1];
	} 
	//初始棋盘入队
	pos temp[12];
	for(i=0;i<=11;i++)
	{
		temp[i].x=pieces[i].x;
		temp[i].y=pieces[i].y;
	}
	putpos(0,temp);
	queue[0].parent=-1; 
	tail++;
	//search
	if(reached(temp))
	printf("0");
	else
	{
		int v;
	    v=search(1000); 
	    /*
	    for(i=0;i<=110;i++)
	    {
	    	printf("queue[%d]:%d %d\n",i,queue[i].p1,queue[i].p2);
		}
		
	    if(v==0)
	    printf("It can't be solved in 1000 steps!");
	}*/
	pos p[N_PIECES];
	int i,j;
    for(i=0;i<12;i++){
        p[i].x=pieces[i].x;
        p[i].y=pieces[i].y;
      //  printf("%d %d\n",pieces[i].x,pieces[i].y);
    }
   // print(p);
    //printf("\n");
    putpos(0,p);
    
	tail++;
    if(!search(200,p))printf("impossible");
    
    return 0;
	
}
			
