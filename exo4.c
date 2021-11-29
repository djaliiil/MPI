// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	int numtasks, rank, suiv, preced,i;
	MPI_Init(&argc,&argv);
	MPI_Status status;
	int sended[100], receved[100];
	int b = 100;
	MPI_Datatype vector;

	for(i=0;i<100;i++){
		sended[i]=100-i;
	}
	
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	suiv = (rank+1) % numtasks;
	preced = (rank-1+numtasks) % numtasks;

	if(rank==0) {
		MPI_Type_contiguous(100, MPI_INT, &vector);
		MPI_Type_commit(&vector);
		//MPI_Bcast(sended, 1, vector, 0, MPI_COMM_WORLD);

		MPI_Send(&sended, 1, vector , suiv, 1, MPI_COMM_WORLD);
		printf (" My Rank = %d send : [%d...%d] to Rank : %d \n",rank, sended[0], sended[99], suiv);
		
		MPI_Recv(&receved, 100, MPI_INT, preced, 1, MPI_COMM_WORLD, &status);
		printf (" My rank = %d receive from Rank : %d \n",rank, preced);
		printf("\nIt's OKAY !!!\n\n");
	}
	else{
		MPI_Type_contiguous(100, MPI_INT, &vector);
		MPI_Type_commit(&vector);
		//MPI_Bcast(&receved, 100, MPI_INT,0,MPI_COMM_WORLD);

		MPI_Recv(&receved, 100, MPI_INT, preced, 1, MPI_COMM_WORLD, &status);
		printf (" My rank = %d receive : [%d...%d] from Rank : %d \n",rank, receved[0], receved[99], preced);
		
		MPI_Send(&receved, 1, vector , suiv, 1, MPI_COMM_WORLD);
		printf (" My Rank = %d send : [%d...%d] to Rank : %d \n",rank, receved[0], receved[99], suiv);

	}
	printf("\n");
	MPI_Finalize();
}
