#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 100000000

//make global variables as upper case and parameters as lower case
float A[N];
int n;

void init(){
    int i;
    for(i=0;i<N;i++)
        A[i]=rand();
}

void sort(float a[N], int l, int r){
    float t,x;
    int i,s;
    if(l>=r)
        return;
    x=a[r];
    s=l-1;
    for(i=l;i<=r-1;i++){
        if(a[i]<=x){
            s++;
            t=a[i];
            a[i]=a[s];
            a[s]=t;
        }
    }
    t=a[s+1];
    a[s+1]=a[r];
    a[r]=t;
    s++;
    
    //following 2 lines can be performed in parallel
    #pragma omp parallel sections
    {
        #pragma omp section
            sort(a,l,s-1);
        #pragma omp section
            sort(a,s+1,r);
    }
    
}

void display(float a[N], int n){
    for(int i=0;i<n;i++)
        printf("%f ",a[i]);
}

int main(){
    n=100000000;
    double t1=time(NULL);
    init();
    double t2=time(NULL);
    sort(A,0,n-1);
    double t3=time(NULL);
    display(A,10000);
    printf("\nInitialization: %lf, Sort:%lf",t2-t1,t3-t2);
    return 0;
}

// Without Parallelization
// Initialization: 1.000000, Sort:18.000000%

// With Parallelization
// Initialization: 1.000000, Sort:18.000000%