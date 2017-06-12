#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Compilar comandos
  mpicc -o tarea_mpi tarea_mpi.c
  mpirun -np 2 tarea_mpi

  Añadir al Path
  export PATH=:$PATH
*/

int main(int argc, char** argv) {
  
  int my_rank, my_size;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &my_size);

  int ping_pong = 0;
  int Destino = (my_rank + 1) % 2;
  
  while (ping_pong < 10) 
  { 

    if (my_rank == 0) 
    {
      ping_pong++;
      MPI_Send(&ping_pong, 1, MPI_INT, Destino, 0, MPI_COMM_WORLD);
      sleep(2);
      printf("%d envia ping_pong %d a %d\n",my_rank, ping_pong, Destino);
      
    } else {
      MPI_Recv(&ping_pong, 1, MPI_INT, Destino, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sleep(2);
      printf("%d recive ping_pong %d de %d\n",my_rank, ping_pong, Destino);
      printf("\n");

    }
  }
  MPI_Finalize();
}