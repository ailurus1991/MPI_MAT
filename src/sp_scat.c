#include "proto.h"

int    *Mycolind;
int    *Myrowptr; 
double *Myvalues;
double *Myvector; 
int    MyTotalRowCount;
int    MyTotalColCount; 
int    MyTotalVectorCount;

void SparseMatrixScatter(int nsize, int nvtxs, int *MyFirstRow, int *MyLastRow, double *vector, 
			 int *rowptr, int *colind, double *values, MPI_Comm comm)
{
     int 	Root = 0; 
     int 	Numprocs, MyRank;
     int 	Destination, Source, Destination_tag, Source_tag;
     MPI_Status status;

     int        *TotalRowCount, *TotalColCount, *TotalVectorCount;
     int        start_row, end_row;
     int        *DisplacementRowptr, *SendCountRowptr;
     int        *Displacementcolind, *SendCountcolind;
     int        *Displacementvector, *SendCountvector;
     int        iproc;

     int start_rowptr, end_rowptr;

     MPI_Comm_size(comm, &Numprocs);
     MPI_Comm_rank(comm, &MyRank);

     /* Memory Allocation for all arrays */
     DisplacementRowptr   = (int *) malloc(Numprocs * sizeof(int));	
     SendCountRowptr 	  = (int *) malloc(Numprocs * sizeof(int));	

     Displacementcolind   = (int *) malloc(Numprocs * sizeof(int));	
     SendCountcolind 	  = (int *) malloc(Numprocs * sizeof(int));	

     Displacementvector   = (int *) malloc(Numprocs * sizeof(int));	
     SendCountvector 	  = (int *) malloc(Numprocs * sizeof(int));	

     TotalRowCount        = (int *) malloc(Numprocs * sizeof(int));	
     TotalColCount 	  = (int *) malloc(Numprocs * sizeof(int));	
     TotalVectorCount 	  = (int *) malloc(Numprocs * sizeof(int));	

     if(MyRank == Root){

        for(iproc = 0; iproc < Numprocs; iproc++){
	   start_row                 = MyFirstRow[iproc];
	   end_row                   = MyLastRow[iproc]; 

	   TotalRowCount[iproc]      = end_row - start_row + 1; 
	   TotalColCount[iproc]      = rowptr[end_row] - rowptr[start_row];
	   TotalVectorCount[iproc]   = end_row - start_row; 

	   DisplacementRowptr[iproc] = start_row;
	   SendCountRowptr[iproc]    = TotalRowCount[iproc]; 

	   Displacementcolind[iproc] = rowptr[start_row];
	   SendCountcolind[iproc]    = TotalColCount[iproc]; 

	   Displacementvector[iproc] = start_row;
	   SendCountvector[iproc]    = TotalVectorCount[iproc]; 
        }		  

	MyTotalRowCount    = TotalRowCount[0];
	MyTotalColCount    = TotalColCount[0]; 
	MyTotalVectorCount = TotalVectorCount[0];
     }
     if(MyRank == Root){
	for(iproc = 1; iproc < Numprocs; iproc++){
	     Destination     = iproc; 
	     Destination_tag = 99; 

	     MPI_Send(&TotalRowCount[iproc],1,MPI_INT,Destination,Destination_tag,comm);
	     MPI_Send(&TotalColCount[iproc], 1, MPI_INT, Destination, Destination_tag, comm);
	     MPI_Send(&TotalVectorCount[iproc], 1, MPI_INT, Destination, Destination_tag, comm);
	}
     }
     else{
	     Source     = Root;
	     Source_tag = 99;

	     MPI_Recv(&MyTotalRowCount, 1, MPI_INT, Source, Source_tag, comm, &status);
	     MPI_Recv(&MyTotalColCount, 1, MPI_INT, Source, Source_tag, comm, &status);
	     MPI_Recv(&MyTotalVectorCount, 1, MPI_INT, Source, Source_tag, comm, &status);
     }

     Myrowptr = (int *)malloc(MyTotalRowCount * sizeof(int));	
     MPI_Scatterv(rowptr, SendCountRowptr, DisplacementRowptr, MPI_INT, 
                  Myrowptr, MyTotalRowCount, MPI_INT, Root, comm); 

     Mycolind = (int *) malloc(MyTotalColCount * sizeof(int));	
     MPI_Scatterv(colind, SendCountcolind, Displacementcolind, MPI_INT, 
                  Mycolind, MyTotalColCount, MPI_INT, Root, comm); 

     start_row = MyFirstRow[MyRank];
     end_row   = MyLastRow[MyRank];

     Myvalues = (double *)malloc(MyTotalColCount * sizeof(double));	
     MPI_Scatterv(values,  SendCountcolind, Displacementcolind, MPI_DOUBLE, 
		           Myvalues, MyTotalColCount, MPI_DOUBLE, Root, comm); 

     free(TotalRowCount), free(TotalColCount), free(TotalVectorCount);
     free (DisplacementRowptr), free(SendCountRowptr);
     free (Displacementcolind), free(SendCountcolind);
     free (Displacementvector), free(SendCountvector);
}
