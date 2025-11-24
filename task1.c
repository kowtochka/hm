#include <stdio.h>
#include <math.h>

int main() {
    double a = 2.14;
    double b = -4.21;
    double c = 3.25;
    double x_start = -4.5;
    double x_end = -33.5;
    double h = 0.5;
    double Y[100];
    int count = 0;

    for (double x = x_start; x >= x_end; x -= h) {
        Y[count] = a * pow(x, 2) * sin(x) + b * x + c;
        count++;
    }

    printf("Вычислено %d значений.\n", count);

//Сортировка
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (Y[j] < Y[j + 1]) {
                double temp = Y[j];
                Y[j] = Y[j + 1];
                Y[j + 1] = temp;
            }
        }
    }

    //Вывод на экран
    printf("Отсортированный массив Y (по убыванию):\n");
    for (int i = 0; i < count; i++) {
        printf("%.4f\n", Y[i]);
    }

    return 0;
}
