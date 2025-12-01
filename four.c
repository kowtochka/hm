#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    
    int a[3][3];  
    int gen = 0;
    int found = 0;
    
    while (!found) {
        gen++;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                a[i][j] = rand() % 9 + 1;
            }
        }
        int sum = a[0][0] + a[0][1] + a[0][2];
        int ok = 1;
        for (int i = 1; i < 3; i++) {
            if (a[i][0] + a[i][1] + a[i][2] != sum) {
                ok = 0;
                break;
            }
        }
        
        if (!ok) continue;
        for (int j = 0; j < 3; j++) {
            if (a[0][j] + a[1][j] + a[2][j] != sum) {
                ok = 0;
                break;
            }
        }
        
        if (ok) {
            found = 1;
            printf("генераций: %d\n", gen);
            
            printf("магический квадрат:\n");
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    printf("%2d ", a[i][j]);
                    if (j==2){
                        printf("\n");
                    }
                }
            }
            printf("Сумма в каждой строке и столбце: %d\n", sum);
        }
    }
    
    return 0;
}