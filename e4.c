// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	int numtasks, rank, suiv, preced;
	MPI_Init(&argc,&argv);
	MPI_Status status;
	int a,receved;

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	suiv = (rank+1) % numtasks;
	preced = (rank-1+numtasks) % numtasks;


	if (rank == 0) {
		a = 15;
		printf("==================================\n");
		MPI_Bcast(&a, 1, MPI_INT, 1, MPI_COMM_WORLD);
		printf ("My Rank : %d send : %d\n ", rank, a);
	}
	else {
		MPI_Bcast(&a, 1, MPI_INT, 1, MPI_COMM_WORLD);
		printf("My Rank : %d , receved : %d\n", rank, receved);
	}

	MPI_Finalize();
}
