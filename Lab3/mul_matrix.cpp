/* Compilar
mpicxx -o mul mul_matrix.cpp  -Wall
mpirun -np 2 ./mul

*/

#include <stdio.h>
#include <mpi.h>

#define D_data double
#define I_data int

using namespace std;

D_data *llenar_vect(I_data num_elem) 
{
  D_data *vector = new D_data[num_elem];
  
  for (I_data i = 0; i < num_elem; i++) {
    vector[i] = 1;
  }
 	
  return vector;
}


void Mat_vect_mult(D_data *local_A, D_data *x, D_data *local_y, I_data local_m, I_data local_n, I_data n, MPI_Comm comm)
{
	

	for (I_data local_i = 0; local_i < local_m; ++local_i)
	{	
		local_y[local_i]=0.0;
		for (I_data j = 0; j < n; ++j)
		{
			local_y[local_i] += local_A[local_i*n+j]*x[j];
		}
	}
	MPI_Allgather(&local_y, local_n, MPI_DOUBLE, local_y, local_n, MPI_DOUBLE, comm);
}


int main(int argc, char const *argv[])
{	  
	  D_data local_start, local_finish, local_elapsed, elapsed;
	  I_data m, n, local_n;
	  m=32; n=32;
	  D_data *x=llenar_vect(n);

	  MPI_Init(NULL, NULL);

	  int world_rank;
	  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	  int world_size;
	  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	  if (m<world_size || m%world_size==1)
	  {		
	  		printf("Cambiar el numero de procesos!!\n");
	  		MPI_Finalize();
	  		return 0;
	  }

	  local_n=m/world_size;

	  //Llenar el vector global
	  D_data *vector_global = NULL;
	  if (world_rank == 0) {
	  		vector_global = llenar_vect(m*n);
	  }
	  
	  MPI_Barrier(MPI_COMM_WORLD);
	  local_start = MPI_Wtime();
	  //Crear un buffer para el subvector local
	  D_data *vector_local = new D_data[n*local_n];
	  D_data *respuesta = new D_data[m];

	  MPI_Scatter(&vector_global, n*local_n, MPI_DOUBLE, vector_local, n*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	  Mat_vect_mult(vector_local,x,respuesta,local_n,local_n,n,MPI_COMM_WORLD);
	  MPI_Barrier(MPI_COMM_WORLD);
	  local_finish = MPI_Wtime();
	  local_elapsed = local_finish - local_start;
	  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

	  if (world_rank==0)
	  {
		   printf("Elapsed time = %.4f seconds\n", elapsed*1000);  
	  }

	  MPI_Finalize();
}