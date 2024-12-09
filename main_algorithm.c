#include <stdio.h>

#include "struct_matrix.c"
#include "struct_hive.c"

#define PATH "matrix1.bin"
#define SCOUTS 2
#define FORAGERS 28
#define ITERATIONS 1000

void abcAlgorithm(Hive* hive);
void collectNectar(Hive* hive, int inner_node, int foragers);
int availableColor(Hive* hive, int curr_node);

int main() {
    Matrix* arr = matrix(PATH);
    // printf("Nodes: %d\n", arr->nodes);
    // printMatrix(arr);
    // printDegree(arr);
    // printQueue(arr);
    // printStates(arr);

    Hive* hive = beeHive(SCOUTS, FORAGERS);
    hive->arr = arr;

    abcAlgorithm(hive);

    printStates(hive->arr);

    freeMatrix(arr);
    freeHive(hive);

    return 0;
}

void abcAlgorithm(Hive* hive) {
    int n = hive->arr->nodes;   
    int all_foragers;
    int foragers_per_node = (int)hive->foragers / hive->scouts;
    int queue_index = 0;
    int iterations = 0;
    int isColored = 0;
    
    int chromatic;
    int* best_states = (int*) calloc(sizeof(int), n);
    int best_chromatic = 99999;

    while (iterations < ITERATIONS) {
        iterations++;

        if (!isColored && checkForColoring(hive->arr)) {
                isColored = 1;
                printf("Matrix Colored!\n");
        }
           
        chromatic = findChromaticNum(hive->arr);   

        if (isColored) {
            if (chromatic <= best_chromatic) {
                best_chromatic = chromatic;
                for (int i = 0; i < n; i++) {
                    best_states[i] = hive->arr->states[i];
                }
            } else {
                for (int i = 0; i < n; i++) {
                    hive->arr->states[i] = best_states[i];
                    chromatic = best_chromatic;
                }
            }
        }

        if (iterations % 10 == 0 ) {
            printf("Iteration: %d   ", iterations);
            printf("Chromatic number: %d\n", chromatic); 
        }

        for (int i = 0; i < hive->scouts; i++) {
            if (queue_index < n) {
                // printf("    Curr Main Node: %d\n", hive->arr->queue[queue_index]);
                all_foragers = hive->foragers;
                if (all_foragers >= foragers_per_node) {
                    collectNectar(hive, hive->arr->queue[queue_index],  foragers_per_node);
                    all_foragers -= foragers_per_node;
                } else if (all_foragers > 0) {
                    collectNectar(hive, hive->arr->queue[queue_index], all_foragers);
                    all_foragers -= all_foragers;
                }
                queue_index++;
                if (queue_index >= n) {
                    queue_index = 0;
                }
            }
        }
    }

    free(best_states);
}

void collectNectar(Hive* hive, int inner_node, int foragers) {
    int n = hive->arr->nodes;
    hive->arr->states[inner_node] = 0;

    int* temp = readRow(hive->arr, inner_node);

    int curr_colored = 0;
    for (int i = 0; i < n; i++) {
        if (temp[i] == 1 && curr_colored < foragers) {
            hive->arr->states[i] = availableColor(hive, i);
            curr_colored++;
        }
    }

    hive->arr->states[inner_node] = availableColor(hive, inner_node);

    free(temp);
}

int availableColor(Hive* hive, int curr_node) {
    int n = hive->arr->nodes;
    hive->arr->states[curr_node] = 0;

    int* temp = readRow(hive->arr, curr_node);

    int neighbours = hive->arr->degree[curr_node];
    
    int* neighbours_colors = (int*) malloc(sizeof(int) * neighbours);
    if (!neighbours_colors) {
        printf("Memory allocation for used_colors array failed\n");
        exit(1);
    }

    // printf("    Color for %d\n    ", curr_node);
    int index = 0;
    for (int i = 0; i < n; i++) {
        if (temp[i] == 1) {
            neighbours_colors[index] = hive->arr->states[i];
            // printf("%d,", neighbours_colors[index]);
            index++;
        }
    }

    int color = 1;
    for (int i = 0; i < neighbours; i++) {
        if (color == neighbours_colors[i]) {
            color++;
            i = -1;
        }
    }

    // printf("   New color: %d\n", color);

    free(temp);
    free(neighbours_colors);

    // printf("\n");
    return color;
}

