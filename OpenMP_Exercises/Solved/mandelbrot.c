/*
**  PROGRAM: Mandelbrot area (solution)
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           The correct answer should be around 1.510659.
**
**  USAGE:   Program runs without input ... just run the executable
**
**  ADDITIONAL EXERCISES:  Experiment with the schedule clause to fix
**               the load imbalance.   Experiment with atomic vs. critical vs.
**               reduction for numoutside.
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type.   Fixed data environment errors
**          (Tim Mattson, September 2011)
**
**           Added task based implementation and performance evaluation code
**           (Luis Miguel Pinho, July 2023)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS 1000
#define MAXITER 10000

struct d_complex
{
    double r;
    double i;
};

void testpoint(struct d_complex);

struct d_complex c;
int numoutside = 0;

int main()
{
    int i, j;
    double seq_area, seq_error, par_area, par_error, task_area, task_error, eps = 1.0e-5;

    //   Loop over grid of points in the complex plane which contains the Mandelbrot set,
    //   testing each point to see whether it is inside or outside the set.

    double begin = omp_get_wtime();

    for (i = 0; i < NPOINTS; i++)
    {
        for (j = 0; j < NPOINTS; j++)
        {
            c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
            c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
            testpoint(c);
        }
    }

    // Calculate area of set and error estimate and output the results

    seq_area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
    seq_error = seq_area / (double)NPOINTS;

    double end = omp_get_wtime();
    double sequential_time = end - begin;

    // Reset
    numoutside = 0;

    begin = omp_get_wtime();

    #pragma omp parallel for default(shared) firstprivate(eps) private(c, j)
    for (i = 0; i < NPOINTS; i++)
    {
        for (j = 0; j < NPOINTS; j++)
        {
            c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
            c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
            testpoint(c);
        }
    }

    // Calculate area of set and error estimate and output the results

    par_area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
    par_error = par_area / (double)NPOINTS;

    end = omp_get_wtime();
    double parallel_time = end - begin;

    // Reset
    numoutside = 0;

    begin = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
    for (i = 0; i < NPOINTS; i++)
    {
        for (j = 0; j < NPOINTS; j++)
        {
            #pragma omp task firstprivate(i, j, c)
            {
                c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
                c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
                testpoint(c);
            }
        }
    }

    // Calculate area of set and error estimate and output the results

    task_area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
    task_error = task_area / (double)NPOINTS;

    end = omp_get_wtime();
    double task_time = end - begin;

    printf("Area of Mandlebrot set (seq) = %12.8f +/- %12.8f\n", seq_area, seq_error);
    printf("Area of Mandlebrot set (par) = %12.8f +/- %12.8f\n", par_area, par_error);
    printf("Area of Mandlebrot set (tasks) = %12.8f +/- %12.8f\n", task_area, task_error);
    printf("Correct answer should be around 1.510659\n");

    printf("\n- ==== Performance ==== -\n");
    printf("Sequential time: %fs\n", sequential_time);
    printf("Parallel   time: %fs\n", parallel_time);
    printf("Parallel   time: %fs\n", task_time);
}

void testpoint(struct d_complex c)
{

    // Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
    // If loop count reaches MAXITER, point is considered to be inside the set

    struct d_complex z;
    int iter;
    double temp;

    z = c;
    for (iter = 0; iter < MAXITER; iter++)
    {
        temp = (z.r * z.r) - (z.i * z.i) + c.r;
        z.i = z.r * z.i * 2 + c.i;
        z.r = temp;
        if ((z.r * z.r + z.i * z.i) > 4.0)
        {
            #pragma omp atomic // What happens if this is removed?
            numoutside++;
            break;
        }
    }
}
