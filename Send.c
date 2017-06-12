mpicc -o se Send.c -Wall
mpirun -np 8 ./se

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int world_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int number;
    if (world_rank == 0) {
    number = -1;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else if (world_rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n",number);
	}

	MPI_Finalize();	
}
