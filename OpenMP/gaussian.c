// does not work when
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#define N 10000

// float X[N]={0.0,0.0,0.0}, B[N]={12.0,33.0,35.0}, Y[N];
int n;
float A[N][N], U[N][N];
float X[N], B[N], Y[N];

void init(){
    int i,j;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            A[i][j]=rand();
        }
        X[i]=rand();
        B[i]=rand();
    }
}

void gaussian(float a[N][N], float b[N], float u[N][N], float y[N], int n){
    int i,j,k;
    
    //initialize
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            u[i][j]=a[i][j];
        y[i]=b[i];
    }

    
    for(k=0;k<n-1;k++){
        //normalize every equation by the kth element
        for(i=k+1;i<n;i++)
            u[k][i]=u[k][i]/u[k][k];
        y[k]=y[k]/u[k][k];
        u[k][k]=1.0;
            
        //elimination
        #pragma omp parallel for shared(u,y) private(i,j,k)
            for(i=k+1;i<n;i++){
                for(j=k+1;j<n;j++)
                    u[i][j]=u[i][j]-u[i][k]*u[k][j];
                y[i]=y[i]-u[i][k]*y[k];
                u[i][k]=0.0;
            }
    }
}

void backsub(float u[N][N], float y[N], float x[N], int n){
    int i,j;
    for(i=n-1;i>=0;i--){
        x[i]=y[i]/u[i][i];
        #pragma omp parallel for shared(x,y) private(i,j)
            for(j=i-1;j>=0;j--)
                y[j]=y[j]-u[j][i]*x[i];
    }
}

int main(){
    n=10000;
    init();
    double t1=time(NULL);
    gaussian(A,B,U,Y,n);
    double t2=time(NULL);
    backsub(U,Y,X,n);
    double t3=time(NULL);
    for(int i=0;i<n;i++)
        printf("%f ",X[i]);
    printf("\nTime for Gaussian Elimination: %lf \nTime for Backsubstitution:%lf",t2-t1,t3-t2);
    return 0;
    
}

// Without Parallelization - 
// Time for Gaussian Elimination: 804.000000 
// Time for Backsubstitution:0.000000%  

// With Parallelization - 
// Time for Gaussian Elimination: 245.000000 
// Time for Backsubstitution:0.000000%      