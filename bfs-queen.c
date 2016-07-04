#include<stdio.h>  
#include <iostream>    
using namespace std;    
    
#define MAX_N 8    
#define MAX_NODES 5000      
typedef struct state_t{    
    short n, q[MAX_N];     
} state_t;    
    
state_t queue[MAX_NODES];    
int hd = 0, tl = 0;    
    
#define put_node(st) queue[tl++] = st    
#define get_node() queue[hd++]    
#define queue_not_empty (hd < tl)    
    
int sp = 0;    
int queens[MAX_N];    
int conflict(int q, int p, short queens[]);    
int queen(int n);    
void print_queens(state_t *st);    
    
int main( )    
{    
	int n;
    state_t init_st;    
    init_st.n = 0;       
    put_node(init_st);  
    scanf("%d",&n);
    queen(n);    
    return 0;    
}    
void print_queens(state_t *st)      //????    
{    
    int i;    
    for (i = 0; i < st->n; i++)  {    
        printf("%d ", st->q[i]);    
    }    
    printf("\n");    
}    
int queen(int n)              
{    
    int i, nCount=0;    
    state_t st;    
    
    st = get_node(); 
      	//printf("\n%d: ",st.n); 
        //st.n++;  
       for(i=0; i<n; i++){ 
            if (conflict(st.n, i, st.q))      
               	continue;    
           else{  
                st.q[st.n] = i;  
               // printf("%d ",i);   
                put_node(st);     
            	}        
       	}  
    while(queue_not_empty){    
		for(int j=0;queue_not_empty;j++){
			st = get_node(); 
			st.n++;
      		//printf("\n%d: ",st.n); 
       		if (st.n>=n){    
           // print_queens(&st);    
            nCount ++; 
            continue;    
        	}
        	for(i=0; i<n; i++){ 
            	if (conflict(st.n, i, st.q))      
                	continue;    
            	else{  
                	st.q[st.n] = i;  
                //	printf("%d ",i);   
                	put_node(st);     
            	}        
       		}
		}  
    }    
    printf("%d\n", nCount);   
    return 0;    
}    
 
int conflict(int q, int p, short queens[])    
{    
    int k;    
    for (k = 0; k < q; k++){    
        if (queens[k] == p)             
            return 1;    
        if (queens[k] - p == q - k)  //dx = -dy;   
            return 1;    
        if (queens[k] - p == k- q)  //dx = dy;   
            return 1;    
    }    
    return 0;    
}    
