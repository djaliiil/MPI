// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	
	int numtasks, i, j, k, rank;
	int val;
	
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Datatype vector, matrix;
	int row[numtasks], rowRes[numtasks];
	int A[numtasks][numtasks], B[numtasks][numtasks], C[numtasks][numtasks];

	MPI_Type_contiguous(numtasks, MPI_INT, &vector);
	MPI_Type_commit(&vector);

	MPI_Type_contiguous(numtasks*numtasks, MPI_INT, &matrix);
	MPI_Type_commit(&matrix);

	if (rank == 0) {
		k = 0;
		for (i = 0; i < numtasks; i++) {
			for (j = 0; j < numtasks; j++)
			{
				A[i][j] = 1;
				B[i][j] = 1;
			}
		}
		MPI_Bcast(&B, 1, matrix, 0, MPI_COMM_WORLD);
	}
	
	MPI_Bcast(&B, numtasks, vector, 0, MPI_COMM_WORLD);
	MPI_Scatter(&A, 1, vector, &row, 1, vector, 0, MPI_COMM_WORLD);
	printf (" My Rank : %d , I've receive One Row \n ", rank);	

	for (j = 0; j < numtasks; j++) {
		val = 0;
		for (k = 0; k < numtasks; k++){
			val += row[j] * B[k][j];
		}	
		rowRes[j] = val;
	}

	MPI_Gather(&rowRes, 1, vector, &C[rank], 1, vector, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if(rank == 0){
		printf("\n");
		for (i = 0; i < numtasks; i++) {
			printf("\t");
			for (j = 0; j < numtasks; j++) {
				printf(" %d ", C[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();
	printf("\n\n");
}
