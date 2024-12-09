#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int nodes;
    char* path;
    int* states;
    int* degree;
    int* queue;
} Matrix;

int* calcDegrees(Matrix* arr) {
    FILE* file = fopen(arr->path, "rb");
    if (!file) {
        printf("Failed to open file %s\n", arr->path);
        exit(1);
    }

    int* degrees = (int*) calloc(arr->nodes, sizeof(int));
    if (!degrees) {
        printf("Memory allocation for degree failed\n");
        fclose(file);
        exit(1);
    }

    int* temp_row = (int*) malloc(arr->nodes * sizeof(int));
    if (!temp_row) {
        printf("Memory allocation for temp_row failed\n");
        fclose(file);
        free(degrees);
        exit(1);
    }

    for (int i = 0; i < arr->nodes; i++) {
        if (fread(temp_row, sizeof(int), arr->nodes, file) != arr->nodes) {
            printf("Failed to read arr data from file\n");
            fclose(file);
            free(temp_row);
            free(degrees);
            exit(1);
        }
        
        for (int j = 0; j < arr->nodes; j++) {
            degrees[j] += temp_row[j];
        }
    }

    fclose(file);
    free(temp_row);

    return degrees;
}

int* createQueue(Matrix* arr) {
    int* queue = (int*) malloc(sizeof(int) * arr->nodes);
    if (!queue) {
        printf("Memory allocation for queue failed\n");
        exit(1);
    }

    for (int i = 0; i < arr->nodes; i++) {
        queue[i] = i;
    }

    for (int i = 0; i < arr->nodes; i++) {
        for (int j = 0; j < arr->nodes - i - 1; j++) {
            if (arr->degree[queue[j]] < arr->degree[queue[j + 1]]) {
                int temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
        }
    }

    return queue;
}

int checkForColoring(Matrix* arr) {
    for (int i = 0; i < arr->nodes; i++) {
        if (arr->states[i] == 0) {
            return 0;
        }
    }
    return 1;
}

Matrix* matrix(char* path) {
    Matrix* arr = (Matrix*) malloc(sizeof(Matrix));
    if (!arr) {
        printf("Memory allocation for arr failed\n");
        exit(1);
    }

    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open file %s\n", path);
        free(arr);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    arr->nodes = (int)sqrt((double)ftell(file) / sizeof(int));
    fclose(file);

    arr->path = path;

    arr->states = (int*) calloc(arr->nodes, sizeof(int));
    if (!arr->states) {
        printf("Memory allocation for states failed\n");
        free(arr);
        exit(1);
    }

    arr->degree = calcDegrees(arr);

    arr->queue = createQueue(arr);

    return arr;
}

void printStates(Matrix* arr) {
    printf("Matrix states:\n");
    for (int i = 0; i < arr->nodes; i++) {
        printf("%d ", arr->states[i]);
    }
    printf("\n");
}

void printDegree(Matrix* arr) {
    printf("Node : Degree\n");
    for (int i = 0; i < arr->nodes; i++) {
        printf("N: %d   D: %d\n", i, arr->degree[i]);
    }
    printf("\n");
}

void printQueue(Matrix* arr) {
    printf("Queue:\n");
    for (int i = 0; i < arr->nodes; i++) {
        printf("%d ", arr->queue[i]);
    }
    printf("\n\n");
}

void printMatrix(Matrix* arr) {
    int* temp_row = (int*) malloc(sizeof(int) * arr->nodes);
    if (!temp_row) {
        printf("Memory allocation for temp_row failed\n");
        exit(1);
    }

    FILE* file = fopen(arr->path, "rb");

    printf("Matrix:\n");
    for (int i = 0; i < arr->nodes; i++) {
        fread(temp_row, sizeof(int), arr->nodes, file);
        for (int j = 0; j < arr->nodes; j++) {
            printf("%d ", temp_row[j]);
        }
        printf("\n");
    }
    printf("\n");
}

int findChromaticNum(Matrix* arr) {
    int *hash = (int *)calloc(arr->nodes + 1, sizeof(int));
    int unique_count = 0;

    for (int i = 0; i < arr->nodes; i++) {
        if (hash[arr->states[i]] == 0) {
            unique_count++;
            hash[arr->states[i]] = 1;
        }
    }

    free(hash);
    return unique_count;
}

int* readRow(Matrix* arr, int row) {
    FILE* file = fopen(arr->path, "rb");
    if (!file) {
        printf("Failed to open file\n", arr->path);
        exit(1);
    }

    fseek(file, row * arr->nodes * sizeof(int), SEEK_SET);

    int* temp = (int*) malloc(sizeof(int) * arr->nodes);
    if (!temp) {
        printf("Memory allocation for temp failed\n");
        exit(1);
    }

    if (fread(temp, sizeof(int), arr->nodes, file) != arr->nodes) {
        printf("Failed to read data from arr\n");
        exit(1);
    }
    fclose(file);

    return temp;
}

void freeMatrix(Matrix* arr) {
    free(arr->states);
    free(arr->degree);
    free(arr->queue);
    free(arr);
}