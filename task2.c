#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int arr[15];
    srand(time(NULL));

    printf("Исходный массив:\n");
    for (int i = 0; i < 15; i++) {
        // Числа от 0 до 10
        arr[i] = rand() % 11; 
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("Элементы без повторов:\n");
    for (int i = 0; i < 15; i++) {
        int already_seen = 0;
        for (int j = 0; j < i; j++) {
            if (arr[j] == arr[i]) {
                already_seen = 1;
                break;
            }
        }
        // Если не встречали раньше выводим
        if (!already_seen) {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");

    return 0;
}
