// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	
	static long num_steps=1000000;
	
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Datatype vector;

	double x=0.0, step, pi, sum=0.0, sumTotal;
	int receved, epochs;
	int numtasks, i, rank;
	double sumVect[numtasks];

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	step = 1.0 / (double)num_steps;
	
	if(rank==0) {
		step = 1.0 / (double)num_steps;
		MPI_Type_contiguous(numtasks, MPI_INT, &vector);
		MPI_Type_commit(&vector);
		
		epochs = num_steps/numtasks;
		MPI_Bcast(&epochs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf (" My Rank : %d , send nbrIters : %d\n ", rank, epochs);
		for(i=rank; i<epochs; i++) {
			x = (i+0.5)*step;
			sum += 4.0/(1.0+x*x);
		}
	}

	else {
		MPI_Bcast(&epochs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf (" My Rank : %d , receive nbrIters : %d\n", rank, epochs);
		int start, end;
		start = rank * epochs;
		end = start + epochs;
		printf("\n rank :: %d  start : %d  end: %d \n", rank, start, end);
		for(i=start; i<end; i++) {
			x = (i+0.5)*step;
			sum += 4.0/(1.0+x*x);
		}
		
	}
	sumTotal = 0.0;
	printf(" My Rank : %d , I had calculate (sum : %f) \n", rank, sum);
	MPI_Gather(&sum, 1, MPI_DOUBLE, &sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Reduce(&sum, &sumTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	

	if(rank==0) {
		
		printf("\t sumTotal : %f\n", sumTotal);
		printf (" My Rank : %d , Pi was calculated : \n", rank);
		pi = (1/(double)num_steps) * sumTotal;
		printf("\t Pi = %f \n",pi);
	}

	MPI_Finalize();
	printf("\n\n");
}
