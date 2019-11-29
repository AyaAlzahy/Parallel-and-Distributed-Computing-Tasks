#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

int is_prime(int n) {
   int UPPER_LIMIT; 
   int i; 
   if (n== 0 || n==1){ return 0;}
   UPPER_LIMIT = (int)(sqrt((double)n)+0.0001);
   for (i = 2; i <= UPPER_LIMIT; i++){
      if ( n % i == 0 ){
         return 0;
      }
    }
   return 1;
}


int main(int argc , char * argv[])
{
int x; 
x= 0; /* lower bound*/
int y; 
y= 1000; /* upper bound*/
int a; /* lower bound of each slave process*/
int b; /* upper bound of each slave process*/
int r;
int my_rank; /* rank of process */
int p; /* number of process */
int source; /* rank of sender */
int dest; /* rank of reciever */
int count_prime_slave;
int total_prime_count;
int message[2]; /* x and p */
int tag = 0; /* tag for messages */
MPI_Status status; /* return status for */

/* Start up MPI */
MPI_Init( &argc , &argv );
/* Find out process rank */
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
/* Find out number of process */
MPI_Comm_size(MPI_COMM_WORLD, &p);

/* Master work*/
if(my_rank == 0) {
   double start;
   start= MPI_Wtime();
/* calculate prime count from the master portion*/
   r= (y-x)/p;
   message[0]=x;
   message[1]=r;
   a= x; 
   b= x+r;
   int i; 
   for(i=a; i< b; i++){
       if (is_prime(i)){
           total_prime_count ++; 
       }
    } 
   printf("\nmaster found %d prime numbers between %d and %d \n", total_prime_count, a, b);
   for(dest= 1; dest<p ; dest++) {
       MPI_Send(&message, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);}
   for (source=1; source<p; source++){
       MPI_Recv(&count_prime_slave, 1, MPI_INT, source, tag, MPI_COMM_WORLD,&status);
       total_prime_count += count_prime_slave; 
    }
double elapsed;
elapsed = MPI_Wtime() - start;
printf("\nTotal number of primes between 0 and %d:  %d.\n", y, total_prime_count);
printf("\nElapsed time:  %1.3f seconds.\n", elapsed);
}
else{
   source= 0;
   MPI_Recv(&message, 2, MPI_INT, source, tag, MPI_COMM_WORLD,&status);
   x= message[0]; 
   r= message[1];
   /* when the range doesn't divide evenly, the last slave operates on the rest of the range*/
   if (my_rank == p-1) { b= y; a= my_rank*r; } 
   else { a= my_rank* r; /* upper bound*/ 
   b= a+r; /* lower bound*/
   }
   int i; 
   for(i=a; i< b; i++){
       if (is_prime(i)){
           count_prime_slave ++; 
       }
    }
   dest = 0;
   MPI_Send(&count_prime_slave, 1, MPI_INT, dest, tag,MPI_COMM_WORLD);
   printf("\nslave %d found %d prime numbers between %d and %d \n", my_rank, count_prime_slave, a, b);
}

/* shutdown MPI */
MPI_Finalize();
return 0;
}
