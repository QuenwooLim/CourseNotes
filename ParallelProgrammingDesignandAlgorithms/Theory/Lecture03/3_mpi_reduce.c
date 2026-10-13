#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void)
{
   char greeting[MAX_STRING]; 
   int  comm_sz;
   int  my_rank;
   int  a, b, c, d;

   MPI_Init(NULL, NULL);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if(my_rank == 0)
   {
        a = 1; 
        c = 2;
        MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&c, &d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("a=%d, b=%d, c=%d, d=%d\n", a, b, c, d);
   }
   if(my_rank == 1)
   {
        a = 1; 
        c = 2;
        MPI_Reduce(&c, &d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);        
   }
   if(my_rank == 2)
   {
        a = 1; 
        c = 2;
        MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&c, &d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   }   
   

   MPI_Finalize();
   return 0;
}