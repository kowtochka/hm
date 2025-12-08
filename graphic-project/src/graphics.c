#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Глобальный контекст
static GraphicsContext gctx;

// Преобразование углов из градусов в радианы
static double to_radians(int degrees) {
    return degrees * M_PI / 180.0;
}

// Получение цвета по номеру
static Color get_predefined_color(int color) {
    Color colors[] = {
        {0.0, 0.0, 0.0},       // BLACK
        {0.0, 0.0, 1.0},       // BLUE
        {0.0, 1.0, 0.0},       // GREEN
        {0.0, 1.0, 1.0},       // CYAN
        {1.0, 0.0, 0.0},       // RED
        {1.0, 0.0, 1.0},       // MAGENTA
        {0.6, 0.4, 0.2},       // BROWN
        {0.8, 0.8, 0.8},       // LIGHTGRAY
        {0.4, 0.4, 0.4},       // DARKGRAY
        {0.5, 0.5, 1.0},       // LIGHTBLUE
        {0.5, 1.0, 0.5},       // LIGHTGREEN
        {0.5, 1.0, 1.0},       // LIGHTCYAN
        {1.0, 0.5, 0.5},       // LIGHTRED
        {1.0, 0.5, 1.0},       // LIGHTMAGENTA
        {1.0, 1.0, 0.0},       // YELLOW
        {1.0, 1.0, 1.0}        // WHITE
    };
    
    if (color >= 0 && color <= 15) {
        return colors[color];
    }
    return colors[WHITE]; // По умолчанию белый
}

