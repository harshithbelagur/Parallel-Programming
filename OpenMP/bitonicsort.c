//works only for powers of 2
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 33554432

//make global variables as upper case and parameters as lower case
float A[N];
int n;

void init(){
    int i;
    for(i=0;i<N;i++)
        A[i]=rand();
}
    
void merge(float a[N], int l, int count, int dir){
    if(count>1){
        int i,k;
        float t;
        k=count/2;
        for(i=l;i<l+k;i++){
            if(dir==(a[i]>a[i+k])){
                t=a[i+k];
                a[i+k]=a[i];
                a[i]=t;
            }
        }
        merge(a,l,k,dir);
        merge(a,l+k,k,dir);
    }
}

void split(float a[N], int l, int count, int dir){
    if(count>1){
        int k;
        k=count/2;
        #pragma omp parallel sections
        {
            #pragma omp section
                split(a,l,k,1);
            #pragma omp section
                split(a,l+k,k,0);
        }
        merge(a,l,count,dir);
    }
}

void display(float a[N], int n){
    for(int i=0;i<n;i++)
        printf("%f ",a[i]);
}

void sort(float a[N], int n){
    split(a,0,n,1);
}
              
int main(){
    n=33554432;
    double t1=time(NULL);
    init();
    double t2=time(NULL);
    sort(A,n);
    double t3=time(NULL);
    display(A,10000);
    printf("\nInitialization: %lf, Sort:%lf",t2-t1,t3-t2);
    return 0;
}

// Without Parallelization
// Initialization: 0.000000, Sort:29.000000%   

// Without Parallelization
// Initialization: 0.000000, Sort:18.000000%   

