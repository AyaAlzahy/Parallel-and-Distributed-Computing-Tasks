#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


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
clock_t t;
t = clock();

int x; 
x= 0; /* lower bound*/
int y; 
y= 50000; /* upper bound*/
int total_prime_count;

int i; 
for(i=x; i< y; i++){
   if (is_prime(i)){
       total_prime_count ++; 
       }
    } 
   printf("\n Total count of prime numbers between %d and %d is %d\n", x, y, total_prime_count);

t = clock() - t;
double time_taken;
time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
printf("The serial program took %f seconds to execute\n", time_taken);

return 0;
}
