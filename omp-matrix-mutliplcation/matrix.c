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

	int block_size  = 4;

	if (argc > 1)
	{
		block_size = atoi(argv[1]);

		if (MATRIX_SIZE % block_size != 0)
		{
			printf("\e[0;31m==> %d Block size is not divisible into the matrix\n", block_size);
			exit(1);
		}
	}

	// Check that our calculated value matches the real product
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (calc_product[i][j] != real_product[i][j])
			{
				puts("\e[0;31m==> Calculated product differs from real product!");
				exit(1);
			}
		}
	}

	puts("\e[0;32m==>\e[0m Calculated product matches real product");
	puts("\e[0;32m==>\e[0m Calculated product matches real product");

	return 0;
}
