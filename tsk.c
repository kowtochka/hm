#include <stdio.h>
#include <string.h>

int main() {
    char text[1000];
    char prefix[50];
    char temp[100];
    
    printf("Введите текст (слова через запятую): ");
    gets(text);
    
    printf("Введите приставку: ");
    gets(prefix);
    
    printf("Слова с приставкой \"%s\":\n", prefix);
    
    char *p = text;
    while ((p = strtok(p, ",")) != NULL) {
        while (*p == ' ') p++;
        if (strncmp(p, prefix, strlen(prefix)) == 0)
            printf("%s\n", p);
        p = NULL;
    }
    
    printf("\nПодсчёт повторений слов:\n");
    char words[100][50];
    int count[100] = {0};
    int n = 0;
    
    p = text;
    while ((p = strtok(p, ",")) != NULL) {
        while (*p == ' ') p++;
        strcpy(temp, p);
        // ищем, было ли уже такое слово
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(words[i], temp) == 0) {
                count[i]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(words[n], temp);
            count[n] = 1;
            n++;
        }
        p = NULL;
    }
    
    for (int i = 0; i < n; i++) {
        printf("%s - %d раз(а)\n", words[i], count[i]);
    }
    
    // Задание 3: сортировка фамилий по алфавиту
    printf("\nВведите фамилии через запятую: ");
    gets(text);
    
    char surnames[50][50];
    int cnt = 0;
    
    p = text;
    while ((p = strtok(p, ",")) != NULL) {
        while (*p == ' ') p++;
        strcpy(surnames[cnt++], p);
        p = NULL;
    }
    
    // простая сортировка пузырьком
    for (int i = 0; i < cnt-1; i++)
        for (int j = 0; j < cnt-i-1; j++)
            if (strcmp(surnames[j], surnames[j+1]) > 0) {
                strcpy(temp, surnames[j]);
                strcpy(surnames[j], surnames[j+1]);
                strcpy(surnames[j+1], temp);
            }
    
    printf("По алфавиту:\n");
    for (int i = 0; i < cnt; i++)
        printf("%s\n", surnames[i]);
    
    return 0;
}
