#include <stdio.h>

int main(){
    int A[5][5]={{1,1,1,1,1},
                 {2,2,2,2,2},
                 {3,3,3,3,3},
                 {4,4,4,4,4},
                 {5,5,5,5,5}};
for(int str=0;str<5;str++){
    for (int stolb=0;stolb<5;stolb++){
        printf("%2d ",A[stolb][str]);
            if (stolb==4){
                printf("\n");
            }
    }
}
    return 0;
}