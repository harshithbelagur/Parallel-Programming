#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 100000

//make global variables as upper case and parameters as lower case
float A[N];
int n;

void init(){
    int i;
    for(i=0;i<N;i++)
        A[i]=rand();
}

void sort(float a[N], int n){
    int i,j;
    float t;
    //outerloop is sequential but internal can be made parallel
    for(i=0;i<n;i++){
        if(i%2==0){
            #pragma omp parallel for private(t)
                for(j=0;j<n/2;j++){
                    if(a[2*j]>a[2*j+1]){
                        t=a[2*j+1];
                        a[2*j+1]=a[2*j];
                        a[2*j]=t;
                    }
                }
        }
        else{
            #pragma omp parallel for private(t)
                for(j=1;j<n/2;j++){
                    if(a[2*j-1]>a[2*j]){
                        t=a[2*j-1];
                        a[2*j-1]=a[2*j];
                        a[2*j]=t;
                    }
                }
        }
    }
}

void display(float a[N], int n){
    for(int i=0;i<n;i++)
        printf("%f ",a[i]);
}

int main(){
    n=100000;
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
// Initialization: 0.000000, Sort:26.000000%   

// Without Parallelization
// Initialization: 0.000000, Sort:4.000000%
