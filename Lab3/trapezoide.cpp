/*
mpicxx -o trap trapezoide.cpp -Wall
mpirun -np 2 ./trap
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define D_data double
#define I_data int

using namespace std;

/* Funcion que vamos a integrar*/
double f(double x)
{     
      return 2*(pow(x,3));
}

// Calcular la integral en cada proceso(local)
D_data Trap(D_data lim_inferior, D_data lim_superior, I_data nro_trap, D_data h)
{

   D_data int_aprox, x; 
   I_data i;

   int_aprox = f(lim_inferior) + f(lim_superior);
   for (i = 1; i <= nro_trap-1; i++) 
   {
      x = lim_inferior + i*h;
      int_aprox += f(x);
   }
   int_aprox = 2.0*int_aprox;

   return (int_aprox*h)/2.0;

}     

int main(int argc, char ** argv) {

   I_data my_rank, comm_sz, n = 100000000, local_n;   
   D_data a = 0.0, b = 5.0, h, local_a, local_b;
   D_data local_int = 0.0, total_int = 0.0;
   //I_data current_id; 

   /* incia mpi */
   MPI_Init(&argc, &argv);
 
   /* Da el id del proceso actual */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Numero de procesos */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   h = (b-a)/n;          /* Avance de cada integral*/
   local_n = n/comm_sz;  /* Numero de trapezoides para cada proceso*/

   local_a = a + my_rank * local_n * h; //limite inferior para cada proceso
   local_b = local_a + local_n * h;  //Limite superior para cada proceso  
   local_int = Trap(local_a, local_b, local_n, h); // Calculo de la integral local

/* //Usando send y recv
   if (my_rank != 0) { 
      MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
   } else {
      total_int = local_int;
      for (current_id = 1; current_id < comm_sz; current_id++) {
         MPI_Recv(&local_int, 1, MPI_DOUBLE, current_id, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         total_int += local_int;
      }
   } 
   */

   //Usando reduce :D
   MPI_Reduce(&local_int,&total_int, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

   /*Imprimir el resultado*/
   if (my_rank == 0) {
      printf("n = %d trapezoides, resultado aprox: \n", n);
      printf("Integral de  %f a %f = %.4f\n", a, b, total_int);
   }

   MPI_Finalize();

   return 0;
} /*  main  */
