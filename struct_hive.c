#include <stdio.h>

typedef struct {
    int scouts;
    int foragers;
    Matrix* arr;
} Hive;

Hive* beeHive(int scouts, int foragers) {
    Hive* hive = (Hive*) malloc(sizeof(Hive));
    if (!hive) {
        printf("Memory allocation for hive failed\n");
        exit(1);
    }

    hive->scouts = scouts;
    hive->foragers = foragers;
    hive->arr = NULL;

    return hive;
}

void freeHive(Hive* hive) {
    free(hive);
}