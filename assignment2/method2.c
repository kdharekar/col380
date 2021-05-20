#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void method2U(double **A, double **L, double **U, int n,int j,int dist,int val){
  if(val != 15){
    double sum = 0;
  for (int i = j+val*dist; i < j+(val+1)*dist; i++) {
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
  else{
      double sum = 0;
    for (int i = j+15*dist; i < n; i++) {
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
void method2L(double **A, double **L, double **U, int n,int j,int dist,int val){
  int tid= omp_get_thread_num();

  if(val != 15){
  //  printf("pid1  = %d dist = %d val = %d j = %d\n",tid,dist,val,j);
    double sum1 = 0;
  for (int i = j+val*dist; i < j+(val+1)*dist; i++) {
      sum1 = 0;
      for (int k = 0; k < j; k++) {
    //    printf("pid1 : %d val = %d L[%d][%d] = %lf and U[%d][%d] = %lf\n",tid,val,i,k,L[i][k],k,j,U[k][j]);
          sum1 = sum1 + L[i][k] * U[k][j];
      }
      L[j][i]=0;
      L[i][j] = A[i][j] - sum1;
      //printf("pid1 : %d L[%d][%d] = %lf and sum = %lf\n",tid,i,j,L[i][j],sum1);
  }
}
else{
  //printf("pid2  = %d dist = %d val = %d j = %d\n",tid,dist,val,j);
  double sum1 =0;
  for (int i = j+15*dist; i < n; i++) {
    sum1 = 0;
      for (int k = 0; k < j; k++) {
    //  printf("pid2 : %d val = %d L[%d][%d] = %lf and U[%d][%d] = %lf\n",tid,val,i,k,L[i][k],k,j,U[k][j]);
          sum1 = sum1 + L[i][k] * U[k][j];
      }
      L[j][i]=0;
      L[i][j] = A[i][j] - sum1;

    //  printf("L[0][0] = %lf L[0][1] = %lf L[0][2] = %lf L[0][3] = %lf\n",L[0][0],L[0][1],L[0][2],L[0][3]);
      //printf("L[1][0] = %lf L[1][1] = %lf L[1][2] = %lf L[1][3] = %lf\n",L[1][0],L[1][1],L[1][2],L[1][3]);
      //printf("L[2][0] = %lf L[2][1] = %lf L[2][2] = %lf L[2][3] = %lf\n",L[2][0],L[2][1],L[2][2],L[2][3]);
      //printf("L[3][0] = %lf L[3][1] = %lf L[3][2] = %lf L[3][3] = %lf\n",L[3][0],L[3][1],L[3][2],L[3][3]);

      //printf("pid2 : %d L[%d][%d] = %lf and sum1 = %lf\n",tid,i,j,L[i][j],sum1);
  }

}
}
void crout(double  **A, double **L, double **U, int n,int thread_count) {

  //  double sum = 0;
  //  #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n; i++) {
        U[i][i] = 1;
    }
    ;
    #pragma omp parallel num_threads(thread_count)
    for (int j = 0; j < n; j++) {
        int dist = (n-j)/16;
        int k = j;
      #pragma omp sections
       {
        #pragma omp section
        method2L(A, L, U, n,k,dist,0);
        #pragma omp section
        method2L(A, L, U, n,k,dist,1);
        #pragma omp section
        method2L(A, L, U, n,k,dist,2);
        #pragma omp section
        method2L(A, L, U, n,k,dist,3);
        #pragma omp section
        method2L(A, L, U, n,k,dist,4);
        #pragma omp section
        method2L(A, L, U, n,k,dist,5);
        #pragma omp section
        method2L(A, L, U, n,k,dist,6);
        #pragma omp section
        method2L(A, L, U, n,k,dist,7);
        #pragma omp section
        method2L(A, L, U, n,k,dist,8);
        #pragma omp section
        method2L(A, L, U, n,k,dist,9);
        #pragma omp section
        method2L(A, L, U, n,k,dist,10);
        #pragma omp section
        method2L(A, L, U, n,k,dist,11);
        #pragma omp section
        method2L(A, L, U, n,k,dist,12);
        #pragma omp section
        method2L(A, L, U, n,k,dist,13);
        #pragma omp section
        method2L(A, L, U, n,k,dist,14);
        #pragma omp section
        method2L(A, L, U, n,k,dist,15);
   }
    #pragma omp sections
        {
      #pragma omp section
      method2U(A, L, U, n,j,dist,0);
      #pragma omp section
      method2U(A, L, U, n,j,dist,1);
      #pragma omp section
      method2U(A, L, U, n,j,dist,2);
      #pragma omp section
      method2U(A, L, U, n,j,dist,3);
      #pragma omp section
      method2U(A, L, U, n,j,dist,4);
      #pragma omp section
      method2U(A, L, U, n,j,dist,5);
      #pragma omp section
      method2U(A, L, U, n,j,dist,6);
      #pragma omp section
      method2U(A, L, U, n,j,dist,7);
      #pragma omp section
      method2U(A, L, U, n,j,dist,8);
      #pragma omp section
      method2U(A, L, U, n,j,dist,9);
      #pragma omp section
      method2U(A, L, U, n,j,dist,10);
      #pragma omp section
      method2U(A, L, U, n,j,dist,11);
      #pragma omp section
      method2U(A, L, U, n,j,dist,12);
      #pragma omp section
      method2U(A, L, U, n,j,dist,13);
      #pragma omp section
      method2U(A, L, U, n,j,dist,14);
      #pragma omp section
      method2U(A, L, U, n,j,dist,15);
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
{int n = atoi(argv[1]);
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
  write_output("output_L_2_1.txt",L,n);
  write_output("output_U_2_1.txt",U,n);
}
if(thread_count==2){
write_output("output_L_2_2.txt",L,n);
write_output("output_U_2_2.txt",U,n);
}
if(thread_count==4){
write_output("output_L_2_4.txt",L,n);
write_output("output_U_2_4.txt",U,n);
}
if(thread_count==8){
write_output("output_L_2_8.txt",L,n);
write_output("output_U_2_8.txt",U,n);
}
if(thread_count==16){
write_output("output_L_2_16.txt",L,n);
write_output("output_U_2_16.txt",U,n);
}
  for(int i=0;i<n;++i){
    free(U[i]);
      free(L[i]);
    free(A[i]);}
  free(A);
  free(L);
  free(U);
}
