#include "graphics.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Демонстрация графической библиотеки ===\n");
    
    if (initgraph(800, 600, "Extended Graphics Demo") != 0) {
        fprintf(stderr, "ERROR: Не удалось инициализировать графику\n");
        return 1;
    }
    
    printf("SUCCESS: Графика инициализирована\n");
    
    // Очищаем экран
    setbkcolor(BLUE);
    cleardevice();
    
    // Демонстрация новых функций
    
    // 1. Работа с текстом
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(100, 30, "Демонстрация текстовых функций");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    
    // Вывод текста с разным выравниванием
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(50, 80, "Левый верхний");
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(400, 80, "Центр");
    
    settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
    outtextxy(750, 80, "Правый нижний");
    
    // 2. Демонстрация перемещения курсора
    setcolor(GREEN);
    moveto(100, 150);
    outtext("Начало: ");
    moverel(50, 0);
    outtext("После moverel");
    
    // 3. Получение информации о цветах
    char info[100];
    setcolor(RED);
    settextjustify(DEFAULT_FONT, DEFAULT_FONT);
    sprintf(info, "Текущий цвет: %d, Фон: %d", getcolor(), getbkcolor());
    outtextxy(100, 200, info);
    
    // 4. Размеры экрана
    sprintf(info, "Размеры: %dx%d", getmaxwidth(), getmaxheight());
    outtextxy(100, 230, info);
    
    // 5. Размеры текста
    char *sample_text = "Пример текста";
    sprintf(info, "Ширина текста: %d, Высота: %d", 
            textwidth(sample_text), textheight(sample_text));
    outtextxy(100, 260, info);

    
    // 6. Демонстрация задержки и ввода
    setcolor(CYAN);
    outtextxy(100, 300, "Нажмите любую клавишу для продолжения...");
    flush();
    
    // Ждем нажатия клавиши
    while (!kbhit()) {
        delay(100);
    }
    int key = getch();
    
    sprintf(info, "Нажата клавиша: %d", key);
    outtextxy(100, 330, info);
    
    // 7. Демонстрация получения настроек текста
    textsettingstype text_info;
    gettextsettings(&text_info);
    sprintf(info, "Шрифт: %d, Направление: %d, Размер: %d", 
            text_info.font, text_info.direction, text_info.charsize);
    outtextxy(100, 360, info);
    
    // 8. Анимация с использованием delay
    setcolor(MAGENTA);
    for (int i = 0; i < 5; i++) {
        int x = 100 + i * 100;
        int y = 400;
        fillellipse(x, y, 30, 30);
        flush();
        delay(500); // Задержка 500 мс
    }
    
    outtextxy(100, 500, "Демонстрация завершена. Нажмите любую клавишу для выхода...");
    flush();
    
    // Ждем нажатия клавиши для выхода
    getch();
    
    closegraph();
    printf("Графика закрыта. Демонстрация завершена.\n");
    
    return 0;
}