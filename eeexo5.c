// required MPI include file

#include "mpi.h"

#include <stdio.h>

static long num_steps = 1000000;



int main(int argc, char *argv[]) {

	MPI_Init(&argc,&argv);

	double somme, step = 1/(double)num_steps;

	double x, sum = 0;

	int rank,numtasks,receved,i;

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	double ss;

    int pas=num_steps/numtasks;

	if (rank==0)

	{

		MPI_Bcast(&pas, 1, MPI_INT, 0, MPI_COMM_WORLD);

		for(i = rank; i < pas; i++)

			{

				x = (i + 0.5) * step;

				sum += 4.0/(1+x*x);

			}



	}else

	{

		MPI_Bcast(&receved, 1, MPI_INT,0,MPI_COMM_WORLD);

		printf("=====================\n");

		printf (" My rank= %d  and i recive %d  \n",rank,receved);

		for(i = receved*rank; i < receved*rank+receved; i++)

			{

				x = (i + 0.5) * step;

				sum += 4.0/(1+x*x);

			}

		printf("start from %d   end  in %d \n", receved*rank,receved*rank+receved);

	}

    //MPI_Gather(&sum,1,MPI_DOUBLE,ss,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

    MPI_Reduce(&sum,&ss,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);



    if (rank==0)

    {

    	printf(" la soumme est %f \n ",step * ss);

    }



	MPI_Finalize();

}
