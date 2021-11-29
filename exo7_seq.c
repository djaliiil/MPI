#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	int i, j, k;
	int length = 1000;
	double (*Mat)[length] = malloc(length * sizeof(*Mat));
	double (*MatRes)[length] = malloc(length * sizeof(*MatRes));
	double (*MatIm)[length] = malloc(length * sizeof(*MatIm));
	//double Mat[length][length], MatRes[length][length], MatIm[length][length];
	printf("************\n");

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

for (int epoch = 0; epoch < 1000; epoch++) {
		printf("\n>>>>>>>>>>>>>  EPOCH :: %d  <<<<<<<<<<<<<\n", epoch);

		for (i = 1; i < length-1; i++) {
			for (j = 1; j < length-1; j++) {
				MatIm[i][j] = (MatRes[i][j-1] + MatRes[i][j+1] + MatRes[i-1][j] + MatRes[i+1][j]) / 4;
			}
			MatIm[i][0] = MatRes[i][0];
			MatIm[i][length-1] = MatRes[i][length-1];
		}
		for (i = 1; i < length-1; i++) {
			for (j = 1; j < length-1; j++) {
				MatRes[i][j] = MatIm[i][j];
			}
		}
}

	printf("\n\n");

		printf("================= MATRICE INITIALE =====================\n");
		for (i = 0; i < length; i++) {
			for (j = 0; j < length; j++) {
				printf(" %.2f  ", Mat[i][j]);
			}
			printf("\n");
		}
		printf("========================================\n");


		FILE *fp;
		fp = fopen("dataMatSequencial","w");
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
