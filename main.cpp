#include <stdio.h>
#include <stdlib.h>
#define SIZE 1024
int main(int argc, char** argv) {
char* str = (char*)malloc(sizeof(char) * SIZE);
<<<<<<< HEAD
printf("Введите своё имя: \n");
scanf("%s", str);
=======
printf("Введите своё имя:\n");
str = gets(str);
>>>>>>> main
printf("Привет, %s", str);
return 0;
}
