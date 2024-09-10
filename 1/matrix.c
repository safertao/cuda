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

extern int block_size_L3_row;
extern int block_size_L3_col;
extern int block_size_L2_row;
extern int block_size_L2_col;
extern int block_size_L1_row;
extern int block_size_L1_col;

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
    for (int i = 0; i < rowsA; i += block_size_L3_row) 
    {
        for (int k = 0; k < colsA; k += block_size_L3_col) 
        {
            for (int j = 0; j < colsB; j += block_size_L3_col) 
            {
                for (int ib = i; ib < i + block_size_L3_row && ib < rowsA; ++ib) 
                {
                    for (int kb = k; kb < k + block_size_L3_col && kb < colsA; ++kb) 
                    {
                        for (int jb = j; jb < j + block_size_L3_col && jb < colsB; ++jb) 
                        {
                            C[ib][jb] += A[ib][kb] * B[kb][jb];
                        }
                    }
                }
            }
        }
    }
}

void mult_matrix_L2() 
{
    for (int i = 0; i < rowsA; i += block_size_L2_row) 
    {
        for (int k = 0; k < colsA; k += block_size_L2_col) 
        {
            for (int j = 0; j < colsB; j += block_size_L2_col) 
            {
                for (int ib = i; ib < i + block_size_L2_row && ib < rowsA; ++ib) 
                {
                    for (int kb = k; kb < k + block_size_L2_col && kb < colsA; ++kb) 
                    {
                        for (int jb = j; jb < j + block_size_L2_col && jb < colsB; ++jb) 
                        {
                            C[ib][jb] += A[ib][kb] * B[kb][jb];
                        }
                    }
                }
            }
        }
    }
}

void mult_matrix_L1() 
{
    for (int i = 0; i < rowsA; i += block_size_L1_row) 
    {
        for (int k = 0; k < colsA; k += block_size_L1_col) 
        {
            for (int j = 0; j < colsB; j += block_size_L1_col) 
            {
                for (int ib = i; ib < i + block_size_L1_row && ib < rowsA; ++ib) 
                {
                    for (int kb = k; kb < k + block_size_L1_col && kb < colsA; ++kb) 
                    {
                        for (int jb = j; jb < j + block_size_L1_col && jb < colsB; ++jb) 
                        {
                            C[ib][jb] += A[ib][kb] * B[kb][jb];
                        }
                    }
                }
            }
        }
    }
}

// void mult_matrix_L3_L2() 
// {
//     for (int i = 0; i < rowsA; i += block_size_L3_row) 
//     {
//         for (int k = 0; k < colsA; k += block_size_L3_col) 
//         {
//             for (int j = 0; j < colsB; j += block_size_L3_col) 
//             {
//                 for (int ib = i; ib < i + block_size_L3_row && ib < rowsA; ib += block_size_L2_row) 
//                 {
//                     for (int kb = k; kb < k + block_size_L3_col && kb < colsA; kb += block_size_L2_col) 
//                     {
//                         for (int jb = j; jb < j + block_size_L3_col && jb < colsB; jb += block_size_L2_col) 
//                         {
//                             for (int iib = ib; iib < ib + block_size_L2_row && iib < rowsA; ++iib) 
//                             {
//                                 for (int kkb = kb; kkb < kb + block_size_L2_col && kkb < colsA; ++kkb) 
//                                 {
//                                     for (int jjb = jb; jjb < jb + block_size_L2_col && jjb < colsB; ++jjb) 
//                                     {
//                                         C[iib][jjb] += A[iib][kkb] * B[kkb][jjb];
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }


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