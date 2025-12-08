#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Структура для анимированного объекта
typedef struct {
    int x, y;
    int dx, dy;
    int size;
    int color;
    int shape_type; // 0-круг, 1-квадрат, 2-треугольник
} AnimatedShape;

// Функция для рисования треугольника
void draw_triangle(int x, int y, int size, int color) {
    int points[] = {
        x, y - size,
        x - size, y + size,
        x + size, y + size
    };
    setcolor(color);
    fillpoly(3, points);
}

// Функция для отскакивания от границ
void bounce_borders(AnimatedShape *shape, int max_x, int max_y) {
    if (shape->x - shape->size <= 0 || shape->x + shape->size >= max_x) {
        shape->dx = -shape->dx;
    }
    if (shape->y - shape->size <= 0 || shape->y + shape->size >= max_y) {
        shape->dy = -shape->dy;
    }
}

int main() {
    printf("=== Анимированные фигуры ===\n");
    
    if (initgraph(800, 600, "Animated Shapes Demo") != 0) {
        fprintf(stderr, "ERROR: Не удалось инициализировать графику\n");
        return 1;
    }
    
    // Создаем несколько анимированных фигур
    AnimatedShape shapes[10];
    int num_shapes = 8;
    
    // Инициализация фигур со случайными параметрами
    for (int i = 0; i < num_shapes; i++) {
        shapes[i].x = 50 + rand() % 700;
        shapes[i].y = 50 + rand() % 500;
        shapes[i].dx = (rand() % 7) - 3;
        shapes[i].dy = (rand() % 7) - 3;
        if (shapes[i].dx == 0) shapes[i].dx = 1;
        if (shapes[i].dy == 0) shapes[i].dy = 1;
        shapes[i].size = 15 + rand() % 25;
        shapes[i].color = 1 + rand() % 14; // Все цвета кроме черного и белого
        shapes[i].shape_type = rand() % 3;
    }
    
    int frame_count = 0;
    int max_frames = 500; // Ограничение количества кадров
    
    printf("Анимация запущена. Нажмите любую клавишу для остановки...\n");
    
    while ((frame_count < max_frames) && !kbhit()) {
        // Очищаем экран
        setbkcolor(BLACK);
        cleardevice();
        
        // Рисуем сетку для фона
        setcolor(DARKGRAY);
        for (int x = 0; x < 800; x += 40) {
            line(x, 0, x, 600);
        }
        for (int y = 0; y < 600; y += 40) {
            line(0, y, 800, y);
        }
        
        // Обновляем и рисуем все фигуры
        for (int i = 0; i < num_shapes; i++) {
            // Обновляем позицию
            shapes[i].x += shapes[i].dx;
            shapes[i].y += shapes[i].dy;
            
            // Проверяем столкновение с границами
            bounce_borders(&shapes[i], 800, 600);
            
            // Рисуем фигуру в зависимости от типа
            switch (shapes[i].shape_type) {
                case 0: // Круг
                    setcolor(shapes[i].color);
                    fillellipse(shapes[i].x, shapes[i].y, shapes[i].size, shapes[i].size);
                    setcolor(WHITE);
                    circle(shapes[i].x, shapes[i].y, shapes[i].size);
                    break;
                    
                case 1: // Квадрат
                    setcolor(shapes[i].color);
                    bar(shapes[i].x - shapes[i].size, shapes[i].y - shapes[i].size,
                        shapes[i].x + shapes[i].size, shapes[i].y + shapes[i].size);
                    setcolor(WHITE);
                    rectangle(shapes[i].x - shapes[i].size, shapes[i].y - shapes[i].size,
                             shapes[i].x + shapes[i].size, shapes[i].y + shapes[i].size);
                    break;
                    
                case 2: // Треугольник
                    draw_triangle(shapes[i].x, shapes[i].y, shapes[i].size, shapes[i].color);
                    break;
            }
        }
        
        // Отображаем информацию
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        char info[100];
        sprintf(info, "Кадр: %d/%d | Фигур: %d | Нажмите любую клавишу для выхода", 
                frame_count, max_frames, num_shapes);
        outtextxy(10, 10, info);
        
        // Отображаем FPS
        sprintf(info, "FPS: ~60");
        outtextxy(700, 10, info);
        
        flush();
        delay(16); // ~60 FPS
        
        frame_count++;
    }
    
    // Финальное сообщение
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(400, 250, "Анимация завершена!");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(400, 300, "Нажмите любую клавишу для выхода");
    
    flush();
    getch();
    
    closegraph();
    printf("Анимация завершена после %d кадров.\n", frame_count);
    
    return 0;
}