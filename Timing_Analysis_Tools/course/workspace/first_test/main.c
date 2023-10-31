#include <stdio.h>
int main()
{
    #pragma omp parallel
    #pragma omp single
    {
        int x, y;
        #pragma omp task depend(out:x)
        printf("Hi from task 1\n");

        #pragma omp task depend(out:y)
        printf("Hi from task 2\n");

        #pragma omp task depend(in:x,y)
        printf("Hi from task 3\n");
    }
}