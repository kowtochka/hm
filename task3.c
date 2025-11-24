#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    float A[20];
    float B[20];
    int sizeA = 20;
    int sizeB = 0;

    srand(time(NULL));

    printf("Массив А (исходный):\n");
    for (int i = 0; i < sizeA; i++) {
        float scale = (float)rand() / (float)RAND_MAX; //умная генерация рандомных чисел
        A[i] = -10.0 + scale * 20.0;
        printf("%.2f  ", A[i]);
    }
    printf("\n\n");
    for (int i = 0; i < sizeA; i++) {
        if (A[i] > 0) {
            B[sizeB] = A[i];
            sizeB++;
        }
    }

    printf("Массив B (только положительные):\n");
    for (int i = 0; i < sizeB; i++) {
        printf("%.2f  ", B[i]);
    }
    printf("\n");

    return 0;
}
