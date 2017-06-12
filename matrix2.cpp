/*
g++ matrix2.cpp -o mat2  -lpthread
./mat2 1 8 8000000
*/

/*Librerias*/
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>

/*Definicion de Tipos*/
typedef int m_int;
typedef size_t m_ST;
using namespace std;

/*Variables globales*/
m_int thread_count;
long m,n;
double **Matriz,*vec1,*y,*r;

void crear_()
{	
	Matriz= new double*[m];
	vec1= new double[n];
	y= new double[m];
	r= new double[m];
	for(register m_ST k=0; k < m; ++k)
		Matriz[k] = new double[n];
}

void liberar_()
{	
	delete[] vec1;
	delete[] y;
	delete[] r;

	for (register m_ST k = 0; k < m; ++k)
		delete[] Matriz[k];
	delete[] Matriz;
}

void generar_data() 
{

	for (register m_ST i = 0 ; i < m ; i++ )
		for (register m_ST j = 0 ; j < n ; j++ ) 
			Matriz[i][j] = random ()%10;

	for (register m_ST k = 0; k < n; ++k)
		vec1[k] = random()% 10;
}

void *Pth_mat_vect(void* rank) 
{
	long my_rank = (long) rank;
	register m_ST i, j;
	int local_m = m/thread_count;
	int my_first_row = my_rank*local_m;
	int my_last_row = (my_rank+1)*local_m-1;
	for (i = my_first_row; i <= my_last_row; i++) 
	{
		y[i] = 0.0;
		for (j = 0; j < n; j++)
			y[i] += Matriz[i][j]*vec1[j];
	}
}/* Pth mat vect */

void mul_normal()
{
	
	for(register m_ST i = 0; i < m; i++)
	{
		r[i] = 0.0;
		for(register m_ST j = 0; j < n; j++)
			r[i] += Matriz[i][j] * vec1[j];
	}
}	
int main(int argc , char *argv [])
{	
	double final_par,final_serial;
	thread_count=atoi(argv[1]);
	m=atoi(argv[2]);
	n=atoi(argv[3]);
	crear_();
	generar_data();
	
	const clock_t begin_time = clock();
	pthread_t* hilos = new pthread_t[thread_count];
	
	for(register m_ST i = 0; i < thread_count; i++)
		pthread_create(&hilos[i], NULL,Pth_mat_vect, (void*)i);
	
	for (register m_ST k = 0; k < thread_count; k++)
		pthread_join(hilos[k], NULL);

	final_par=double( clock () - begin_time ) /  CLOCKS_PER_SEC;
	const clock_t begin_time2 = clock();
	mul_normal();
	final_serial=double( clock () - begin_time2 ) /  CLOCKS_PER_SEC;
	printf("Tiempo=%f Eficiencia=%f \n",final_par,final_serial/(thread_count*final_par));
	liberar_();

	delete[] hilos;
}