# Graphics Library Project

Простая графическая бибилотека на C с использованием Cairo и X11.

## Зависимости

- libcairo2-dev
- libx11-dev
- pkg-config

## Установка зависимостей

### Ubuntu/Debian
```bash
sudo make install-deps-ubuntu
```
### Fedora/RHEL/РЭД ОС

```bash
sudo make install-deps-fedora
```
### Arch Linux

```bash
sudo make install-deps-arch
```

## Сборка

```bash
# Проверка зависимостей и сборка
sudo make all

# Или по отдельности

## Компиляция статической библиотеки
make library

## Компиляция динамической библиотеки
make shared

## Установка статической библиотеки
sudo make install

## Установка динамической библиотеки
sudo make install-shared

## Добавление установки LD_LIBRARY_PATH в .bashrc
make setup-bashrc

## Компиляция примеров
make examples
```

## Запуск примеров

```bash
# Показать список примеров
make list-examples

# Запустить конкретный пример
make run-<название примера>

## Например
make run-demo-shapes
make run-demo-particle-system
```

## Функции библиотеки

### Инициализация и завершение

#### `initgraph(int width, int height, const char* title)`
Инициализирует графическую систему.

**Параметры:**
- `width` - ширина окна
- `height` - высота окна  
- `title` - заголовок окна

**Возвращает:**
- `0` при успехе, `-1` при ошибке

**Пример:**
```c
if (initgraph(800, 600, "My Graphics App") == -1) {
    printf("Ошибка инициализации графики\n");
    return -1;
}
```

#### `closegraph()`
Завершает работу с графической системой и освобождает ресурсы.

### Управление цветом

#### `setcolor(int color)`
Устанавливает текущий цвет рисования.

**Параметры:**
- `color` - код цвета (0-15) из предопределенной палитры

#### `setbkcolor(int color)`
Устанавливает цвет фона.

#### `setrgbcolor(double r, double g, double b)`
Устанавливает цвет с помощью компонентов RGB.

**Параметры:**
- `r`, `g`, `b` - компоненты цвета (0.0 - 1.0)

#### `getcolor()`, `getbkcolor()`
Возвращают текущий цвет рисования и фона.

### Функции рисования примитивов

#### `line(int x1, int y1, int x2, int y2)`
Рисует линию от (x1,y1) до (x2,y2).

#### `rectangle(int left, int top, int right, int bottom)`
Рисует прямоугольник.

#### `circle(int x, int y, int radius)`
Рисует окружность.

#### `bar(int left, int top, int right, int bottom)`
Рисует залитый прямоугольник.

#### `bar3d(int left, int top, int right, int bottom, int depth, int topflag)`
Рисует 3D прямоугольник.

**Параметры:**
- `depth` - глубина 3D эффекта
- `topflag` - рисовать верхнюю грань (0/1)

#### `arc(int x, int y, int stangle, int endangle, int radius)`
Рисует дугу.

**Параметры:**
- `stangle`, `endangle` - начальный и конечный углы в градусах
- `radius` - радиус дуги

#### `ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius)`
Рисует эллипс или его часть.

#### `fillellipse(int x, int y, int xradius, int yradius)`
Рисует залитый эллипс.

#### `pieslice(int x, int y, int stangle, int endangle, int radius)`
Рисует и заполняет сектор круга.

#### `sector(int x, int y, int stangle, int endangle, int xradius, int yradius)`
Рисует и заполняет сектор эллипса.

#### `drawpoly(int n_points, int* points)`
Рисует многоугольник.

**Параметры:**
- `n_points` - количество точек
- `points` - массив координат [x1,y1,x2,y2,...]

#### `fillpoly(int n_points, int* points)`
Рисует и заполняет многоугольник.

#### `putpixel(int x, int y, int color)`
Рисует пиксель указанным цветом.

### Управление позицией рисования

#### `moveto(int x, int y)`
Перемещает текущую позицию рисования.

