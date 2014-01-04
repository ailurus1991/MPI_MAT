#include <math.h>
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h> 

#include "mpi.h"
#include "proto.h"


extern int    *Mycolind;
extern int    *Myrowptr; 
extern double *Myvalues; 
extern double *Myvector; 
extern int    MyTotalRowCount, MyTotalColCount, MyTotalVectorCount;

main(int argc, char **argv) 
{
 int         Root = 0;
 int         i, j, ResultsMatch = 1;
 int         Numprocs, MyRank, NoofNodes;
 int         Myrows, Mycols, Myptr;
 int         namelen;
 double      *global_vector, *global_output;
 double      *Myoutput, *serial_output;
 char        processor_name[100];
 MPI_Status  status;

 /* First declare the variables & use them as external variables */
 int      nvtxs;         /* (Total number of non zero entries. */
 int      nsize;         /* (Total number of rows of the sparse matrix. */ 
 int      *MyFirstRow ;  /* First row on each processes */
 int      *MyLastRow  ;  /* Last row on each processes */
 int      *rowptr;       /* CSR rowptr */
 int      *colind;       /* CSR Column index */
 double   *values;       /* CSR Sparse matrix non-zero entries */
 double   *vector; /* Vector values in the sparse matrix-vector multiplication */
 double   *VectorOnProc, *My_Vector, *My_values; 

 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
 MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
 MPI_Get_processor_name(processor_name, &namelen); 
 printf("Process %d of %d is on %s\n", MyRank, Numprocs, processor_name);     
 if(MyRank == Root){
    ReadSparseInput(&nvtxs,&nsize, &vector, &rowptr, &colind, &values);
 }

 MPI_Barrier(MPI_COMM_WORLD);

 MPI_Bcast(&nsize, 1, MPI_INT, Root, MPI_COMM_WORLD);
 MPI_Bcast(&nvtxs, 1, MPI_INT, Root, MPI_COMM_WORLD);

 AllocateInt(&MyFirstRow, Numprocs);
 AllocateInt(&MyLastRow, Numprocs);  

/*Root sends info about first and last row on each process to All process */
MatrixRowsCommunication(nsize, MyFirstRow, MyLastRow, MPI_COMM_WORLD); 

 SparseMatrixScatter( nsize, nvtxs, MyFirstRow, MyLastRow, vector,
		      rowptr, colind, values, MPI_COMM_WORLD);

 if(MyRank != Root)
    vector = (double *) malloc(nsize * sizeof(double));
 MPI_Bcast(vector, nsize, MPI_DOUBLE, Root, MPI_COMM_WORLD);


 Myrows   = MyTotalVectorCount;
 Myptr    = MyTotalRowCount;
 Mycols   = MyTotalColCount;

 Myoutput = (double *) malloc(nsize * sizeof(double));
 SerialSparseMatrixVector(Myrows, Mycols, Myrowptr, Mycolind, 
 MyFirstRow, MyLastRow, Myvalues, vector, Myoutput, MPI_COMM_WORLD);

  
 if(MyRank == Root)
    global_output = (double *)malloc(nsize*sizeof(double));

OutputGather(nsize, Myrows, MyTotalVectorCount, MyFirstRow, MyLastRow,
	      Myoutput, global_output, MPI_COMM_WORLD);

 if(MyRank == Root){
    for(i = 0; i < nsize; i++) 
        printf("FinalResult[%d] =%12lf\n", i, global_output[i]);
 }
 MPI_Finalize(); 

}

