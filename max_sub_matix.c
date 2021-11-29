// required MPI include file
#include "mpi.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
	
	int numtasks, i, j, k, rank;
	int len, som, x, somTotal;
	
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Datatype vector, val, matrix;
	
	len = (int)(sqrt(numtasks));
	int mat[numtasks][numtasks], subMat[len][len], matRes[len][len], c[numtasks];

	MPI_Type_vector(len, len, len, MPI_INT, &matrix);
	MPI_Type_commit(&matrix);

	MPI_Type_vector(1, 1, 1, MPI_INT, &val);
	MPI_Type_commit(&val);

	if (rank == 0) {
		k = 0;
		for (i = 0; i < numtasks; i++) {
			for (j = 0; j < numtasks; j++) {
				x = (i*i) + (j*j);
				mat[i][j] = (int)(sqrt(9));			
			}
		}		
	}

	som = 0;
	MPI_Scatter(&mat, 1, matrix, &subMat, 1, matrix, 0, MPI_COMM_WORLD);
	printf (" My Rank : %d , I've receive One Matrix \n ", rank);	

	if(rank == 0){
		printf("================= Matrice Initiale =================\n");
		for (i = 0; i < numtasks; i++) {
			printf("\t");
			for (j = 0; j < numtasks; j++) {
				printf(" %d ", mat[i][j]);
			}
			printf("\n");
		}
	}

		printf("================ %d =================\n", rank);
		printf (" My Rank : %d , I've receive One Matrix \n ", rank);	
		som = 0;
		for (i = 0; i < len; i++) {
			printf("\t");
			for (j = 0; j < len; j++) {
				som += subMat[i][j];
				printf(" %d ", subMat[i][j]);
			}
			printf("\n");			
		}
		printf("\t Somme : %d\n", som);
		printf("\n");
		int aa = som;
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(&matRes, 1, val, &aa, 1, val, 0, MPI_COMM_WORLD);
	MPI_Reduce(&som, &somTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0){		
		//printf("\t \t ** %d **\n", matRes[2][2]);
		printf("\n\n");
		printf("\t Somme Totale   >>>>>>>>>>>>>   %d\n", somTotal);
		printf("\n");
	}
	

	MPI_Finalize();
	printf("\n");

}
