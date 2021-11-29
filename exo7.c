#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	int numtasks, i, j, k, rank;
	int val;
	int chunk[numtasks];
	int length = atoi(argv[1]);
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int next, prev;
	next = (rank+1) % numtasks;
	prev = (rank-1+numtasks) % numtasks;

	printf("\n\n\n\t\%d\n\n\n", length);
	//double old_row[length], new_row[length], old_row_prev[length], old_row_next[length];
	double (*old_row) = malloc(length * sizeof(*old_row));
	double (*new_row) = malloc(length * sizeof(*new_row));
	double (*old_row_prev) = malloc(length * sizeof(*old_row_prev));
	double (*old_row_next) = malloc(length * sizeof(*old_row_next));
	double (*Mat)[length] = malloc(length * sizeof(*Mat));
	double (*MatRes)[length] = malloc(length * sizeof(*MatRes));
	//double Mat[length][length], MatRes[length][length];

	if (rank == 0) {
		k = 0;
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				if ((i != 0) && (i != length-1) && (j != 0) && (j != length-1)) {
					Mat[i][j] = 5.00;
					MatRes[i][j] = 5.00;
				}
				else {
					if((i == 0) || (i == length-1)) {
						if((j == 0) || (j == length-1)){
							Mat[i][j] = 3.00;
							MatRes[i][j] = 3.00;
						}
						else {
							if (i == 0) {
								Mat[i][j] = 7.00;
								MatRes[i][j] = 7.00;
							}
							if(i == length-1) {
								Mat[i][j] = 9.00;
								MatRes[i][j] = 9.00;
							}
						}
					}
					else {
						if (j == 0)	{
							Mat[i][j] = 6.00;
							MatRes[i][j] = 6.00;
						}
						if (j == length-1) {
							Mat[i][j] = 4.00;
							MatRes[i][j] = 4.00;
						}
					}
				}
			}
		}

	}

for (int epoch = 0; epoch < 1000; epoch++) {
		if (rank == 0) {
			printf("\n>>>>>>>>>>>>>  EPOCH :: %d  <<<<<<<<<<<<<\n", epoch);
		}
		MPI_Scatter(MatRes, length, MPI_DOUBLE, old_row, length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

		MPI_Send(old_row, length, MPI_DOUBLE, next, 1, MPI_COMM_WORLD);
		MPI_Send(old_row, length, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD);

		MPI_Recv(old_row_prev, length, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(old_row_next, length, MPI_DOUBLE, next, 1, MPI_COMM_WORLD, &status);

		for (i = 1; i < length-1; i++) {
				new_row[i] = (old_row[i-1] + old_row[i+1] + old_row_prev[i] + old_row_next[i]) / 4;
		}
		new_row[0] = old_row[0];
		new_row[length-1] = old_row[length-1];

	MPI_Gather(new_row, length, MPI_DOUBLE, MatRes, length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//printf (" My Rank : %d , I've send One Row \n ", rank);
	MPI_Barrier(MPI_COMM_WORLD);
}
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
	printf("\n\n");

	if(rank == 0) {
		printf("\t================== MATRICE INITIALE ====================\n");
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				printf(" %.2f  ", Mat[i][j]);
			}
			printf("\n\n");
		}
		printf("\t=======================================================\n\n");
	}

	FILE *fp;
	fp = fopen("dataMat","w");
	if(rank == 0) {
		printf("\t================== MATRICE FINALE =====================\n");
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				printf(" %.2f  ", MatRes[i][j]);
				fprintf(fp, " %.2f  ", MatRes[i][j]);
			}
			fprintf(fp,"\n");
			printf("\n\n");
		}
		printf("\t==================================================\n");
		fclose(fp);
  printf("\n  Data stored !\n");
	}

	printf("\n\n");
}
