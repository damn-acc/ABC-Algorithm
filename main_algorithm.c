#include <stdio.h>

#include "struct_matrix.c"
#include "struct_hive.c"

#define PATH "matrix2.bin"
#define SCOUTS 2
#define FORAGERS 6

int main() {
    Matrix* arr = matrix(PATH);
    printMatrix(arr);
    printDegree(arr);
    printQueue(arr);
    printStates(arr);

    Hive* hive = beeHive(SCOUTS, FORAGERS);
    hive->arr = arr;

    freeMatrix(arr);
    freeHive(hive);

    return 0;
}