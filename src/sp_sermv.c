#include "proto.h"

void SerialSparseMatrixVector(int Myrows, int Mycols, int *Myrowptr, int *Mycolind, 
			      int *MyFirstRow, int *MyLastRow, double *Myvalues, 
			      double *vector, double *Myoutput, MPI_Comm comm) 
{

  int  i, j, count, npes, mype, start, end, globalrow, global_colindex;

  MPI_Comm_size(comm, &npes);
  MPI_Comm_rank(comm, &mype);

  start = MyFirstRow[mype];
  end   = MyLastRow[mype];
  count = 0;

  for (i = 0; i < Myrows; i++) {
    Myoutput[i] = 0.0;
    for(j = Myrowptr[i]; j<Myrowptr[i+1]; j++) {
	   global_colindex = Mycolind[count];
           Myoutput[i] += Myvalues[count] * vector[Mycolind[count]];   
	   count++;
    }
  }  
}
