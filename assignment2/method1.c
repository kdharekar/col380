#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void crout(double **A, double **L, double **U, int n,int thread_count) {

    double sum = 0;
    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n; i++) {
        U[i][i] = 1;
    }
  //  for(int i=0;i<n;i++){
    //  for(int j=0;j<n;j++){
      //  printf("U[%d][%d] = %lf\n ",i,j,U[i][j]);
    //  }
    //}

    for (int j = 0; j < n; j++) {
      #pragma omp parallel for num_threads(thread_count) private(sum) schedule(static,1)
      for (int i = j; i < n; i++) {
          sum = 0;
          for (int k = 0; k < j; k++) {
              sum = sum + L[i][k] * U[k][j];
          }
          L[j][i]=0;
          L[i][j] = A[i][j] - sum;
          //printf("L[%d][%d] = %lf and sum = %lf\n",i,j,L[i][j],sum);
      }
      #pragma omp parallel for num_threads(thread_count) private(sum) schedule(static,1)
      for (int i = j; i < n; i++) {
          sum = 0;
          for(int k = 0; k < j; k++) {
              sum = sum + L[j][k] * U[k][i];
          }
          if (L[j][j] == 0) {
              exit(0);
          }
          U[i][j] = 0;
          U[j][i] = (A[j][i] - sum) / L[j][j];
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
  crout(A,L,U,n,thread_count);
  if(thread_count==1){
  write_output("output_L_1_1.txt",L,n);
  write_output("output_U_1_1.txt",U,n);
}
if(thread_count==2){
write_output("output_L_1_2.txt",L,n);
write_output("output_U_1_2.txt",U,n);
}
if(thread_count==4){
write_output("output_L_1_4.txt",L,n);
write_output("output_U_1_4.txt",U,n);
}
if(thread_count==8){
write_output("output_L_1_8.txt",L,n);
write_output("output_U_1_8.txt",U,n);
}
if(thread_count==16){
write_output("output_L_1_16.txt",L,n);
write_output("output_U_1_16.txt",U,n);
}
  for(int i=0;i<n;++i){
    free(U[i]);
      free(L[i]);
    free(A[i]);}
  free(A);
  free(L);
  free(U);
}
