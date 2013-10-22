#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define MATRIX_1 "matrix1.txt"
#define MATRIX_2 "matrix2.txt"
#define PRODUCT  "product.txt"

#define MATRIX_SIZE 16

int main(int argc, char *argv[])
{
	puts("\e[0;34m==>\e[0m Reading in values from the matrix files...");

	double matrix1[MATRIX_SIZE][MATRIX_SIZE],
	       matrix2[MATRIX_SIZE][MATRIX_SIZE],
	       real_product[MATRIX_SIZE][MATRIX_SIZE],
	       calc_product[MATRIX_SIZE][MATRIX_SIZE];

	{
		FILE *matrix1_file = fopen(MATRIX_1, "r"),
		     *matrix2_file = fopen(MATRIX_2, "r"),
		     *product_file = fopen(PRODUCT,  "r");

		for (int i = 0; i < MATRIX_SIZE; ++i)
		{
			for (int j = 0; j < MATRIX_SIZE; ++j)
			{
				fscanf(matrix1_file, "%lf", &matrix1[i][j]);
				fscanf(matrix2_file, "%lf", &matrix2[i][j]);
				fscanf(product_file, "%lf", &real_product[i][j]);
			}
		}

		fclose(matrix1_file);
		fclose(matrix2_file);
		fclose(product_file);
	}

	return 0;
}
