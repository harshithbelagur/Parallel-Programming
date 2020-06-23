#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 2000

int global[100];
#pragma omp threadprivate(global)

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
    #pragma omp parallel for shared(a,b,C) private(i,j,k) num_threads(50) if(6>5) schedule(dynamic,50)
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
    n=2000;
    init();
    double t2=time(NULL);
    //mat_mat(A,B,n);
    double t4=time(NULL);
    printf("Matrix-Matrix: %lf\n",t4-t2);
    printf("%d\n",omp_get_dynamic());
    printf("PATH : %s\n", getenv("OMP_ATV_THREADS"));
    return 0;
}

// Without Parallelization
// Initialization: 0.000000, Matrix-Vector: 0.000000, Matrix-Matrix:120.000000

// With Parallelization
// Initialization: 0.000000, Matrix-Vector: 0.000000, Matrix-Matrix:13.000000