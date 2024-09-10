#include "matrix.h"

#define L3 12*1024*1024
#define L2 5*1024*1024
#define L1 320*1024

// L3 - 12 МБ, L2 - 5 МБ, L1 - 320 КБ

typ** A;
typ** B;
typ** C;
int rowsA;
int colsA;
int colsB;
int rowsB;
int block_cols;
int block_rows;
int block_size_L3;
int block_size_L2;
int block_size_L1;

int block_size;

void init()
{
    srand(time(NULL));
    int matrix_size = calculate_matrix_size(L3);
    block_rows = calculate_block_size(L3);

    block_size_L1 = calculate_block_size(L1);
    block_size_L2 = calculate_block_size(L2);
    block_size_L3 = calculate_block_size(L3);

    block_size = calculate_block_size(L3);

    rowsA = block_rows;
    colsB = rowsA;

    int temp = L3 / 4 / 4 / rowsA;
    int remainder = temp % 64;
    temp -= remainder;
    block_cols = temp;
    
    temp *= 4;

    colsA = temp;
    rowsB = colsA;
    
    // printf("matrix size: %d\n", matrix_size);
    printf("rowsA: %d\n", rowsA);
    printf("colsA: %d\n", colsA);
    printf("matrix size: %d\n", colsA*rowsA*sizeof(typ));

    // printf("block size:  %d\n", block_size);

    A = allocate_matrix(rowsA, colsA);
    B = allocate_matrix(rowsB, colsB);
    C = allocate_matrix(rowsA, colsB);
    fill_matrix(A, rowsA, colsA);
    fill_matrix(B, rowsB, colsB);
}

double profile_mult(void (*mult_func)())
{
    unsigned long long start = __rdtsc();
    mult_func(A, B, C, rowsA, colsA, colsB);
    unsigned long long end = __rdtsc();
    unsigned long long cycles = end - start;
    double frequency = 2.8e9; // Частота в Гц
    double time_in_ms = (cycles / frequency) * 1000; // Переводим в миллисекунды
    return time_in_ms;
}


int main()
{
    init();

    double first_time = profile_mult(mult_matrix_simple);
    printf("Первый способ: %.3f секунд\n", first_time/1000);

    print_matrix(C, 1, 5);
    free_matrix(C, rowsA);
    C = allocate_matrix(rowsA, colsB);
    
    double second_time = profile_mult(mult_matrix_L3);
    printf("Второй способ: %.3f секунд\n", second_time/1000);

    print_matrix(C, 1, 5);
    free_matrix(C, rowsA);

    printf("Блочный алгоритм быстрее в %.2f раз\n", first_time/second_time);
    return 0;
}