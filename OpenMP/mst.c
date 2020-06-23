//adopted idea from the internet since my own version doesn't seem to work

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>
#define INF 999999999
struct str{
	float val;
	int in;
};

void set1(struct str *s)
{
	s->val=INF;
	s->in=-1;
} 

float A[10000][10000],d[10000];

void djk(float a[][10000],int s,int n) 
{   
	bool set[n];
	int from[n];
	for(int i=0;i<n;i++)
	{ 
		d[i]=INF;
		set[i]=false; 
	}   
	d[s]=0;
	from[s]=s;  
	float c=0;
        for (int i=0;i<n;i++)
	{ 
		struct str s;
		set1(&s);
		#pragma omp declare reduction(minimum : struct str : omp_out=omp_in.val<omp_out.val?omp_in:omp_out) initializer(set1(&omp_priv))

		#pragma omp parallel for reduction(minimum:s)	
			for(int j=0;j<n;j++)
			{
				if(set[j]==false && d[j]<=s.val)
				{
					s.val=d[j];
					s.in=j;
				}
			}
		set[s.in]=true;
		c=c+s.val;
		if(s.in!=from[s.in])
		printf("%d %d \n",s.in+1,from[s.in]+1);
		#pragma omp for  
			for (int j=0;j<n;j++)
			{
				if(!set[j] && d[j]>a[s.in][j])
				{
					d[j]=a[s.in][j];
					from[j]=s.in; 
				}
			}
	 } 
	printf("Cost of spanning tree : %f\n",c);
} 

int main()
{
	clock_t t1,t2,t3;
	t1=clock();
	int n,m,u,v,s;
	printf("Enter number of vertices\n");
	scanf("%d",&n);
	printf("Enter number of edges\n");
	scanf("%d",&m);
	for(int i=0;i<n;i++)
	{	
		for(int j=0;j<n;j++)
		A[i][j]=(i==j)?0:INF;	
	}
	for(int i=1;i<=m;i++)
	{
		scanf("%d %d",&u,&v);
		scanf("%f",&A[u-1][v-1]);
		A[v-1][u-1]=A[u-1][v-1];
	}
	s=1;
	t2=clock();
	printf("Output\n");
	djk(A,s-1,n);
	t3=clock();
	double v1,v2;
	v1=(double)(t2-t1)/CLOCKS_PER_SEC;
	v2=(double)(t3-t2)/CLOCKS_PER_SEC;
	printf("Input time  : %0.9f\n",v1);
	printf("Computation time : %0.9f\n",v2);
}
/*
Enter number of vertices
9
Enter number of edges
14
1 2 4
1 8 8
2 3 8
2 8 11
8 9 7
8 7 1
3 9 2
9 7 6
3 6 4
7 6 2
4 6 14
4 5 9
6 5 10
3 4 7
Output
2 1 
3 2 
9 3 
6 3 
7 6 
8 7 
4 3 
5 4 
Cost of spanning tree : 37.000000
Input time  : 0.000150000
Computation time : 0.017689000
*/
