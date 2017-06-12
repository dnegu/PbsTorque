/*
g++ strk.cpp -o str -lpthread
./str 4
*/
/*Librerias*/
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>

/*Definicion de Tipos y variables globales*/
typedef int m_int;
#define MAX 28
using namespace std;

m_int thread_count;
FILE *stdin;
sem_t *sems;
char *saveptr;

void* Tokenize(void* rank) 
{
	long my_rank = (long)rank;
	int count;
	int next = (my_rank + 1) % thread_count;
	char *fg_rv;
	char my_line[MAX];
	char *my_string;
	sem_wait(&sems[my_rank]);
	fg_rv = fgets(my_line, MAX, stdin);
	sem_post(&sems[next]);
	while (fg_rv != NULL) 
	{
		printf("Thread %ld > my line = %s", my_rank, my_line);
		count = 0;
		my_string = strtok(my_line, " \t\n");
		while ( my_string != NULL ) 
		{
			count++;
			printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
			my_string = strtok(NULL, " \t\n");
		}
		sem_wait(&sems[my_rank]);
		fg_rv = fgets(my_line, MAX, stdin);
		sem_post(&sems[next]);
	}
}/* Tokenize */

void* Tokenize2(void* rank) 
{
	long my_rank = (long)rank;
	int count;
	int next = (my_rank + 1) % thread_count;
	char *fg_rv;
	char my_line[MAX];
	char *my_string;
	sem_wait(&sems[my_rank]);
	fg_rv = fgets(my_line, MAX, stdin);
	sem_post(&sems[next]);
	while (fg_rv != NULL) 
	{
		printf("Thread %ld > my line = %s", my_rank, my_line);
		count = 0;
		my_string = strtok_r(my_line, " \t\n", &saveptr);
		while ( my_string != NULL ) 
		{
			count++;
			printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
			my_string = strtok_r(NULL, " \t\n", &saveptr);
		}
		sem_wait(&sems[my_rank]);
		fg_rv = fgets(my_line, MAX, stdin);
		sem_post(&sems[next]);
	}
}/* Tokenize */

int main(int argc , char *argv [])
{	
	stdin = fopen("text.in" , "r");
	thread_count=2;
	sems = new sem_t[thread_count];
	sem_init(&sems[0], 0, 1);
	for (int i = 1; i < thread_count; ++i)
	{
		sem_init(&sems[i], 0, 0);
	}

	pthread_t* hilos = new pthread_t[thread_count];
	
	for(int i = 0; i < thread_count; i++)
	{		
		pthread_create(&hilos[i], NULL,Tokenize2, (void*)i);
	}


	for (int k = 0; k < thread_count; k++)
	{	
		pthread_join(hilos[k], NULL);
	}
	
}