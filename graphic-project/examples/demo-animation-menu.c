#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void bouncing_shapes_animation();
void sine_wave_animation();
void particle_system_animation();

int main() {
    printf("=== Меню анимаций ===\n");
    
    if (initgraph(800, 600, "Animation Menu") != 0) {
        return 1;
    }
    
    int selected = 0;
    char* animations[] = {
        "1. ПРЫГАЮЩИЕ ФИГУРЫ",
        "2. СИНУСОИДАЛЬНАЯ ВОЛНА", 
        "3. СИСТЕМА ЧАСТИЦ",
        "4. ВЫХОД"
    };
    int num_animations = 4;
    
    while (1) {
        setbkcolor(BLUE);
        cleardevice();
        
        // Заголовок
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        outtextxy(400, 50, "ВЫБОР АНИМАЦИИ");
        
        // Пункты меню
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        
        for (int i = 0; i < num_animations; i++) {
            if (i == selected) {
                setcolor(RED);
            } else {
                setcolor(WHITE);
            }
            outtextxy(400, 150 + i * 60, animations[i]);
        }
        
        // Инструкция
        setcolor(LIGHTGREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(400, 450, "Используйте СТРЕЛКИ ВВЕРХ/ВНИЗ для выбора");
        outtextxy(400, 470, "Нажмите ENTER для запуска анимации");
        outtextxy(400, 490, "Нажмите ESC для выхода");
        
        // Предпросмотр анимации
        setcolor(CYAN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(400, 520, "Текущий выбор: ");
        
        switch (selected) {
            case 0:
                outtextxy(400, 540, "Прыгающие фигуры со случайным движением");
                break;
            case 1:
                outtextxy(400, 540, "Синусоидальная волна с движущимся шаром");
                break;
            case 2:
                outtextxy(400, 540, "Система частиц с гравитацией");
                break;
            case 3:
                outtextxy(400, 540, "Выход из программы");
                break;
        }
        
        flush();
        
        // Обработка ввода
        int key = getch();
        
        switch (key) {
            case KEY_UP:
                selected = (selected - 1 + num_animations) % num_animations;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % num_animations;
                break;
            case KEY_ENTER:
                if (selected == 3) {
                    closegraph();
                    printf("Выход из программы.\n");
                    return 0;
                }
                
                // Запускаем выбранную анимацию
                closegraph(); // Закрываем текущее окно
                
                switch (selected) {
                    case 0:
                        bouncing_shapes_animation();
                        break;
                    case 1:
                        sine_wave_animation();
                        break;
                    case 2:
                        particle_system_animation();
                        break;
                }
                
                // После завершения анимации переинициализируем графику
                if (initgraph(800, 600, "Animation Menu") != 0) {
                    return 1;
                }
                break;
            case KEY_ESC:
                closegraph();
                printf("Выход из программы.\n");
                return 0;
        }
    }
    
    return 0;
}

// Реализации анимаций (упрощенные версии)
void bouncing_shapes_animation() {
    initgraph(800, 600, "Bouncing Shapes");
    // ... код примера ...
    closegraph();
}

void sine_wave_animation() {
    initgraph(800, 600, "Sine Wave");
    // ... код примера ...
    closegraph();
}

void particle_system_animation() {
    initgraph(800, 600, "Particle System");
    // ... код примера ...
    closegraph();
}