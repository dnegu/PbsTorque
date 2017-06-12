/*
mpicxx -o sum sca_gat.cpp -Wall 
 mpirun -np 2 ./sum
*/

#include <stdio.h>
#include <mpi.h>

typedef int m_int;
typedef float m_float;
typedef size_t m_ST;

using namespace std;

m_float *llenar_vect(m_int num_elem) 
{
  m_float *vector = new m_float[num_elem];
  
  for (register m_ST i = 0; i < num_elem; i++) 
    vector[i] = 1;

  return vector;
}

m_float sumar_vect(m_float *vector, m_int num_elem) 
{
  m_float sum = 0.0;
  
  for (register m_ST i = 0; i < num_elem; i++) {
    sum += vector[i];
  }
  return sum;
}


int main(int argc, char** argv) 
{
	  m_int m_num_elem = 10;

	  MPI_Init(NULL, NULL);

	  int world_rank;
	  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	  int world_size;
	  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	  m_float *vec_global = NULL;
	  if (world_rank == 0) {
	    vec_global = llenar_vect(m_num_elem * world_size);
	  }

	  m_float *vector_local = new m_float[m_num_elem];
  	  
  	  MPI_Scatter(vec_global, m_num_elem, MPI_FLOAT, vector_local,m_num_elem, MPI_FLOAT, 0, MPI_COMM_WORLD);
  	  m_float sum_local = sumar_vect(vector_local, m_num_elem);

	  m_float *Resultados = NULL;
	  if (world_rank == 0) {
	    Resultados= new m_float[world_size];
	  }

	  MPI_Gather(&sum_local, 1, MPI_FLOAT, Resultados, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	  if (world_rank == 0) {
	    m_float suma_total = sumar_vect(Resultados, world_size);
	    printf("La suma de elementos es  %f\n", suma_total);
	    
	    m_float suma_global = sumar_vect(vec_global, m_num_elem * world_size);
	    printf("La suma de elementos de vec_global es %f\n", suma_global);
	  }	

	  if (world_rank == 0) {
	    delete vec_global;
	    delete Resultados;
	    
	  }
	  delete vector_local;

	  MPI_Barrier(MPI_COMM_WORLD);
	  MPI_Finalize();
}