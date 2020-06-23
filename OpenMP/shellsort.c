#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 10000000

//make global variables as upper case and parameters as lower case
float A[N];
int n;

void init(){
    int i;
    for(i=0;i<N;i++)
        A[i]=rand();
}

void InsertSort(float arr[], int i, int n,  int half){
	float temp = 0;
	int j = 0;
	for (int f = half + i; f < n; f = f + half)
	{
		j = f;
		while(j > i && arr[j-half] > arr[j])
		{
			temp = arr[j];
			arr[j] = arr[j-half];
			arr[j-half] = temp;
			j = j-half;
		}
	}
}

void sort(float a[N], int n){
    int h;
	int j = 0;
	float temp = 0;
	int i = 0;
	for(h =n/2;h>0;h=h/2){
		#pragma omp parallel for shared(a,n,h)
            for(i=0;i<h;i++)
                InsertSort(a, i, n, h); //calls insertion sort for elements at h-sized intervals
	}
}

void display(float a[N], int n){
    for(int i=0;i<n;i++)
        printf("%f ",a[i]);
}

int main(){
    n=10000000;
    double t1=time(NULL);
    init();
    double t2=time(NULL);
    sort(A,n);
    double t3=time(NULL);
    display(A,1000);
    printf("\nInitialization: %lf, Sort:%lf",t2-t1,t3-t2);
    return 0;
}

// Without Parallelization
// Initialization: 0.000000, Sort:5.000000%   

// With Parallelization
// Initialization: 0.000000, Sort:1.000000%
