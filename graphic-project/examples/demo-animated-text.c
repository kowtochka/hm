#include "graphics.h"
#include <stdio.h>
#include <string.h>

int main() {
    if (initgraph(800, 600, "Animated Text Styles") != 0) {
        return 1;
    }
    
    setbkcolor(BLACK);
    cleardevice();
    
    char* message = "Анимированный текст!";
    int colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE};
    
    // Анимация изменения размера
    for (int size = 1; size <= 5; size++) {
        cleardevice();
        
        setcolor(colors[size - 1]);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        
        outtextxy(400, 200, message);
        
        char info[50];
        sprintf(info, "Размер: %d", size);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(400, 300, info);
        
        flush();
        delay(800);
    }
    
    // Анимация изменения направления
    cleardevice();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(400, 100, "Меняем направление...");
    
    for (int i = 0; i < 3; i++) {
        cleardevice();
        
        // Горизонтальный текст
        setcolor(GREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(400, 200, "Горизонтально");
        
        // Вертикальный текст
        setcolor(RED);
        settextstyle(DEFAULT_FONT, VERT_DIR, 2);
        outtextxy(500 + i * 50, 300, "Вертикально");
        
        flush();
        delay(1000);
    }
    
    // Финальное сообщение
    cleardevice();
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(400, 300, "Анимация завершена!");
    
    delay(3000);
    closegraph();
    return 0;
}