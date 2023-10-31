//
// This file is part of Heat Gauss Seidel  and is licensed under the terms contained in the COPYING file.
// Copyright (C) 2015-2020 Barcelona Supercomputing Center (BSC)
//

/*
 * Iterative solver for heat distribution
 */
#include "heat.h"
#include "omp.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#if !defined(NB)
#error All versions must define the number of blocks (NB)
#endif


#ifndef _OPENMP
    int omp_get_max_threads() { return 1; }
#endif

void usage( char *s )
{
    fprintf(stderr, "Usage: %s <input file>\n\n", s);
}

double (*u)[NP];

/* The Gauss Seidel Heat function*/
double relax_gauss(double *u1)
{
  double sum = 0.0;

  int sizex = NP;
  int sizey = NP;

  u = (double (*)[sizey])u1;
  int nbx = NB;
  
  int nby = NB;
  
  
  for (int ii=0; ii<NB; ii++) {
      for (int jj=0; jj<NB; jj++) {
          int inf_i = 1 + ii * bx;
          int sup_i = ((inf_i + bx) < sizex - 1) ? inf_i + bx : sizex - 1;
          int inf_j = 1 + jj * by;
          int sup_j = ((inf_j + by) < sizey - 1) ? inf_j + by : sizey - 1;

          #pragma omp task depend(in: u[inf_i-bx][inf_j], \
                                      u[sup_i][inf_j],    \
                                      u[inf_i][inf_j-by], \
                                      u[inf_i][sup_j])    \
                          depend(inout: u[inf_i][inf_j]) firstprivate(sizex, sizey, u)
	 	  {
              for (int i = inf_i; i < sup_i; ++i) {
                  for (int j = inf_j; j < sup_j; ++j) {
                      double unew;
                      unew = 0.25 * (u[i][j-1] + u[i][j+1] + u[i-1][j] + u[i+1][j]);
                      u[i][j]=unew;
                  }
              }
          }
      }
  }
  return sum;
}

int main( int argc, char *argv[] )
{
    FILE *infile, *resfile;

    // algorithmic parameters
    algoparam_t param;
    int np;

    double runtime;
    // double residual=0.0;
    unsigned iter = 0;

    // check arguments
    if( argc < 2 )
    {
	usage( argv[0] );
	return 1;
    }

    // check input file
    if( !(infile=fopen(argv[1], "r"))  )
    {
	fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);

	usage(argv[0]);
	return 1;
    }

    // check result file
    unsigned size = strlen(argv[0]) + 4 + 1;
    char resfilename[size]; 
    resfilename[0] = '\0';
    strcat(resfilename, argv[0]);
    strcat(resfilename, ".ppm");
/*
    if( !(resfile=fopen(resfilename, "w")) )
    {
	fprintf(stderr, "\nError: Cannot open \"%s\" for writing.\n\n", resfilename);
	usage(argv[0]);
	return 1;
    }
*/
    // check input
    if( !read_input(infile, &param) )
    {
	fprintf(stderr, "\nError: Error parsing input file.\n\n");
	usage(argv[0]);
	return 1;
    }

    assert((param.algorithm == 1)
            && "Only Gauss-seidel is currently implemented\n");

    if( !initialize(&param) )
	{
	    fprintf(stderr, "Error in Solver initialization.\n\n");
	    usage(argv[0]);
            return 1;
	}

    // full size (param.resolution are only the inner points)
    // np = param.resolution + 2;
    np = RESOLUTION + 2;

    // starting time
     runtime = wtime();

    for (int i=0; i<NUM_ITER; i++)
    {
        for(iter=0; iter < param.maxiter; ++iter)
        {
            #pragma omp parallel
            #pragma omp single
            relax_gauss(param.u);
        }
    }

    // stopping time
    runtime = wtime() - runtime;

    fprintf(stdout,"Run time: %f\n", runtime);
    // write_image(resfile, param.u, NP, NP);
    finalize( &param );
    return 0;
}
