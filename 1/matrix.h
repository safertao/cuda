#ifndef MATRIX_H_
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <x86intrin.h>
#include <math.h>

#define typ int
#define L3 12*1024*1024
#define L2 5*1024*1024
#define L1 320*1024

int calculate_matrix_size(int cache_size);
int calculate_block_size(int cache_size);
void mult_matrix_simple();
void mult_matrix_L3();
void mult_matrix_L2();
void mult_matrix_L1();
typ** allocate_matrix(int rows, int cols);
void fill_matrix(typ** array, int rows, int cols);
void print_matrix(typ** array, int rows, int cols);
void free_matrix(typ** array, int rows);


#endif // MAXTRIX_H_