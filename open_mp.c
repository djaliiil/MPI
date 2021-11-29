
#include "mpi.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
  
  int numtasks,rank;
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int haute[numtasks+2],bas[numtasks+2],ligne[numtasks+2];
  int matric[numtasks+2][numtasks+2];
  int matric_res[numtasks][numtasks+2];
  int ligne_finale[numtasks+2];
  if (rank == 0) {     
     for (int i = 0; i < numtasks+2; ++i)
     {
      for (int j = 0; j < numtasks+2; ++j)
      {
         matric[i][j]=i;
      }
     }
       
  }
   MPI_Scatter(&matric[rank],numtasks+2,MPI_INT,&haute,numtasks+2,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Scatter(&matric[rank+1],numtasks+2,MPI_INT,&ligne,numtasks+2,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Scatter(&matric[rank+2],numtasks+2,MPI_INT,&bas,numtasks+2,MPI_INT,0,MPI_COMM_WORLD);

   int k=0;
   ligne_finale[0]=ligne[0];
   ligne_finale[numtasks+1]=ligne[numtasks+1];
   for (int i = 1; i < numtasks+1; ++i)
   {
   	  ligne_finale[i]=haute[k]+haute[k+1]+haute[k+2] + bas[k]+bas[k+1]+bas[k+2] + ligne[k]+ligne[k+1]+ligne[k+2];
   	  k++;
   }
   
   for (int i = 0; i < numtasks+2; ++i)
   {
   	printf("my rank %d", rank );
   	printf("*%d  \n",ligne_finale[i]);
   }
 
   MPI_Gather(&ligne_finale,numtasks+2, MPI_INT, &matric[rank+1],numtasks+2, MPI_INT, 0, MPI_COMM_WORLD);
   

   if (rank==0)
   {

   	    for (int i = 0; i < numtasks+2; i++)
    	{
    	    printf("\t");
    		for (int j = 0; j < numtasks+2; j++)
    		{
    			printf("%d ", matric[i][j]);
    		}
    	    printf("\n");
    	}
   	 
   }

   MPI_Finalize();


}