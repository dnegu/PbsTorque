/*
g++ matrix-vector.cpp -o matvec  -lpthread
./matvec 8

*/
/* Librerias */
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

/* Definicion de datos */
typedef int m_int;
typedef float m_float;
typedef size_t m_ST;

using namespace std;

/*Variables Globales*/
m_int nro_hilo=0;
m_float *respuesta, *respuesta2;
pthread_mutex_t mutex;

struct s_param{
	m_int nro_hilos;
	m_int m;
	m_int n;
	m_float *vector;
	m_float **matriz;
};

void crear_(m_float **&matriz, m_float *&v, m_int m, m_int n)
{	
	matriz= new m_float*[m];
	v= new m_float[n];

	for (register m_ST k = 0; k < m; ++k)
	{
		matriz[k] = new m_float[n];
	}
}

void liberar_(m_float **&matriz, m_float *&v, m_int m, m_int n)
{	
	delete[] v;

	for (register m_ST k = 0; k < m; ++k)
	{
		delete[] matriz[k];
	}
	delete[] matriz;
}

void generar_data(m_float **&matriz, m_float *&v, m_int m, m_int n) 
{
	for (register m_ST i = 0 ; i < m ; i++ )
		for (register m_ST j = 0 ; j < n ; j++ ) 
			matriz[i][j] = random ()%100;

	for (int k = 0; k < n; ++k)
		v[k] = random()% 100;
}

void mostrar_data(m_float **&matriz, m_float *&v, m_int m, m_int n) 
{
	cout<<"############# Matriz ###############"<<endl;
	for (register m_ST i = 0 ; i < m ; i++ ){
		for (register m_ST j = 0 ; j < n ; j++ ) 
			cout<<matriz[i][j]<<" ";
		cout<<endl;
	}
	cout<<"############## Vector ################"<<endl;
	for (register m_ST k = 0; k < n; ++k)
		cout<<v[k]<<" ";
	cout<<endl;
}

void mostrar_vector(m_float *v, m_int n) 
{
	cout<<"######### Vector Respuesta ##########"<<endl;
	for (register m_ST k = 0; k < n; ++k)
		cout<<v[k]<<" ";
	cout<<endl;
}

void * mul_matrix( void *arg ) 
{
	pthread_mutex_lock(&mutex);
 	struct s_param *p;
 	p = (struct s_param *) arg ;

    int inicio = (nro_hilo*p->m)/p->nro_hilos;
    int fin = inicio + (p->m/p->nro_hilos)- 1;

	for (register m_ST i = inicio; i <= fin; i++){
	  respuesta[i] = 0.0;
	  for (register m_ST j = 0; j < p->n; j++)
	      respuesta[i] += (p->matriz)[i][j]*(p->vector)[j];
	}
	nro_hilo++;
 	pthread_mutex_unlock(&mutex);
 }

void mul_normal(m_float **matriz,m_float *vector,m_float *&r, m_int m, m_int n)
{
	for(register m_ST i = 0; i < m; i++){
		r[i] = 0.0;
		for(register m_ST j = 0; j < n; j++)
			r[i] += matriz[i][j] * vector[j];
	}
}

int main(int argc , char *argv [])
{	
	size_t m, n, n_th;//tam mat y vec
	m_float **matriz,*vector; //mat y vec
	
	n_th=atoi(argv[1]); //obtencion del tamanho por consola
	m=n_th*2;//ingreso de dato
	n=n_th+2;//ingreso de dato
	respuesta = new m_float[m]; //asignacion de memoria
	respuesta2= new m_float[m]; //asignacion de memoria

	crear_(matriz,vector,m,n);
	generar_data(matriz,vector,m,n);//datos aleatorios
	struct s_param arg; 

	//argumentos para el hilo
    arg.nro_hilos=n_th;
	arg.m=m;
	arg.n=n;
	arg.vector=vector;
	arg.matriz=matriz;

	pthread_t* hilos = new pthread_t[n_th];
	
	for(register m_ST i = 0; i < n_th; i++)
		pthread_create(&hilos[i], NULL,mul_matrix, &arg);

	for (register m_ST k = 0; k < n_th; k++)
		pthread_join(hilos[k], NULL);
	
	mul_normal(matriz,vector,respuesta2,m,n);
	mostrar_data(matriz,vector,m,n);
	mostrar_vector(respuesta,n);
	mostrar_vector(respuesta2,n);
	pthread_exit(NULL);

	/*liberacion de memoria*/
	liberar_(matriz,vector,m,n);
	delete[] respuesta;
	delete[] respuesta2;
	delete[] hilos;
}