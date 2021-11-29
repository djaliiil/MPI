// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	int numtasks, rank, suiv, preced;
	MPI_Init(&argc,&argv);MPI_Status status;
	int a,receved;

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	suiv = (rank+1) % numtasks;
	preced = (rank-1+numtasks) % numtasks;


	if(rank==0) {
		a=10;
		// printf("%d",a);
		MPI_Send(&a,1,MPI_INT,suiv,1,MPI_COMM_WORLD);
		printf (" My rank = %d envoyé \n",rank);
		MPI_Recv(&receved,1,MPI_INT,preced,1,MPI_COMM_WORLD,&status);
		printf (" My rank = %d reçu \n",rank);
	}

	else {
		MPI_Recv(&receved,1,MPI_INT,preced,1,MPI_COMM_WORLD,&status);
		printf (" My rank = %d reçu \n",rank);
		MPI_Send(&a,1,MPI_INT,suiv,1,MPI_COMM_WORLD);
		printf (" My rank = %d envoyé \n",rank);
	}
		
	MPI_Finalize();
}
