/*
g++ pi.cpp -o pi -lpthread
./pi 1 8

*/
/*Librerias*/
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>

/*Definicion de Tipos*/
typedef int m_int;
typedef float m_float;
using namespace std;

/*variables globales*/
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
long thread_count,n;
int flag=0;
double sum = 0.0, sum2=0.0;


long calcular_tiempo(timeval start, timeval end)
{
	long seconds, useconds;
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}

void *Thread_sum1(void *rank)
{

	long my_rank = long(rank);
	double factor;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	double my_sum = 0.0;

	if (my_first_i % 2 == 0)
		factor = 1.0;
	else
		factor = -1.0;
	
	for (i = my_first_i; i < my_last_i; i++, factor = -factor) 
	{	//while (flag != my_rank);
		my_sum += factor/(2*i+1);
		//flag=(flag+1) % thread_count;
	}
	while (flag != my_rank);
	sum += my_sum;
	flag = (flag+1) % thread_count;
	

}

void *Thread_sum2(void *rank)
{	

	long my_rank = long(rank);
	double factor;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	double my_sum = 0.0;

	if (my_first_i % 2 == 0)
		factor = 1.0;
	else
		factor = -1.0;
	
	for (i = my_first_i; i < my_last_i; i++, factor = -factor) 
	{	//while (flag != my_rank);
		my_sum += factor/(2*i+1);
		//flag=(flag+1) % thread_count;
	}
	pthread_mutex_lock(&mutex);
	sum2 += my_sum;
	pthread_mutex_unlock(&mutex);
	

}

double sum_serial(int n_)
{	
	n_=pow(10,n_);
	double factor = 1.0;
	double sum_ = 0.0;
	for (long long i = 0; i < n ; i ++, factor = -factor ) 
	{
		sum_ += factor /(2* i +1);
	}
	return 4.0* sum_;
}

int main(int argc , char *argv [])
{	
    double final_par, final_par2;

	thread_count=atoi(argv[1]);
	n=atoi(argv[2]);
	n=pow(10,n);

	const clock_t begin_time = clock();

	pthread_t* hilos = new pthread_t[thread_count];

	for(int i = 0; i < thread_count;i++)
	{	

		pthread_create(&hilos[i], NULL,Thread_sum1, (void *)i);
		//pthread_join(hilos[i], NULL);
	}

	for (int k = 0; k < thread_count; k++)
		pthread_join(hilos[k], NULL);

	final_par=double( clock () - begin_time ) /  CLOCKS_PER_SEC;
	printf("Pi=%.8f\n",4.0*sum);

	const clock_t begin_time2 = clock();

	pthread_t* hilos2 = new pthread_t[thread_count];

	for(int ii = 0; ii < thread_count;ii++)
	{	
		pthread_create(&hilos2[ii], NULL,Thread_sum2, (void *)ii);
		//pthread_join(hilos[i], NULL);
	}

	for (int k = 0; k < thread_count; k++)
	{	
		pthread_join(hilos2[k], NULL);
	}

	final_par2=double( clock () - begin_time2 ) /  CLOCKS_PER_SEC;
	
	delete[] hilos;
	delete[] hilos2;
	//printf("Pi=%.8f\n",4.0*sum2);
	
	printf("bussy-wait=%f mutex=%f \n",final_par,final_par2);

}