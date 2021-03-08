#include <omp.h> 
  
#include <stdio.h> 
#include <stdlib.h> 


long long method0(int start, int end , int thread_count){
	int local_start = start;
	int local_end ;int local_gap; long long my_result = 0;
	int my_rank = omp_get_thread_num();
	//printf("%d\n", my_rank);
	int gap = (end-start)/thread_count;
	if(my_rank==thread_count-1){
	local_start = start + my_rank*gap + 1;
	local_end = local_start + gap;
	for(int i = local_start;i<=end;i++){
		//printf("%d\n", i);
		my_result = my_result + i;
}

}
	else{
	local_start = start + my_rank*gap + 1;
	local_end = local_start + gap-1;
	for(int i = local_start;i<=local_end;i++){
		//printf("%d\n", i);
		my_result = my_result + i;
		}
	

}

	return my_result;

}



int main(int argc, char* argv[]) 
{ 
	double start = omp_get_wtime();
   unsigned long long int sum=0;  
 
	int method = atoi(argv[1]);
	//printf("%d\n", method);
	int thread_count = atoi(argv[2]);
	//printf("%d\n", thread_count);
	int N = atoi(argv[3]);
	//printf("%d\n", N);
	 long long int a[thread_count] ;
	
	if(method==0){
	#pragma omp parallel num_threads(thread_count)
	{
	unsigned long long local_sum = 0;
	local_sum = method0(0,N,thread_count);
	//printf("%llu\n", local_sum); 
	#pragma omp critical 
	sum = sum + local_sum; 
	}

	printf("%llu\n", sum);
	}
	else if(method==1){
	#pragma omp parallel num_threads(thread_count)
	{
	unsigned long long local_sum = 0;
	local_sum = method0(0,N,thread_count);
	int my_rank = omp_get_thread_num();
	a[my_rank] = local_sum;
	}
	
	while(thread_count>1){
	thread_count = thread_count/2;
	long long int b[thread_count];
	#pragma omp parallel num_threads(thread_count)
	for(int i=0;i<thread_count;i++){
	b[i] = a[2*i] + a[2*i+1];
}
	#pragma omp parallel num_threads(thread_count)
	for(int i=0;i<thread_count;i++){
	a[i] = b[i];
}
	
	
}
	printf("%llu\n", a[0]);  
}
	
double stop = omp_get_wtime();	
printf("%f\n", stop-start);	  
} 



