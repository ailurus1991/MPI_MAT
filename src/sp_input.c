#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *fp;
void RandomPermute (double *perm, int nvtxs, int row_no);

void swap (double *a, double *b);

void ReadSparseInput (int *nvtxs, int *nsize , double **vector, 
		      int **rowptr, int **colind, double **values)
{
        int    i,j;
	int    MaxColNonZeros;
	int    irow, icol, index, ColIndex;
	int    TotalSize, TotalRowMatrixSize, nonzero;

	int     *local_rowptr;
	int     *local_colind;
	int     nvtxs_local, nlocal_size,tmp=0,vsize;

	float   SPARSITY;
	double  *local_values;  
	double  *local_vector; 
	double  *local_output;

	double  num, *randvect, RandNo;
        struct  timeval startTime, endTime;
        gettimeofday(&startTime, NULL);
        printf("start time is : %f\n", startTime);
	if ((fp = fopen ("../data/mdat_sparse.inp", "r")) == NULL){
		printf (" Can't open the Matrix Data file, terminating the application \n");
	}
	fscanf(fp,"%f",&SPARSITY);
	fscanf(fp,"%d",&nlocal_size);
	*nsize = nlocal_size ; 
	
	MaxColNonZeros    =  SPARSITY ; 
	TotalSize         = nlocal_size* (MaxColNonZeros ) ; 

	local_rowptr 	= (int *) malloc ( (nlocal_size+1) * sizeof(int));
	local_colind    = (int *) malloc (TotalSize * sizeof(int));
	local_values    = (double *) malloc (TotalSize * sizeof(double));
	local_vector    = (double *) malloc (nlocal_size * sizeof(double));
	randvect        = (double *) malloc (nlocal_size*sizeof(double));
	
    for(index=0; index<nlocal_size; index++)
	    randvect[index] = 0.0;

        nvtxs_local = 0;
	local_rowptr[0] = 0;
	for(irow=0; irow< nlocal_size; irow++){
	    for(index=0; index<nlocal_size; index++){
	        fscanf(fp, "%lf",&randvect[index]);
	    }

	    for(icol=0; icol <nlocal_size; icol++) {
		if(randvect[icol] >= 0.00001){
		    local_colind[nvtxs_local] = icol;
		    local_values[nvtxs_local] = randvect[icol];
		    nvtxs_local++;
		}
	    }
	   
	    local_rowptr[irow+1] = nvtxs_local;
	    tmp=nvtxs_local;		
	}


	fclose(fp);

	if ((fp = fopen ("../data/vdat_sparse.inp", "r")) == NULL){
	     printf(" Can't open the Vector Data file, terminating the application \n");
	     exit(-1);
	}

	fscanf(fp, "%d", &vsize);
	for(index=0; index<nlocal_size; index++){
	    fscanf(fp, "%lf", &local_vector[index]);
	}
	fclose(fp);

	free(randvect);

	*nvtxs = nvtxs_local ;

	*vector = local_vector;
	*colind = local_colind;
	*rowptr = local_rowptr; 
	*values = local_values;
        printf("end time is %f\n", endTime);
        gettimeofday(&endTime, NULL);
        printf("INPUT (microseconds): %lu\n", (endTime.tv_sec-startTime.tv_sec)*1000000+endTime.tv_usec-startTime.tv_usec);

}
