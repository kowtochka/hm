#include "graphics.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    printf("=== Синусоидальная анимация ===\n");
    
    if (initgraph(800, 600, "Sine Wave Animation") != 0) {
        return 1;
    }
    
    int center_x = 400;
    int center_y = 300;
    double time = 0.0;
    
    printf("Синусоидальная анимация запущена...\n");
    
    while (!kbhit()) {
        setbkcolor(BLACK);
        cleardevice();
        
        // Рисуем ось X
        setcolor(DARKGRAY);
        line(0, center_y, 800, center_y);
        
        // Анимированная синусоида
        int prev_x = 0;
        int prev_y = center_y;
        
        for (int x = 0; x < 800; x += 4) {
            double x_angle = (x / 800.0) * 4 * M_PI + time;
            double y_offset = sin(x_angle) * 100;
            
            int current_y = center_y + (int)y_offset;
            
            setcolor(GREEN);
            line(prev_x, prev_y, x, current_y);
            
            // Рисуем точку на волне
            setcolor(CYAN);
            fillellipse(x, current_y, 3, 3);
            
            prev_x = x;
            prev_y = current_y;
        }
        
        // Движущийся шар по синусоиде
        double ball_x_angle = time * 2;
        double ball_y_offset = sin(ball_x_angle) * 100;
        int ball_x = (int)((ball_x_angle / (4 * M_PI)) * 800) % 800;
        int ball_y = center_y + (int)ball_y_offset;
        
        setcolor(RED);
        fillellipse(ball_x, ball_y, 15, 15);
        setcolor(YELLOW);
        circle(ball_x, ball_y, 15);
        
        // Вращающиеся круги вокруг центра
        for (int i = 0; i < 8; i++) {
            double angle = time + (i * M_PI / 4);
            int orbit_radius = 150;
            int circle_x = center_x + (int)(cos(angle) * orbit_radius);
            int circle_y = center_y + (int)(sin(angle) * orbit_radius);
            
            setcolor(2 + i); // Разные цвета
            fillellipse(circle_x, circle_y, 20, 20);
            setcolor(WHITE);
            circle(circle_x, circle_y, 20);
        }
        
        // Пульсирующий круг в центре
        int pulse_size = 30 + (int)(sin(time * 3) * 15);
        setcolor(MAGENTA);
        fillellipse(center_x, center_y, pulse_size, pulse_size);
        setcolor(WHITE);
        circle(center_x, center_y, pulse_size);
        
        // Информация
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        char info[100];
        sprintf(info, "Время: %.2f | Нажмите любую клавишу для выхода", time);
        outtextxy(10, 10, info);
        
        sprintf(info, "Синусоида: y = sin(4πx + t)");
        outtextxy(10, 30, info);
        
        flush();
        delay(16);
        
        time += 0.05;
    }
    
    closegraph();
    printf("Анимация завершена.\n");
    
    return 0;
}