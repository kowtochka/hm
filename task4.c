#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int size) { //баблсорт функция
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int C[20], D[20], E[40];
    int size = 20;
    
    srand(time(NULL));
    for(int i=0; i<size; i++) {
        C[i] = rand() % 21; // [0, 20]
        D[i] = rand() % 21; // [0, 20]
    }
    bubbleSort(C, size);
    bubbleSort(D, size);

    printf("Массив C (отсортирован):\n");
    for(int i=0; i<size; i++) printf("%d ", C[i]);
    printf("\n");

    printf("Массив D (отсортирован):\n");
    for(int i=0; i<size; i++) printf("%d ", D[i]);
    printf("\n\n");

    //Алгоритм слияния
    int i = 0; // Индекс для C
    int j = 0; // Индекс для D
    int k = 0; // Индекс для E

    while (i < size && j < size) {
        if (C[i] < D[j]) {
            E[k] = C[i];
            i++;
        } else {
            E[k] = D[j];
            j++;
        }
        k++;
    }
    while (i < size) { // остатки C
        E[k] = C[i];
        i++;
        k++;
    }

    while (j < size) { // остатки D
        E[k] = D[j];
        j++;
        k++;
    }

    printf("Массив E (результат слияния):\n");
    for(int x=0; x < 40; x++) {
        printf("%d ", E[x]);
    }
    printf("\n");

    return 0;
}
