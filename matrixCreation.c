#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NODES_NUM 200
#define FILE_PATH "matrix1.bin"

int** generateAdjacencyMatrix(int nodes_num);
void printAdjacencyMatrix(int** adj_matrix, int nodes_num);
void saveAdjacencyMatrixToBinary(const char* filename, int** adj_matrix, int nodes_num);
int** readMatrixFromBinary(const char* filename, int nodes_num);

int main() {
    srand(time(NULL));
    int** adj_matrix = generateAdjacencyMatrix(NODES_NUM);
    // int** adj_matrix = readMatrixFromBinary(FILE_PATH, NODES_NUM);
    printAdjacencyMatrix(adj_matrix, NODES_NUM);
    saveAdjacencyMatrixToBinary(FILE_PATH, adj_matrix, NODES_NUM);
    return 0;
}

int** generateAdjacencyMatrix(int nodes_num) {
    int** matrix = (int**) calloc(nodes_num, sizeof(int*));
    for (int i = 0; i < nodes_num; i++) {
        matrix[i] = (int*) calloc(nodes_num, sizeof(int));
    }

    int power;
    int curr_power;
    int chance;
    for (int i = 0; i < nodes_num; i++) {
        power = rand() % 30 + 1;
        curr_power = 0;
        for (int j = i + 1; j < nodes_num; j++) {
            chance = rand() % nodes_num + 1;
            if (chance <= (nodes_num * 5 / power) && curr_power < power) {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
                curr_power++;
            } else {
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

void printAdjacencyMatrix(int** adj_matrix, int nodes_num) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < nodes_num; i++) {
        for (int j = 0; j < nodes_num; j++) {
            printf("%d ", adj_matrix[i][j]);
        }
        printf("\n");
    }
}

void saveAdjacencyMatrixToBinary(const char* filename, int** adj_matrix, int nodes_num) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open %s\n", &filename);
        exit(1);
    }
    for (int i = 0; i < nodes_num; i++) {
        fwrite(adj_matrix[i], sizeof(int), nodes_num, file);
    }
    fclose(file);
}

int** readMatrixFromBinary(const char* filename, int nodes_num) {
    int** matrix = (int**) calloc(nodes_num, sizeof(int*));
    for (int i = 0; i < nodes_num; i++) {
        matrix[i] = (int*) calloc(nodes_num, sizeof(int));
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }  
    for (int i = 0; i < nodes_num; i++) {
        fread(matrix[i], sizeof(int), nodes_num, file);
    }
    fclose(file);
    return matrix;
}