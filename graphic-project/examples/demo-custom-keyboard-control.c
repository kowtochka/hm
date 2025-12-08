#include "graphics.h"
#include <stdio.h>

// Структура для управляемой фигуры
typedef struct {
    int x, y;
    int size;
    int color;
    int speed;
    int shape_type; // 0-круг, 1-квадрат, 2-треугольник
    int score;
    char name[20];
} Player;

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

// Функция для проверки границ
void check_bounds(Player *player, int max_x, int max_y) {
    if (player->x - player->size < 0) player->x = player->size;
    if (player->x + player->size > max_x) player->x = max_x - player->size;
    if (player->y - player->size < 0) player->y = player->size;
    if (player->y + player->size > max_y) player->y = max_y - player->size;
}

// Функция для отображения управления
void show_controls(int x, int y) {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    
    outtextxy(x, y, "Управление:");
    outtextxy(x, y + 20, "Стрелки - движение");
    outtextxy(x, y + 40, "SPACE - сменить форму");
    outtextxy(x, y + 60, "C - сменить цвет");
    outtextxy(x, y + 80, "+/- - изменить размер");
    outtextxy(x, y + 100, "ESC - выход");
}

int main() {
    printf("=== Управление фигурой с клавиатуры ===\n");
    
    if (initgraph(800, 600, "Keyboard Control - Custom Keys") != 0) {
        return 1;
    }
    
    // Инициализация игрока
    Player player = {
        .x = 400,
        .y = 300,
        .size = 30,
        .color = RED,
        .speed = 5,
        .shape_type = 0,
        .score = 0,
        .name = "Игрок"
    };
    
    int running = 1;
    int frame_count = 0;
    
    // Цели для сбора
    int target_x = 200, target_y = 200;
    int target_active = 1;
    int targets_collected = 0;
    
    printf("Игра запущена. Управление:\n");
    printf("  Стрелки - движение\n");
    printf("  SPACE - сменить форму\n");
    printf("  C - сменить цвет\n");
    printf("  +/- - изменить размер\n");
    printf("  ESC - выход\n");
    
    while (running) {
        // Обработка событий клавиатуры
        while (kbhit()) {
            int key = getch();
            
            switch (key) {
                case KEY_UP:
                    player.y -= player.speed;
                    printf("Движение вверх\n");
                    break;
                    
                case KEY_DOWN:
                    player.y += player.speed;
                    printf("Движение вниз\n");
                    break;
                    
                case KEY_LEFT:
                    player.x -= player.speed;
                    printf("Движение влево\n");
                    break;
                    
                case KEY_RIGHT:
                    player.x += player.speed;
                    printf("Движение вправо\n");
                    break;
                    
                case KEY_SPACE:
                    player.shape_type = (player.shape_type + 1) % 3;
                    printf("Форма изменена: ");
                    switch (player.shape_type) {
                        case 0: printf("Круг\n"); break;
                        case 1: printf("Квадрат\n"); break;
                        case 2: printf("Треугольник\n"); break;
                    }
                    break;
                    
                case KEY_C:
                    player.color = (player.color + 1) % 14;
                    if (player.color == 0) player.color = 1; // Пропускаем черный
                    printf("Цвет изменен: %d\n", player.color);
                    break;
                    
                case KEY_PLUS:
                    player.size += 5;
                    if (player.size > 100) player.size = 100;
                    printf("Размер увеличен: %d\n", player.size);
                    break;
                    
                case KEY_MINUS:
                    player.size -= 5;
                    if (player.size < 10) player.size = 10;
                    printf("Размер уменьшен: %d\n", player.size);
                    break;
                    
                case KEY_ESC:
                    running = 0;
                    printf("Выход по нажатию ESC\n");
                    break;
                    
                default:
                    printf("Нажата клавиша с кодом: 0x%04X\n", key);
                    break;
            }
        }
        
        // Проверяем границы
        check_bounds(&player, 800, 600);
        
        // Проверяем сбор цели
        if (target_active) {
            int dx = player.x - target_x;
            int dy = player.y - target_y;
            int distance = dx * dx + dy * dy;
            
            if (distance < (player.size + 15) * (player.size + 15)) {
                player.score++;
                targets_collected++;
                target_active = 0;
                printf("Цель собрана! Счет: %d\n", player.score);
                
                // Создаем новую цель через 0.5 секунды
                delay(500);
                target_x = 50 + rand() % 700;
                target_y = 50 + rand() % 500;
                target_active = 1;
            }
        }
        
        // Отрисовка
        setbkcolor(BLACK);
        cleardevice();
        
        // Рисуем сетку
        setcolor(DARKGRAY);
        for (int x = 0; x < 800; x += 40) {
            line(x, 0, x, 600);
        }
        for (int y = 0; y < 600; y += 40) {
            line(0, y, 800, y);
        }
        
        // Рисуем цель
        if (target_active) {
            setcolor(GREEN);
            fillellipse(target_x, target_y, 15, 15);
            setcolor(WHITE);
            circle(target_x, target_y, 15);
            
            // Мигающая анимация цели
            if (frame_count % 30 < 15) {
                setcolor(YELLOW);
                circle(target_x, target_y, 20);
            }
        }
        
        // Рисуем игрока
        setcolor(player.color);
        switch (player.shape_type) {
            case 0: // Круг
                fillellipse(player.x, player.y, player.size, player.size);
                setcolor(WHITE);
                circle(player.x, player.y, player.size);
                break;
                
            case 1: // Квадрат
                bar(player.x - player.size, player.y - player.size,
                    player.x + player.size, player.y + player.size);
                setcolor(WHITE);
                rectangle(player.x - player.size, player.y - player.size,
                         player.x + player.size, player.y + player.size);
                break;
                
            case 2: // Треугольник
                draw_triangle(player.x, player.y, player.size, player.color);
                setcolor(WHITE);
                int points[] = {
                    player.x, player.y - player.size,
                    player.x - player.size, player.y + player.size,
                    player.x + player.size, player.y + player.size
                };
                drawpoly(3, points);
                break;
        }
        
        // Отображаем информацию
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        
        char info[100];
        sprintf(info, "Счет: %d", player.score);
        outtextxy(10, 10, info);
        
        sprintf(info, "Позиция: (%d, %d)", player.x, player.y);
        outtextxy(10, 30, info);
        
        sprintf(info, "Размер: %d", player.size);
        outtextxy(10, 50, info);
        
        // Информация о форме
        const char* shape_names[] = {"Круг", "Квадрат", "Треугольник"};
        sprintf(info, "Форма: %s", shape_names[player.shape_type]);
        outtextxy(10, 70, info);
        
        sprintf(info, "Собрано целей: %d", targets_collected);
        outtextxy(10, 90, info);
        
        sprintf(info, "Кадр: %d", frame_count);
        outtextxy(10, 110, info);
        
        // Отображаем управление
        show_controls(500, 10);
        
        // Прогресс-бар размера
        setcolor(BLUE);
        int bar_width = (player.size - 10) * 2; // 10-100 -> 0-180
        bar(600, 570, 600 + bar_width, 580);
        setcolor(WHITE);
        rectangle(600, 570, 780, 580);
        outtextxy(550, 570, "Размер:");
        
        flush();
        delay(16); // ~60 FPS
        frame_count++;
    }
    
    // Завершение
    cleardevice();
    setbkcolor(BLUE);
    cleardevice();
    
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(400, 200, "ИГРА ЗАВЕРШЕНА");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    char final_score[100];
    sprintf(final_score, "Финальный счет: %d", player.score);
    outtextxy(400, 250, final_score);
    
    char collected[100];
    sprintf(collected, "Собрано целей: %d", targets_collected);
    outtextxy(400, 290, collected);
    
    char frames[100];
    sprintf(frames, "Кадров отрисовано: %d", frame_count);
    outtextxy(400, 330, frames);
    
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(400, 400, "Нажмите любую клавишу для выхода");
    
    flush();
    
    // Ждем любую клавишу
    while (!kbhit()) {
        delay(100);
    }
    getch();
    
    closegraph();
    printf("Игра завершена. Счет: %d, Целей: %d, Кадров: %d\n", 
           player.score, targets_collected, frame_count);
    
    return 0;
}