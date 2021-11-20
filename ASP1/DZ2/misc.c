#include "misc.h"
#include <stdlib.h>
#include <string.h>

int calc_char_sum(int nums[], int dimension) {
	int n = dimension * dimension;
	int sum = 0;
	for(int i = 0; i < n; i++)
		sum += nums[i];

	return sum / dimension;
}

int* allocate_int_array(int dimension) {
	int* array = (int*)calloc(dimension, sizeof(int));

	return array;
}

bool* allocate_bool_array(int dimension) {
	bool* array = (bool*)calloc(dimension, sizeof(bool));

	return array;
}

int* clone_int_array(int* array, int dimension) {
	int* new_array = allocate_int_array(dimension);
	memcpy(new_array, array, dimension * sizeof(int));

	return new_array;
}

bool* clone_bool_array(bool* array, int dimension) {
	bool* new_array = allocate_bool_array(dimension);
	memcpy(new_array, array, dimension * sizeof(bool));

	return new_array;
}

int** allocate_matrix(int dimension) {
	int** matrix = (int**)calloc(dimension, sizeof(int*));

	if(!matrix)
		return NULL;

	for(int i = 0; i < dimension; i++) {
		matrix[i] = (int*)calloc(dimension, sizeof(int));

		if(!matrix[i])
			return NULL;
	}

	return matrix;
}

int** deallocate_matrix(int** matrix, int dimension) {
	for(int i = 0; i < dimension; i++)
		free(matrix[i]);

	free(matrix);

	return NULL;
}