#include <stdio.h>

int main(){
    float A[5][5]={{1.0, 2.0, 3.0, 2.0, 2.0},
                   {2.5, 1.5, 2.0, 2.0, 2.0}, 
                   {0.5, 1.5, 2.5, 3.5, 2.0},
                   {3.0, 2.0, 1.0, 2.0, 2.0}, 
                   {1.5, 1.5, 1.5, 2.5, 3.0} };
    float sum_str[5];
    for (int str=0;str<5;str++){
        for (int stolb=0;stolb<5;stolb++){
            sum_str[str]+=A[str][stolb];
        }
    }
    for (int str=0;str<5;str++){
        for (int stolb=0;stolb<5;stolb++){
            printf("%4f ",A[str][stolb]/sum_str[str]);
            if (stolb==4){
                printf("\n");
            }
        }
    }
    return 0;
}