// Получение номера цвета по RGB
static int get_color_number(Color color) {
    Color colors[] = {
        {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
        {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {0.6, 0.4, 0.2}, {0.8, 0.8, 0.8},
        {0.4, 0.4, 0.4}, {0.5, 0.5, 1.0}, {0.5, 1.0, 0.5}, {0.5, 1.0, 1.0},
        {1.0, 0.5, 0.5}, {1.0, 0.5, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}
    };
    
    for (int i = 0; i <= 15; i++) {
        if (fabs(colors[i].r - color.r) < 0.01 &&
            fabs(colors[i].g - color.g) < 0.01 &&
            fabs(colors[i].b - color.b) < 0.01) {
            return i;
        }
    }
    return WHITE; // По умолчанию белый
}

// Функция ожидания инициализации окна
static void wait_for_window_initialization() {
    if (!gctx.display || !gctx.window) return;
    
    // Ждем события MapNotify
    XEvent event;
    while (1) {
        XNextEvent(gctx.display, &event);
        if (event.type == MapNotify && event.xmap.window == gctx.window) {
            break;
        }
    }
    
    // Дополнительная небольшая задержка для гарантии
    usleep(100000); // 100ms
}

int initgraph(int width, int height, const char* title) {
    // Инициализация X11
    gctx.display = XOpenDisplay(NULL);
    if (!gctx.display) {
        fprintf(stderr, "Не удалось подключиться к X серверу\n");
        return -1;
    }

    int screen = DefaultScreen(gctx.display);
    Window root = RootWindow(gctx.display, screen);

    // Создание окна с обработкой событий
    XSetWindowAttributes attrs;
    attrs.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;
    
    gctx.window = XCreateWindow(gctx.display, root,
                                100, 100, width, height, 2,
                                CopyFromParent, CopyFromParent, CopyFromParent,
                                CWEventMask, &attrs);

    // Установка заголовка
    XStoreName(gctx.display, gctx.window, title);
    
    // Показ окна
    XMapWindow(gctx.display, gctx.window);
    XFlush(gctx.display);

    // Ждем инициализации окна
    wait_for_window_initialization();

    // Создание поверхности Cairo
    gctx.surface = cairo_xlib_surface_create(gctx.display, gctx.window,
                                            DefaultVisual(gctx.display, screen),
                                            width, height);
    if (cairo_surface_status(gctx.surface) != CAIRO_STATUS_SUCCESS) {
        fprintf(stderr, "Ошибка создания поверхности Cairo\n");
        return -1;
    }
    
    gctx.cr = cairo_create(gctx.surface);
    if (cairo_status(gctx.cr) != CAIRO_STATUS_SUCCESS) {
        fprintf(stderr, "Ошибка создания контекста Cairo\n");
        return -1;
    }
    
    gctx.width = width;
    gctx.height = height;
    gctx.auto_flush = true;
    gctx.line_width = 1;
    gctx.current_x = 0;
    gctx.current_y = 0;
    
    // Установка цветов по умолчанию
    gctx.current_color = get_predefined_color(WHITE);
    gctx.background_color = get_predefined_color(BLACK);
    
    // Инициализация настроек текста по умолчанию
    gctx.text_settings.font = DEFAULT_FONT;
    gctx.text_settings.direction = HORIZ_DIR;
    gctx.text_settings.charsize = 1;
    gctx.text_settings.horiz = LEFT_TEXT;
    gctx.text_settings.vert = TOP_TEXT;
    
    // Очистка экрана
    cleardevice();
    
    printf("Графическая система инициализирована: %dx%d\n", width, height);
    return 0;
}

void closegraph() {
    if (gctx.cr) {
        cairo_destroy(gctx.cr);
        gctx.cr = NULL;
    }
    if (gctx.surface) {
        cairo_surface_destroy(gctx.surface);
        gctx.surface = NULL;
    }
    if (gctx.window) {
        XDestroyWindow(gctx.display, gctx.window);
        gctx.window = 0;
    }
    if (gctx.display) {
        XCloseDisplay(gctx.display);
        gctx.display = NULL;
    }
}

// ==================== УПРАВЛЕНИЕ ЦВЕТОМ ====================

void setcolor(int color) {
    if (!gctx.cr) return;
    gctx.current_color = get_predefined_color(color);
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
}

void setbkcolor(int color) {
    gctx.background_color = get_predefined_color(color);
}

void setrgbcolor(double r, double g, double b) {
    if (!gctx.cr) return;
    gctx.current_color.r = r;
    gctx.current_color.g = g;
    gctx.current_color.b = b;
    cairo_set_source_rgb(gctx.cr, r, g, b);
}

int getbkcolor() {
    return get_color_number(gctx.background_color);
}

int getcolor() {
    return get_color_number(gctx.current_color);
}

// ==================== ФУНКЦИИ РИСОВАНИЯ ====================

void arc(int x, int y, int stangle, int endangle, int radius) {
    if (!gctx.cr) return;
    
    double start_angle = to_radians(stangle);
    double end_angle = to_radians(endangle);
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_new_sub_path(gctx.cr);
    cairo_arc(gctx.cr, x, y, radius, start_angle, end_angle);
    cairo_stroke(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void bar(int left, int top, int right, int bottom) {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_rectangle(gctx.cr, left, top, right - left, bottom - top);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void bar3d(int left, int top, int right, int bottom, int depth, int topflag) {
    if (!gctx.cr) return;
    
    // Основной прямоугольник
    bar(left, top, right, bottom);
    
    // 3D эффект - боковые грани
    Color darker = {
        gctx.current_color.r * 0.7,
        gctx.current_color.g * 0.7, 
        gctx.current_color.b * 0.7
    };
    
    cairo_set_source_rgb(gctx.cr, darker.r, darker.g, darker.b);
    
    // Правая грань
    cairo_move_to(gctx.cr, right, top);
    cairo_line_to(gctx.cr, right + depth, top - depth);
    cairo_line_to(gctx.cr, right + depth, bottom - depth);
    cairo_line_to(gctx.cr, right, bottom);
    cairo_close_path(gctx.cr);
    cairo_fill(gctx.cr);
    
    // Нижняя грань
    cairo_move_to(gctx.cr, left, bottom);
    cairo_line_to(gctx.cr, right, bottom);
    cairo_line_to(gctx.cr, right + depth, bottom - depth);
    cairo_line_to(gctx.cr, left + depth, bottom - depth);
    cairo_close_path(gctx.cr);
    cairo_fill(gctx.cr);
    
    if (topflag) {
        // Верхняя грань
        cairo_move_to(gctx.cr, left, top);
        cairo_line_to(gctx.cr, right, top);
        cairo_line_to(gctx.cr, right + depth, top - depth);
        cairo_line_to(gctx.cr, left + depth, top - depth);
        cairo_close_path(gctx.cr);
        cairo_fill(gctx.cr);
    }
    
    if (gctx.auto_flush) flush();
}

void circle(int x, int y, int radius) {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_arc(gctx.cr, x, y, radius, 0, 2 * M_PI);
    cairo_stroke(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void cleardevice() {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.background_color.r, gctx.background_color.g, gctx.background_color.b);
    cairo_paint(gctx.cr);
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    if (gctx.auto_flush) flush();
}

void clearviewport() {
    cleardevice();
}

void drawpoly(int n_points, int* points) {
    if (!gctx.cr || n_points < 2) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_move_to(gctx.cr, points[0], points[1]);
    for (int i = 1; i < n_points; i++) {
        cairo_line_to(gctx.cr, points[i * 2], points[i * 2 + 1]);
    }
    cairo_close_path(gctx.cr);
    cairo_stroke(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    if (!gctx.cr) return;
    
    double start_angle = to_radians(stangle);
    double end_angle = to_radians(endangle);
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_save(gctx.cr);
    cairo_translate(gctx.cr, x, y);
    cairo_scale(gctx.cr, xradius, yradius);
    cairo_arc(gctx.cr, 0, 0, 1, start_angle, end_angle);
    cairo_restore(gctx.cr);
    cairo_stroke(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void fillellipse(int x, int y, int xradius, int yradius) {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    cairo_save(gctx.cr);
    cairo_translate(gctx.cr, x, y);
    cairo_scale(gctx.cr, xradius, yradius);
    cairo_arc(gctx.cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_restore(gctx.cr);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void fillpoly(int n_points, int* points) {
    if (!gctx.cr || n_points < 2) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    cairo_move_to(gctx.cr, points[0], points[1]);
    for (int i = 1; i < n_points; i++) {
        cairo_line_to(gctx.cr, points[i * 2], points[i * 2 + 1]);
    }
    cairo_close_path(gctx.cr);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void line(int x1, int y1, int x2, int y2) {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_move_to(gctx.cr, x1, y1);
    cairo_line_to(gctx.cr, x2, y2);
    cairo_stroke(gctx.cr);
    
    gctx.current_x = x2;
    gctx.current_y = y2;
    
    if (gctx.auto_flush) flush();
}

void linerel(int dx, int dy) {
    line(gctx.current_x, gctx.current_y, gctx.current_x + dx, gctx.current_y + dy);
}

void lineto(int x, int y) {
    line(gctx.current_x, gctx.current_y, x, y);
}

void moverel(int dx, int dy) {
    gctx.current_x += dx;
    gctx.current_y += dy;
}

void moveto(int x, int y) {
    gctx.current_x = x;
    gctx.current_y = y;
}

void pieslice(int x, int y, int stangle, int endangle, int radius) {
    if (!gctx.cr) return;
    
    double start_angle = to_radians(stangle);
    double end_angle = to_radians(endangle);
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    cairo_move_to(gctx.cr, x, y);
    cairo_arc(gctx.cr, x, y, radius, start_angle, end_angle);
    cairo_close_path(gctx.cr);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void putpixel(int x, int y, int color) {
    if (!gctx.cr) return;
    
    Color pixel_color = get_predefined_color(color);
    cairo_set_source_rgb(gctx.cr, pixel_color.r, pixel_color.g, pixel_color.b);
    cairo_rectangle(gctx.cr, x, y, 1, 1);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}


void rectangle(int left, int top, int right, int bottom) {
    if (!gctx.cr) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    cairo_set_line_width(gctx.cr, gctx.line_width);
    
    cairo_rectangle(gctx.cr, left, top, right - left, bottom - top);
    cairo_stroke(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

void sector(int x, int y, int stangle, int endangle, int xradius, int yradius) {
    if (!gctx.cr) return;
    
    double start_angle = to_radians(stangle);
    double end_angle = to_radians(endangle);
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    cairo_save(gctx.cr);
    cairo_translate(gctx.cr, x, y);
    cairo_scale(gctx.cr, xradius, yradius);
    cairo_move_to(gctx.cr, 0, 0);
    cairo_arc(gctx.cr, 0, 0, 1, start_angle, end_angle);
    cairo_close_path(gctx.cr);
    cairo_restore(gctx.cr);
    cairo_fill(gctx.cr);
    
    if (gctx.auto_flush) flush();
}

// ==================== ТЕКСТОВЫЕ ФУНКЦИИ ====================

void gettextsettings(textsettingstype *texttypeinfo) {
    if (texttypeinfo) {
        *texttypeinfo = gctx.text_settings;
    }
}

void outtext(char *textstring) {
    outtextxy(gctx.current_x, gctx.current_y, textstring);
}

void outtextxy(int x, int y, char *textstring) {
    if (!gctx.cr || !textstring) return;
    
    cairo_set_source_rgb(gctx.cr, gctx.current_color.r, gctx.current_color.g, gctx.current_color.b);
    
    // Установка шрифта
    cairo_select_font_face(gctx.cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    
    // Размер шрифта (базовый размер * множитель)
    double font_size = 12.0 * gctx.text_settings.charsize;
    cairo_set_font_size(gctx.cr, font_size);
    
    if (gctx.text_settings.direction == HORIZ_DIR) {
        // Горизонтальный текст
        cairo_text_extents_t extents;
        cairo_text_extents(gctx.cr, textstring, &extents);
        
        double text_x = x;
        double text_y = y;
        
        // Горизонтальное выравнивание
        switch (gctx.text_settings.horiz) {
            case CENTER_TEXT:
                text_x -= extents.x_bearing + extents.width / 2;
                break;
            case RIGHT_TEXT:
                text_x -= extents.x_bearing + extents.width;
                break;
            case LEFT_TEXT:
            default:
                text_x -= extents.x_bearing;
                break;
        }
        
        // Вертикальное выравнивание
        switch (gctx.text_settings.vert) {
            case CENTER_TEXT:
                text_y -= extents.y_bearing + extents.height / 2;
                break;
            case BOTTOM_TEXT:
                text_y -= extents.y_bearing + extents.height;
                break;
            case TOP_TEXT:
            default:
                text_y -= extents.y_bearing;
                break;
        }
        
        // Рисование текста
        cairo_move_to(gctx.cr, text_x, text_y);
        cairo_show_text(gctx.cr, textstring);
        
        // Обновление текущей позиции
        gctx.current_x = x + extents.width - extents.x_bearing;
        gctx.current_y = y;
        
    } else {
        // ВЕРТИКАЛЬНЫЙ ТЕКСТ - используем поворот на 90 градусов
        cairo_save(gctx.cr); // Сохраняем состояние
        
        // Поворачиваем систему координат на 90 градусов против часовой стрелки
        cairo_translate(gctx.cr, x, y);
        cairo_rotate(gctx.cr, -M_PI / 2); // -90 градусов
        
        // Теперь работаем в повернутой системе координат
        cairo_text_extents_t extents;
        cairo_text_extents(gctx.cr, textstring, &extents);
        
        double text_x = 0;
        double text_y = 0;
        
        // В повернутой системе координат:
        // horiz становится вертикальным выравниванием
        // vert становится горизонтальным выравниванием (но инвертированным)
        
        // "Вертикальное" выравнивание в повернутой системе (было горизонтальным)
        switch (gctx.text_settings.horiz) {
            case CENTER_TEXT:
                text_y -= extents.y_bearing + extents.height / 2;
                break;
            case RIGHT_TEXT:
                text_y -= extents.y_bearing + extents.height;
                break;
            case LEFT_TEXT:
            default:
                text_y -= extents.y_bearing;
                break;
        }
        
        // "Горизонтальное" выравнивание в повернутой системе (было вертикальным)
        switch (gctx.text_settings.vert) {
            case CENTER_TEXT:
                text_x += extents.x_bearing + extents.width / 2;
                break;
            case BOTTOM_TEXT:
                text_x += extents.x_bearing + extents.width;
                break;
            case TOP_TEXT:
            default:
                text_x += extents.x_bearing;
                break;
        }
        
        // Рисование текста в повернутой системе координат
        cairo_move_to(gctx.cr, text_x, text_y);
        cairo_show_text(gctx.cr, textstring);
        
        cairo_restore(gctx.cr); // Восстанавливаем состояние
        
        // Обновление текущей позиции для вертикального текста
        // После поворота на -90 градусов, движение вниз в обычной системе
        // становится движением вправо в повернутой системе
        gctx.current_x = x;
        gctx.current_y = y + extents.width; // Ширина становится высотой в вертикальном тексте
    }
    
    if (gctx.auto_flush) flush();
}

void settextjustify(int horiz, int vert) {
    gctx.text_settings.horiz = horiz;
    gctx.text_settings.vert = vert;
}

void settextstyle(int font, int direction, int charsize) {
    gctx.text_settings.font = font;
    gctx.text_settings.direction = direction;
    gctx.text_settings.charsize = charsize;
}

int textheight(char *textstring) {
    if (!gctx.cr || !textstring) return 0;
    
    cairo_text_extents_t extents;
    cairo_text_extents(gctx.cr, textstring, &extents);
    
    if (gctx.text_settings.direction == HORIZ_DIR) {
        return (int)(extents.height + 0.5);
    } else {
        // Для вертикального текста высота = ширина обычного текста
        return (int)(extents.width + 0.5);
    }
}

int textwidth(char *textstring) {
    if (!gctx.cr || !textstring) return 0;
    
    cairo_text_extents_t extents;
    cairo_text_extents(gctx.cr, textstring, &extents);
    
    if (gctx.text_settings.direction == HORIZ_DIR) {
        return (int)(extents.width + 0.5);
    } else {
        // Для вертикального текста ширина = высота обычного текста
        return (int)(extents.height + 0.5);
    }
}

// ==================== УПРАВЛЕНИЕ И ВВОД ====================

void delay(int msec) {
    usleep(msec * 1000);
}

int getch() {
    if (!gctx.display) return -1;
    
    XEvent event;
    while (1) {
        XNextEvent(gctx.display, &event);
        if (event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            return (int)keysym;
        }
    }
}

int kbhit() {
    if (!gctx.display) return 0;
    
    // Обрабатываем все события в очереди, но возвращаем true только для KeyPress
    XEvent event;
    while (XCheckMaskEvent(gctx.display, KeyPressMask | ExposureMask | StructureNotifyMask, &event)) {
        if (event.type == KeyPress) {
            // Возвращаем событие в очередь для последующей обработки getch()
            XPutBackEvent(gctx.display, &event);
            return 1;
        }
        // Игнорируем другие события (Expose, ConfigureNotify, etc.)
    }
    return 0;
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

void flush() {
    if (gctx.surface && gctx.display) {
        cairo_surface_flush(gctx.surface);
        XFlush(gctx.display);
    }
}

void setlinestyle(int style, int thickness) {
    if (!gctx.cr) return;
    
    gctx.line_width = thickness;
    cairo_set_line_width(gctx.cr, thickness);
    
    switch (style) {
        case 0: // SOLID
            cairo_set_dash(gctx.cr, NULL, 0, 0);
            break;
        case 1: // DOTTED
            {
                double dashes[] = {2, 2};
                cairo_set_dash(gctx.cr, dashes, 2, 0);
            }
            break;
        case 2: // DASHED
            {
                double dashes[] = {8, 4};
                cairo_set_dash(gctx.cr, dashes, 2, 0);
            }
            break;
        default:
            cairo_set_dash(gctx.cr, NULL, 0, 0);
    }
}

int getmaxx() { return gctx.width; }
int getmaxy() { return gctx.height; }
int getmaxwidth() { return gctx.width; }
int getmaxheight() { return gctx.height; }
int getx() { return gctx.current_x; }
int gety() { return gctx.current_y; }