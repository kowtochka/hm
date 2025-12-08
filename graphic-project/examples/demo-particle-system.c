#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_PARTICLES 200
#define X 1200
#define Y 800
#define SIZE 15

typedef struct {
    float x, y;
    float dx, dy;
    int life;
    int max_life;
    int color;
    int size;
} Particle;

Particle particles[MAX_PARTICLES];

void init_particle(int index, int x, int y) {
    particles[index].x = x;
    particles[index].y = y;

    // Случайное направление
    float angle = (rand() % 360) * M_PI / 180.0;
    float speed = 0.5 + (rand() % 100) / 50.0;
    particles[index].dx = cos(angle) * speed;
    particles[index].dy = sin(angle) * speed;

    particles[index].life = 0;
    particles[index].max_life = 50 + rand() % 100;
    particles[index].color = 1 + rand() % 14;
    particles[index].size = 2 + rand() % SIZE;
}

void update_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life < particles[i].max_life) {
            // Обновляем позицию
            particles[i].x += particles[i].dx;
            particles[i].y += particles[i].dy;

            // Добавляем гравитацию
            particles[i].dy += 0.1;

            // Увеличиваем возраст
            particles[i].life++;

            // Проверяем границы
            if (particles[i].x < 0 || particles[i].x > X ||
                particles[i].y < 0 || particles[i].y > Y) {
                particles[i].life = particles[i].max_life; // Убиваем частицу
            }
        }
    }
}

void draw_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life < particles[i].max_life) {
            // Вычисляем альфа-канал на основе оставшейся жизни
            float alpha = 1.0 - ((float)particles[i].life / particles[i].max_life);
            int current_size = (int)(particles[i].size * alpha);

            if (current_size > 0) {
                setcolor(particles[i].color);
                fillellipse((int)particles[i].x, (int)particles[i].y, current_size, current_size);
            }
        }
    }
}

int find_dead_particle() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life >= particles[i].max_life) {
            return i;
        }
    }
    return -1;
}

int main() {
    printf("=== Система частиц ===\n");

    if (initgraph(X, Y, "Particle System") != 0) {
        return 1;
    }

    // Инициализируем все частицы как "мертвые"
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].life = particles[i].max_life = 1;
    }

    int mouse_x = X/2, mouse_y = Y/2;
    int frame_count = 0;

    printf("Нажмите любую клавишу для выхода...\n");

    while (!kbhit()) {
        setbkcolor(BLACK);
        cleardevice();

        // Создаем новые частицы вокруг курсора мыши
        for (int i = 0; i < 5; i++) {
            int particle_index = find_dead_particle();
            if (particle_index != -1) {
                int offset_x = (rand() % 21) - 10;
                int offset_y = (rand() % 21) - 10;
                init_particle(particle_index, mouse_x + offset_x, mouse_y + offset_y);
            }
        }

        // Обновляем и рисуем частицы
        update_particles();
        draw_particles();

        // Рисуем курсор мыши
        setcolor(WHITE);
        circle(mouse_x, mouse_y, 20);
        line(mouse_x - 15, mouse_y, mouse_x + 15, mouse_y);
        line(mouse_x, mouse_y - 15, mouse_x, mouse_y + 15);


        // Информация
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

        char info[100];
        sprintf(info, "Частиц: %d/%d | Кадр: %d", MAX_PARTICLES, MAX_PARTICLES, frame_count);
        outtextxy(10, 10, info);

        sprintf(info, "Курсор: (%d, %d)", mouse_x, mouse_y);
        outtextxy(10, 30, info);

        flush();
        delay(33); // ~30 FPS

        // Обновляем позицию мыши (в реальном приложении здесь был бы ввод мыши)
        mouse_x = X/2 + (int)(sin(frame_count * 0.07) * (X-100)/2);
        mouse_y = Y/2 + (int)(cos(frame_count * 0.05) * (Y-100)/2);

        frame_count++;
    }

    closegraph();
    printf("Система частиц завершена после %d кадров.\n", frame_count);

    return 0;
}
