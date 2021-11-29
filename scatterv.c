#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(int argc, char *argv[]) {

	int numtasks, i, j, k, rank;
	int val;
	int length = atoi(argv[1]);
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Request req;
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Datatype vector, matrix, matrix3, mm;
	int next, prev;
	int (*chunk) = malloc(numtasks * sizeof(*chunk));
	int  (*displs) = malloc(numtasks * sizeof(*displs));
	next = (rank+1) % numtasks;
	prev = (rank-1+numtasks) % numtasks;

	typedef struct {
			int ind;
			double row[length];
	}indexed_row;

	MPI_Datatype index;
	int blocs[2] = {1, length};
	MPI_Datatype types[2];
	MPI_Aint moves[2];
	moves[0] = offsetof(indexed_row, ind);
	moves[1] = offsetof(indexed_row, row);
	types[0] = MPI_INT;
	types[1] = MPI_DOUBLE;
	MPI_Type_struct(2, blocs, moves, types, &index);
	MPI_Type_commit(&index);

	//double old_row[length], new_row[length], old_row_prev[length], old_row_next[length];
	double (*old_row) = malloc(length * sizeof(*old_row));
	double (*new_row) = malloc(length * sizeof(*new_row));
	double (*old_row_prev) = malloc(length * sizeof(*old_row_prev));
	double (*old_row_next) = malloc(length * sizeof(*old_row_next));
	double (*Mat)[length] = malloc(length * sizeof(*Mat));
	double (*MatRes)[length] = malloc(length * sizeof(*MatRes));
	//double Mat[length][length], MatRes[length][length];

	for (i = 0; i < numtasks; i++)
			chunk[i] = (int)(length/numtasks);
	double (*rows)[chunk[rank]] = malloc(length * sizeof(*rows));
	double (*new_rows)[chunk[rank]] = malloc(length * sizeof(*new_rows));
	i = 0;
	while (i < (length%numtasks)){
		chunk[i]++;
		i++;
	}
	displs[0] = 0;
	for (i = 1; i < numtasks; i++)
			displs[i] = displs[i-1] + chunk[i-1];


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


for (int epoch = 0; epoch < 1; epoch++) {
	if (rank == 0){
		printf("\n>>>>>>>>>>>>>  EPOCH :: %d  <<<<<<<<<<<<<\n", epoch);		

		printf("\t================== MATRICE INITIALE ====================\n");
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				printf(" %.2f  ", Mat[i][j]);
			}
			printf("\n\n");
		}
		printf("\t=======================================================\n\n");
	
	}
	

	MPI_Scatterv(MatRes, chunk, displs, MPI_DOUBLE, rows, *chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	for (int i = 0; i < chunk[rank]; i++) {
		printf("\n******  Rank : %d ******  Row : %d *****\n", rank, i+displs[rank]);
		for (int j = 0; j < length; j++) {
			printf(" %.2f ", rows[i][j]);
		}
		printf("\n");
	}
	
	k = displs[rank];
	for (int i = 0; i < chunk[rank]; i++) {		
		if(k == 0 || (k+chunk[rank]) == length-1) {
			printf("******************** %d ***************\n", k);
			for (int l = 0; l < length; l++)			
				new_rows[k][l] = rows[k][l];
		}
		else {
			for (int l = 1; l < length-1; l++)
				new_rows[k][l] = (rows[k][l-1] + rows[k][l+1] + MatRes[k-1][l] + MatRes[k+1][l])/4;
			new_rows[k][0] = rows[k][0];
			new_rows[k][length-1] = rows[k][length-1];			
		}
		k++;
	}
	

	MPI_Gatherv(new_rows, *chunk, MPI_DOUBLE, MatRes, chunk, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
}
	
	MPI_Finalize();

	if(rank == 0) {
		printf("\t================== MATRICE INITIALE ====================\n");
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				printf(" %.2f  ", Mat[i][j]);
			}
			printf("\n\n");
		}
		printf("\t=======================================================\n\n");
	

		FILE *fp;
		fp = fopen("dataMatEnhanced","w");
	
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
	


		printf("\n\n");
	}


}
