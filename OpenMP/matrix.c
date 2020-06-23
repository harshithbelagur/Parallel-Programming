#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 2500

//make global variables as upper case and parameters as lower case
float A[N][N], B[N][N], C[N][N];
float X[N], Y[N];
int n;

void init(){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            A[i][j]=rand();
            B[i][j]=rand();
        }
        X[i]=rand();
    }
}

void mat_vect(float a[N][N], float x[N], int n){
    int i,j;
    for(i=0;i<n;i++){
        Y[i]=0.0;
        for(j=0;j<n;j++)
            Y[i]+=a[i][j]*x[j];
    }
}

void mat_mat(float a[N][N], float b[N][N], int n){

    int i,j,k;

    //using combined directive
    #pragma omp parallel for shared(a,b,C) private(i,j,k)
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                C[i][j]=0.0;
                for(k=0;k<n;k++){
                    C[i][j]+=a[i][k]*b[k][j];
                }
            }
        }
}

void display_mat(float a[N][N], int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            printf("%f ",a[i][j]);
        printf("\n");
    }
}

void display_vect(float a[N], int n){
    int i,j;
    for(i=0;i<n;i++){
        printf("%f",a[i]);
        printf("\n");
    }
}

int main(){
    n=2500;
    double t1=time(NULL);
    init();
    double t2=time(NULL);
    mat_vect(A,X,n);
    double t3=time(NULL);
    mat_mat(A,B,n);
    double t4=time(NULL);
    printf("Initialization: %lf, Matrix-Vector: %lf, Matrix-Matrix:%lf\n",t2-t1,t3-t2,t4-t3);
    printf("%lf\n",t4-t1);
    return 0;
}

// Without Parallelization
// Initialization: 0.000000, Matrix-Vector: 0.000000, Matrix-Matrix:120.000000

// With Parallelization
// Initialization: 0.000000, Matrix-Vector: 0.000000, Matrix-Matrix:13.000000