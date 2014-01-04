#ifndef _PROTO_
#define _PROTO_

#include "define.h"
#include "mpi.h"

#include <math.h>
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h> 

void ReadSparseInput (int *nvtxs, int *nsize, double **vector, int **rowptr, 
		      int **colind, double **values);

void AllocateInt(int **MyRows, int Numprocs);

void MatrixRowsCommunication(int nsize, int *MyFirstRow, int *MyLastRow, MPI_Comm comm); 

void SparseMatrixScatter(int nsize, int nvtxs, int *MyFirstRow, int *MyLastRow, 
			 double *vector, int *rowptr, int *colind, double *values, 
			 MPI_Comm comm);

void SerialSparseMatrixVector(int Myrows, int Mycols, int *Myrowptr, int *Mycolind, 
			      int *MyFirstRow, int *MyLastRow, double *Myvalues, 
			      double *vector, double *Myoutput, MPI_Comm comm);

void OutputGather(int nsize, int Myrows, int MyTotalVectorCount, int *MyFirstRow, int *MyLastRow, 
	    double *Myoutput, double *global_output, MPI_Comm comm);

#endif


