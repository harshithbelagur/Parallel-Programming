#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define MAX_SIZE 1000000


void generate_list(float * x, int n) {
   int i,j,t;
   for (i = 0; i < n; i++)
     x[i] = i;
   for (i = 0; i < n; i++) {
     j = rand() % n;
     t = x[i];
     x[i] = x[j];
     x[j] = t;
   }
}

void print_list(float * x, int n) {
   int i;
   for (i = 0; i < n; i++) {
      printf("%f ",x[i]);
   } 
}

void merge(float * X, int n, float * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;

   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
      ti++; i++;
   }
      while (j<n) { /* finish up upper half */
         tmp[ti] = X[j];
         ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(int));

} // end of merge()

void mergesorted(float * X, int n, float * tmp)
{
   if (n < 2) return;

    //using this instead of sections to test the difference
   #pragma omp task firstprivate (X, n, tmp)
   mergesorted(X, n/2, tmp);

   #pragma omp task firstprivate (X, n, tmp)
   mergesorted(X+(n/2), n-(n/2), tmp);
 
   #pragma omp taskwait

    /* merge sorted halves into sorted list */
   merge(X, n, tmp);
}


int main()
{
   int n = 1000000;
   double start, stop;

   float data[MAX_SIZE], tmp[MAX_SIZE];
   double t1=time(NULL);
   generate_list(data, n);
   printf("List Before Sorting...\n");
   print_list(data, n);
   double t2=time(NULL);
   #pragma omp parallel
   {
      #pragma omp single
      mergesorted(data, n, tmp);
   }
   double t3=time(NULL);
   printf("\nList After Sorting...\n");
   print_list(data, n);
   printf("\nInitialization: %lf, Sort:%lf",t2-t1,t3-t2);
   return 0;
}

// Without Parallelization
// Initialization: 0.000000, Sort:12.000000%   

// With Parallelization
// Initialization: 0.000000, Sort:1.000000%