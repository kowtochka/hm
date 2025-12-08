#include "graphics.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("=== Демонстрация графической библиотеки ===\n");
    
    if (initgraph(800, 600, "Graphics Library Demo") != 0) {
        fprintf(stderr, "ERROR: Не удалось инициализировать графику\n");
        return 1;
    }
    
    printf("SUCCESS: Графика инициализирована\n");
    
    // Очищаем экран
    setbkcolor(BLUE);
    cleardevice();
    
    // Рисуем различные фигуры
    printf("Рисование фигур...\n");
    
    // 1. Прямоугольники
    setcolor(RED);
    bar(50, 50, 200, 150);
    
    setcolor(GREEN);
    rectangle(250, 50, 400, 150);
    
    setcolor(YELLOW);
    bar3d(450, 50, 600, 150, 15, 1);
    
    // 2. Окружности и эллипсы
    setcolor(WHITE);
    circle(100, 300, 50);
    
    setcolor(MAGENTA);
    fillellipse(250, 300, 60, 40);
    
    setcolor(CYAN);
    ellipse(400, 300, 0, 270, 70, 30);
    
    // 3. Дуги и секторы
    setcolor(YELLOW);
    arc(550, 300, 45, 315, 50);
    
    setcolor(LIGHTGREEN);
    pieslice(150, 450, 30, 300, 40);
    
    setcolor(LIGHTRED);
    sector(300, 450, 90, 270, 80, 40);
    
    // 4. Многоугольники
    setcolor(LIGHTCYAN);
    int poly_points[] = {500, 400, 550, 350, 600, 400, 575, 450, 525, 450};
    drawpoly(5, poly_points);
    
    setcolor(LIGHTMAGENTA);
    int fill_poly_points[] = {650, 400, 700, 350, 750, 400, 725, 450, 675, 450};
    fillpoly(5, fill_poly_points);
    
    // 5. Линии
    setcolor(WHITE);
    moveto(50, 500);
    lineto(150, 500);
    linerel(50, -50);
    lineto(300, 450);
    
    // 6. Пиксели
    for (int i = 0; i < 100; i++) {
        putpixel(350 + i, 500 + (i % 20), (i % 15) + 1);
    }
    
    printf("Все фигуры нарисованы. Ожидание 10 секунд...\n");
    sleep(10);
    
    closegraph();
    printf("Графика закрыта. Демонстрация завершена.\n");
    
    return 0;
}