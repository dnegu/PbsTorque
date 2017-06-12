#include <mpi.h>
#include "mpi_mutex.h"

pthread_mutex_t mutex;
pthread_mutex_init(&mutex,);
int flag;

void Mat_vect_mult(double local_A[], double local_x[],double local_y[],int local_m,int n, int local_n, MPI_Comm comm)
{
    double *X;
    int local_i,j;
    int local_ok = 1;

    x = malloc(n*sizeof(double));
    MPI_Allgather(local_x,local_n,MPI_DOUBLE,x,local_n, MPI_DOUBLE,comm);

    for(local_i=0;local_i<local_m;local_i++)
    {
        local_y[local_i]=0.0;
        for(j=0;j<n;j++)
            local_y[local_i]+=local_A[local_i*n+j]*x[j];
    }

    free(x);
}

void* Thread_sumB(void* rank)
{
    long my_rank = (long)rank;
    double factor;
    long long i;
    long long my_n = n/thread_count;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i+my_n;
    double my_sum = 0.0;


    if(my_first_i%2==0) factor = 1.0;
    else factor = -1.0;

    for(i=my_first_i; i<my_last_i;i++,factor=-factor)
        my_sum += factor/(2*i+1);

    while(flag!=my_rank);

    sum += my_sum;
    flag = (flag+1)%thread_count;

    return NULL;
}

void* Thread_sumM(void* rank)
{
    long my_rank = (long)rank;
    double factor;
    long long i;
    long long my_n = n/thread_count;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i+my_n;
    double my_sum = 0.0;


    if(my_first_i%2==0) factor = 1.0;
    else factor = -1.0;

    for(i=my_first_i; i<my_last_i;i++,factor=-factor)
        my_sum += factor/(2*i+1);

    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{


}