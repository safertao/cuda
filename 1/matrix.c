#include "matrix.h"

extern typ** A;
extern typ** B;
extern typ** C;
extern int rowsA;
extern int colsA;
extern int colsB;
extern int rowsB;
extern int block_cols;
extern int block_rows;
extern int block_size_L3;
extern int block_size_L2;
extern int block_size_L1;

extern int block_size;

int calculate_matrix_size(int cache_size)
{
    int temp_size = sqrt(cache_size / 4);
    int remainder = 64 - (temp_size % 64);
    return temp_size + remainder;
}

int calculate_block_size(int cache_size)
{
    int temp_size = sqrt(cache_size / 4 * 0.9 / 3);
    int remainder = temp_size % 64;
    return temp_size - remainder;
}

void mult_matrix_simple()
{
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mult_matrix_L3() 
{
    for (int i = 0; i < rowsA; i += block_size) 
    {
        for (int k = 0; k < colsA; k += block_size) 
        {
            for (int j = 0; j < colsB; j += block_size) 
            {
                for (int ib = i; ib < i + block_size && ib < rowsA; ++ib) 
                {
                    for (int kb = k; kb < k + block_size && kb < colsA; ++kb) 
                    {
                        for (int jb = j; jb < j + block_size && jb < colsB; ++jb) 
                        {
                            C[ib][jb] += A[ib][kb] * B[kb][jb];
                        }
                    }
                }
            }
        }
    }
}

void mult_matrix_L3_optimized() 
{
    int block_size_L1 = 320; // Размер блока для L1 кэша
    int block_size_L2 = 5120; // Размер блока для L2 кэша

    for (int i = 0; i < rowsA; i += block_size_L2) 
    {
        for (int k = 0; k < colsA; k += block_size_L2) 
        {
            for (int j = 0; j < colsB; j += block_size_L2) 
            {
                for (int ii = i; ii < i + block_size_L2 && ii < rowsA; ii += block_size_L1) 
                {
                    for (int kk = k; kk < k + block_size_L2 && kk < colsA; kk += block_size_L1) 
                    {
                        for (int jj = j; jj < j + block_size_L2 && jj < colsB; jj += block_size_L1) 
                        {
                            for (int ib = ii; ib < ii + block_size_L1 && ib < rowsA; ++ib) 
                            {
                                for (int kb = kk; kb < kk + block_size_L1 && kb < colsA; ++kb) 
                                {
                                    for (int jb = jj; jb < jj + block_size_L1 && jb < colsB; ++jb) 
                                    {
                                        C[ib][jb] += A[ib][kb] * B[kb][jb];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

typ** allocate_matrix(int rows, int cols)
{
    typ** array = (typ**)calloc(rows, sizeof(typ*));
    if (!array) 
    {
        fprintf(stderr, "error allocating row.\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++) 
    {
        array[i] = (typ*)calloc(cols, sizeof(typ));
        if (!array[i]) 
        {
            fprintf(stderr, "error allocating column");
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return NULL;
        }
    }
    return array;
}

void fill_matrix(typ** array, int rows, int cols)
{
    for (int i = 0; i < rows; i++) 
        for(int j = 0; j < cols; j++)
            array[i][j] = rand() % 51;
}

void print_matrix(typ** array, int rows, int cols)
{
    for (int i = 0; i < rows; i++) 
    {
        for(int j = 0; j < cols; j++)
            printf("%4d    ", array[i][j]);
        printf("\n");
    }
    printf("\n");
}

void free_matrix(typ** array, int rows)
{
    for (int i = 0; i < rows; i++) 
    {
        free(array[i]);
    }
    free(array);
}