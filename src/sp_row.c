
#include "proto.h"

void 
MatrixRowsCommunication(int nsize, int *MyFirstRow, int *MyLastRow, MPI_Comm comm)
{

	int Root = 0;
	int Numprocs,  MyRank, start, end, MyRowCount;
	int i, j, k, n, iproc, totalsize;
	int Destination, Source, Destination_tag, Source_tag;

	int DistributeRows, MyStart, MyLast, remaining_rows;
	int *temp, *LocalProcRows, *LocalFirstRow, *LocalLastRow;
	MPI_Status      status;

	/* MPI - communication calls */
	MPI_Comm_rank(comm, &MyRank);
	MPI_Comm_size(comm, &Numprocs);

	n = 2*Numprocs;
	temp = (int *) malloc ( n * sizeof(int));

	if(MyRank == Root){
		LocalProcRows  = (int *) malloc(Numprocs*sizeof (int));

		/* Finds first and lastof partial rowwise distribution of sparse matrix */
		DistributeRows = nsize/Numprocs;
		remaining_rows = nsize % Numprocs;

		for(i = 0; i < Numprocs; i++)
			LocalProcRows[i] = DistributeRows;

		assert (remaining_rows <= Numprocs);
		for(i=remaining_rows; i>0; i--)
			(LocalProcRows[remaining_rows-i])++ ;

		for(i = 0; i < Numprocs; i++) {
			MyStart = 0;
			for(j = 0; j < i;  j++)
				MyStart += LocalProcRows[j];
			MyLast = MyStart + LocalProcRows[j];
			MyFirstRow[i] = MyStart;
			MyLastRow[i]  = MyLast;

		}

		for(j = 0; j<Numprocs; j++) {
			temp[j] = MyFirstRow[j];
			k = j+Numprocs;
			assert(k < n);
			temp[k] = MyLastRow[j];
		}

		for(iproc = 1 ; iproc < Numprocs ; iproc++){
			Destination     = iproc;
			Destination_tag = 100;
			MPI_Send(temp,2*Numprocs,MPI_INT,Destination,Destination_tag,comm);
		}
	}
	else{
		Source  = Root;
		Source_tag  = 100;

		MPI_Recv(temp, 2*Numprocs, MPI_INT,Source, Source_tag,comm, &status);

		for(j = 0; j < Numprocs; j++)
			MyFirstRow[j] = temp[j];
		for(j = 0; j < Numprocs; j++)
			MyLastRow[j]  = temp[j+Numprocs];

		free(temp);
	}
}
