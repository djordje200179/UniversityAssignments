#ifndef MISC_H
#define MISC_H

#include <stdbool.h>

int calc_char_sum(int nums[], int dimension);

int* allocate_int_array(int dimension);
bool* allocate_bool_array(int dimension);

int* clone_int_array(int* array, int dimension);
bool* clone_bool_array(bool* array, int dimension);

int** allocate_matrix(int dimension);
int** deallocate_matrix(int** matrix, int dimension);
#endif /* MISC_H */