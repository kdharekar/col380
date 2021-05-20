#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void crout1(double **A, double **L, double **U, int n,int thread_count) {
    int i, j, k;
    double sum = 0;
    for (i = 0; i < n; i++) {
        U[i][i] = 1;
    }

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            sum = 0;
            for (k = 0; k < j; k++) {
                sum = sum + L[i][k] * U[k][j];
            }
            L[j][i]=0;
            L[i][j] = A[i][j] - sum;
          //  printf("L[%d][%d] = %lf and sum = %lf\n",i,j,L[i][j],sum);
        }
        for (i = j; i < n; i++) {
            sum = 0;
            for(k = 0; k < j; k++) {
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0) {
                exit(0);
            }
            U[i][j]  = 0;
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}
void  method3L(double  **A, double **L, double **U,int i, int k1,int j1,int j,double *sum){
    //printf("rank = %d\n",omp_get_thread_num());
    //printf("threadL = %d\n",omp_get_num_threads());
    for (int k = k1; k < j1; k++) {
      #pragma omp critical
        *sum = *sum + L[i][k] * U[k][j];
    }
    //printf("L[%d][%d] = %lf and sum2 = %lf\n",i,j,L[i][j],*sum);
  }
void   method3U(double **A, double **L, double **U,int i, int k1,int j1,int j,double *sum){
      //printf("rank = %d\n",omp_get_thread_num());
      for(int k = k1; k < j1; k++) {
        #pragma omp critical
          *sum = *sum + L[j][k] * U[k][i];
      }
      //printf("U[%d][%d] = %lf and sum4 = %lf\n",j,i,U[j][i],*sum);
    }

void crout(double **A, double **L, double **U, int n,int thread_count) {
      #pragma omp parallel for num_threads(thread_count)
      for (int i = 0; i < n; i++) {
          U[i][i] = 1;
      }
      double sum1 = 0; double sum2 =0;
      double sum3 = 0; double sum4 =0;
    omp_set_num_threads(thread_count);
      for (int j = 0; j < n; j++) {
        #pragma omp parallel for num_threads(thread_count/2) private(sum1,sum2) schedule(static,1)
        for (int i = j; i < n; i++) {
            sum1 = 0; sum2 =0;
          //printf("threads = %d\n",omp_get_num_threads());
              #pragma omp parallel sections num_threads(2)
              {

              #pragma omp section
              method3L(A, L, U,i, 0,j/2,j,&sum1);

              #pragma omp section
              method3L(A, L, U,i, j/2,j,j,&sum2);
            }

            L[j][i]=0;
            L[i][j] = A[i][j] - sum1-sum2;
        //    printf("L[%d][%d] = %lf and sum1 = %f sum2 = %lf\n",i,j,L[i][j],sum1,sum2);
          }
        #pragma omp parallel for num_threads(thread_count/2) private(sum3,sum4)schedule(static,1)
        for (int i = j; i < n; i++) {
            sum3= 0;
            sum4 = 0;
            #pragma omp parallel sections num_threads(2)
            {
              #pragma omp section
              method3U(A, L, U,i, 0,j/2,j,&sum3);
              #pragma omp section
              method3U(A, L, U,i, j/2,j,j,&sum4);
            }
            if (L[j][j] == 0) {
                exit(0);
            }
            U[i][j] = 0;
            U[j][i] = (A[j][i]-sum3-sum4) / L[j][j];
          //  printf("U[%d][%d] = %lf and sum3 = %lf sum4 = %lf\n",j,i,U[j][i],sum3,sum4);
        }
      }
  }


void write_output(char fname[], double** arr, int n ){
    FILE *f = fopen(fname, "w");
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%0.12f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char* argv[])
{ int n = atoi(argv[1]);
char* file_name  = argv[2];
int thread_count  = atoi(argv[3]);
	double**  A=(double**)malloc(n*sizeof(double*));
//  printf("nested = %d\n",omp_get_nested());
  omp_set_nested(2);
  //printf("nested = %d\n",omp_get_nested());
	for(int i=0;i<n;++i){
		A[i]=(double*)malloc(n*sizeof(double));  }
    double**  L=(double**)malloc(n*sizeof(double*));
  	for(int i=0;i<n;++i){
  		L[i]=(double*)malloc(n*sizeof(double));  }
      double**  U=(double**)malloc(n*sizeof(double*));
    	for(int i=0;i<n;++i){
    		U[i]=(double*)malloc(n*sizeof(double));  }

	 FILE *file;

  file=fopen(file_name, "r");

 for(int i = 0; i < n; i++)
  {
      for(int j = 0; j < n; j++)
      {
  //Use lf format specifier, %c is for character
       if (!fscanf(file, "%lf", &A[i][j]))
           break;
      // mat[i][j] -= '0';
       //printf("%lf\n",A[i][j]); //Use lf format specifier, \n is for new line
      }

  }
  fclose(file);
  if(thread_count==1){
  crout1(A,L,U,n,thread_count);
}
else{
  crout(A,L,U,n,thread_count);
}
if(thread_count==1){
write_output("output_L_3_1.txt",L,n);
write_output("output_U_3_1.txt",U,n);
}
if(thread_count==2){
write_output("output_L_3_2.txt",L,n);
write_output("output_U_3_2.txt",U,n);
}
if(thread_count==4){
write_output("output_L_3_4.txt",L,n);
write_output("output_U_3_4.txt",U,n);
}
if(thread_count==8){
write_output("output_L_3_8.txt",L,n);
write_output("output_U_3_8.txt",U,n);
}
if(thread_count==16){
write_output("output_L_3_16.txt",L,n);
write_output("output_U_3_16.txt",U,n);
}
for(int i=0;i<n;++i){
  free(U[i]);
    free(L[i]);
  free(A[i]);}
free(A);
free(L);
free(U);
}
