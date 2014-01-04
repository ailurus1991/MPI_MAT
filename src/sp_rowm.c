void AllocateInt(int **MyRows, int Numprocs)
{

	int *local;

	local  = (int *)malloc(Numprocs * sizeof(int));

	*MyRows   = local;

}