#### `moverel(int dx, int dy)`
Перемещает текущую позицию на относительное расстояние.

#### `lineto(int x, int y)`
Рисует линию от текущей позиции до указанной точки.

#### `linerel(int dx, int dy)`
Рисует линию от текущей позиции на относительное расстояние.

#### `getx()`, `gety()`
Возвращают текущую позицию рисования.

### Работа с текстом

#### `outtext(char *textstring)`
Выводит текст в текущей позиции.

#### `outtextxy(int x, int y, char *textstring)`
Выводит текст в указанной позиции.

#### `settextstyle(int font, int direction, int charsize)`
Устанавливает стиль текста.

**Параметры:**
- `font` - шрифт (пока только DEFAULT_FONT)
- `direction` - HORIZ_DIR или VERT_DIR
- `charsize` - множитель размера шрифта

#### `settextjustify(int horiz, int vert)`
Устанавливает выравнивание текста.

**Параметры:**
- `horiz` - LEFT_TEXT, CENTER_TEXT, RIGHT_TEXT
- `vert` - BOTTOM_TEXT, CENTER_TEXT, TOP_TEXT

#### `textheight(char *textstring)`, `textwidth(char *textstring)`
Возвращают высоту и ширину текста в пикселях.

#### `gettextsettings(textsettingstype *texttypeinfo)`
Получает текущие настройки текста.

### Управление отображением

#### `cleardevice()`, `clearviewport()`
Очищает экран/область просмотра.

#### `flush()`
Принудительно обновляет отображение.

#### `setlinestyle(int style, int thickness)`
Устанавливает стиль линии.

**Параметры:**
- `style` - 0 (SOLID), 1 (DOTTED), 2 (DASHED)
- `thickness` - толщина линии

### Функции ввода

#### `getch()`
Ожидает нажатия клавиши и возвращает её код.

#### `kbhit()`
Проверяет, была ли нажата клавиша (неблокирующая).

#### `delay(int msec)`
Задержка на указанное количество миллисекунд.

### Информационные функции

#### `getmaxx()`, `getmaxy()`
Возвращают максимальные координаты экрана.

#### `getmaxwidth()`, `getmaxheight()`
Возвращают ширину и высоту окна.

## Предопределенные константы

### Цвета
```c
BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, 
LIGHTMAGENTA, YELLOW, WHITE
```

### Настройки текста
```c
DEFAULT_FONT    // Единственный доступный шрифт
HORIZ_DIR       // Горизонтальное направление
VERT_DIR        // Вертикальное направление
LEFT_TEXT       // Выравнивание по левому краю
CENTER_TEXT     // Выравнивание по центру
RIGHT_TEXT      // Выравнивание по правому краю
TOP_TEXT        // Выравнивание по верху
BOTTOM_TEXT     // Выравнивание по низу
```

### Коды клавиш
Библиотека определяет коды для специальных клавиш:
- Функциональные клавиши (KEY_F1 - KEY_F12)
- Стрелки (KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT)
- Служебные клавиши (KEY_ESC, KEY_ENTER, KEY_SPACE и др.)
- Буквенные и цифровые клавиши

## Пример использования с установленной библиотекой

### Исходный код

```c
#include <graphics.h>
#include <stdio.h>

int main() {
    // Инициализация графики
    if (initgraph(800, 600, "Graphics Demo") == -1) {
        return -1;
    }
    
    // Очистка экрана
    cleardevice();
    
    // Установка цветов
    setbkcolor(BLUE);
    setcolor(YELLOW);
    
    // Рисование фигур
    rectangle(50, 50, 200, 150);
    circle(300, 100, 50);
    bar(400, 50, 550, 150);
    
    // Работа с текстом
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(400, 300, "Hello Graphics!");
    
    // Ожидание нажатия клавиши
    getch();
    
    // Завершение
    closegraph();
    return 0;
}
```

### Компиляция c динамической библиотекой

```bash
gcc -I/usr/include/cairo program.c -lgraphics -lm -o program
```