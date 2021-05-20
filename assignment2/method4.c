#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
int thread_count ;


	 FILE *file;

  file=fopen(file_name, "r");
  ///int n =4;



 int rank ;
  MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	/* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &thread_count);	/* get number of processes */

  double**  A=(double**)malloc(n*sizeof(double*));
  for(int i=0;i<n;++i){
    A[i]=(double*)malloc(n*sizeof(double));  }
    double**  L=(double**)malloc(n*sizeof(double*));
    for(int i=0;i<n;++i){
      L[i]=(double*)malloc(n*sizeof(double));  }
      double**  U=(double**)malloc(n*sizeof(double*));
      for(int i=0;i<n;++i){
        U[i]=(double*)malloc(n*sizeof(double));  }
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

        double sum = 0;
        for (int i = 0; i < n; i++) {
            U[i][i] = 1;
        }
      //  for(int i=0;i<n;i++){
        //  for(int j=0;j<n;j++){
          //  printf("U[%d][%d] = %lf\n ",i,j,U[i][j]);
        //  }
        //}

        for (int j = 0; j < n; j++) {

          for (int i = j; i < n; i++) {
            if(i%thread_count==rank)
            {
              sum = 0;
              for (int k = 0; k < j; k++) {
                  sum = sum + L[i][k] * U[k][j];
              }
              L[j][i]=0;
              L[i][j] = A[i][j] - sum;
            }
              //printf("error1 i=%d, j=%d\n",i,j);
          //  printf("A[i][j] = %lf\n",A[i][j]);
              //printf("L[i][j] = %lf\n",L[i][j]);
            //  fflush(stdout);
              MPI_Bcast(&L[i][j], 1, MPI_DOUBLE, i%thread_count, MPI_COMM_WORLD);

              //printf("L[%d][%d] = %lf aEnd sum = %lf\n",i,j,L[i][j],sum);
          }
          MPI_Barrier(MPI_COMM_WORLD);
          for (int i = j; i < n; i++) {
            if(i%thread_count==rank)
            {
              sum = 0;
              for(int k = 0; k < j; k++) {
                  sum = sum + L[j][k] * U[k][i];
              }
              if (L[j][j] == 0) {
                //printf("error\n j  = %d",j);
                //fflush(stdout);
                  exit(0);
              }
              U[i][j] = 0;
              U[j][i] = (A[j][i] - sum) / L[j][j];
                }
            //  printf("error2\n");
              //fflush(stdout);
              MPI_Bcast(&U[j][i], 1, MPI_DOUBLE, i%thread_count, MPI_COMM_WORLD);

        }
        MPI_Barrier(MPI_COMM_WORLD);


}
MPI_Barrier(MPI_COMM_WORLD);


   MPI_Finalize();
 if(thread_count==1){
  write_output("output_L_4_1.txt",L,n);
  write_output("output_U_4_1.txt",U,n);
}
if(thread_count==2){
write_output("output_L_4_2.txt",L,n);
write_output("output_U_4_2.txt",U,n);
}
if(thread_count==4){
write_output("output_L_4_4.txt",L,n);
write_output("output_U_4_4.txt",U,n);
}
if(thread_count==8){
write_output("output_L_4_8.txt",L,n);
write_output("output_U_4_8.txt",U,n);
}
if(thread_count==16){
write_output("output_L_4_16.txt",L,n);
write_output("output_U_4_16.txt",U,n);
}
//printf("ln\n");
  //write_output("L.txt",L,n);
  //write_output("U.txt",U,n);
  for(int i=0;i<n;++i){
    free(U[i]);
      free(L[i]);
    free(A[i]);}
  free(A);
  free(L);
  free(U);
}
