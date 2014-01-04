
#include "proto.h"
void OutputGather(int nsize, int Myrows, int MyTotalVectorCount, int *MyFirstRow, 
                  int *MyLastRow, double *Myoutput, double *global_output, MPI_Comm comm) 

{
  int Root=0;
  int i, j, Numprocs, MyRank; 
  int *TotalRowCount, *TotalColCount, *TotalVectorCount;
  int MyLocalVectorCount;
  
  int start_row, end_row, iproc;
  int *Displacementvector, *RecvCountvector;

  MPI_Comm_size(comm, &Numprocs);
  MPI_Comm_rank(comm, &MyRank);

  Displacementvector        = (int *) malloc(Numprocs * sizeof(int));	
  RecvCountvector 	    = (int *) malloc(Numprocs * sizeof(int));	
  TotalVectorCount 	    = (int *) malloc(Numprocs * sizeof(int));	

  for(iproc=0; iproc<Numprocs; iproc++) 
  {
    start_row                 = MyFirstRow[iproc];
    end_row                   = MyLastRow[iproc]; 
    TotalVectorCount[iproc]   = end_row - start_row; 
    Displacementvector[iproc] = start_row;
    RecvCountvector[iproc]    = TotalVectorCount[iproc]; 
  }

    MyLocalVectorCount  = TotalVectorCount[MyRank];
    assert(MyLocalVectorCount == Myrows);

    if(MyRank == Root) 
    {
       MPI_Gatherv(Myoutput, MyLocalVectorCount,  MPI_DOUBLE, 
  	    global_output, RecvCountvector, Displacementvector,  MPI_DOUBLE, Root, comm); 
    }
    else
    {
       MPI_Gatherv(Myoutput, MyLocalVectorCount,  MPI_DOUBLE, 
  	    global_output, RecvCountvector, Displacementvector,  MPI_DOUBLE, Root, comm); 
    }

    free(TotalVectorCount);
    free(Displacementvector);
    free(RecvCountvector);
}
