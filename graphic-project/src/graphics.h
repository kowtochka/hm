#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

// ==================== КОНСТАНТЫ КЛАВИШ ====================

// Специальные клавиши
#define KEY_ESC          0xFF1B
#define KEY_ENTER        0xFF0D
#define KEY_SPACE        0x0020
#define KEY_BACKSPACE    0xFF08
#define KEY_TAB          0xFF09
#define KEY_CAPS_LOCK    0xFFE5
#define KEY_SHIFT        0xFFE1
#define KEY_CTRL         0xFFE3
#define KEY_ALT          0xFFE9

// Функциональные клавиши
#define KEY_F1           0xFFBE
#define KEY_F2           0xFFBF
#define KEY_F3           0xFFC0
#define KEY_F4           0xFFC1
#define KEY_F5           0xFFC2
#define KEY_F6           0xFFC3
#define KEY_F7           0xFFC4
#define KEY_F8           0xFFC5
#define KEY_F9           0xFFC6
#define KEY_F10          0xFFC7
#define KEY_F11          0xFFC8
#define KEY_F12          0xFFC9

// Стрелки
#define KEY_UP           0xFF52
#define KEY_DOWN         0xFF54
#define KEY_LEFT         0xFF51
#define KEY_RIGHT        0xFF53

// Клавиши навигации
#define KEY_HOME         0xFF50
#define KEY_END          0xFF57
#define KEY_PAGE_UP      0xFF55
#define KEY_PAGE_DOWN    0xFF56
#define KEY_INSERT       0xFF63
#define KEY_DELETE       0xFFFF

// Цифровые клавиши
#define KEY_0            0x0030
#define KEY_1            0x0031
#define KEY_2            0x0032
#define KEY_3            0x0033
#define KEY_4            0x0034
#define KEY_5            0x0035
#define KEY_6            0x0036
#define KEY_7            0x0037
#define KEY_8            0x0038
#define KEY_9            0x0039

// Буквенные клавиши (строчные)
#define KEY_A            0x0061
#define KEY_B            0x0062
#define KEY_C            0x0063
#define KEY_D            0x0064
#define KEY_E            0x0065
#define KEY_F            0x0066
#define KEY_G            0x0067
#define KEY_H            0x0068
#define KEY_I            0x0069
#define KEY_J            0x006A
#define KEY_K            0x006B
#define KEY_L            0x006C
#define KEY_M            0x006D
#define KEY_N            0x006E
#define KEY_O            0x006F
#define KEY_P            0x0070
#define KEY_Q            0x0071
#define KEY_R            0x0072
#define KEY_S            0x0073
#define KEY_T            0x0074
#define KEY_U            0x0075
#define KEY_V            0x0076
#define KEY_W            0x0077
#define KEY_X            0x0078
#define KEY_Y            0x0079
#define KEY_Z            0x007A

// Буквенные клавиши (заглавные)
#define KEY_A_UP         0x0041
#define KEY_B_UP         0x0042
#define KEY_C_UP         0x0043
#define KEY_D_UP         0x0044
#define KEY_E_UP         0x0045
#define KEY_F_UP         0x0046
#define KEY_G_UP         0x0047
#define KEY_H_UP         0x0048
#define KEY_I_UP         0x0049
#define KEY_J_UP         0x004A
#define KEY_K_UP         0x004B
#define KEY_L_UP         0x004C
#define KEY_M_UP         0x004D
#define KEY_N_UP         0x004E
#define KEY_O_UP         0x004F
#define KEY_P_UP         0x0050
#define KEY_Q_UP         0x0051
#define KEY_R_UP         0x0052
#define KEY_S_UP         0x0053
#define KEY_T_UP         0x0054
#define KEY_U_UP         0x0055
#define KEY_V_UP         0x0056
#define KEY_W_UP         0x0057
#define KEY_X_UP         0x0058
#define KEY_Y_UP         0x0059
#define KEY_Z_UP         0x005A

// Символьные клавиши
#define KEY_MINUS        0x002D
#define KEY_PLUS         0x002B
#define KEY_EQUAL        0x003D
#define KEY_BRACKET_LEFT 0x005B
#define KEY_BRACKET_RIGHT 0x005D
#define KEY_SEMICOLON    0x003B
#define KEY_QUOTE        0x0027
#define KEY_BACKSLASH    0x005C
#define KEY_COMMA        0x002C
#define KEY_PERIOD       0x002E
#define KEY_SLASH        0x002F
#define KEY_GRAVE        0x0060

// Цифровая клавиатура
#define KEY_KP_0         0xFFB0
#define KEY_KP_1         0xFFB1
#define KEY_KP_2         0xFFB2
#define KEY_KP_3         0xFFB3
#define KEY_KP_4         0xFFB4
#define KEY_KP_5         0xFFB5
#define KEY_KP_6         0xFFB6
#define KEY_KP_7         0xFFB7
#define KEY_KP_8         0xFFB8
#define KEY_KP_9         0xFFB9
#define KEY_KP_ADD       0xFFAB
#define KEY_KP_SUBTRACT  0xFFAD
#define KEY_KP_MULTIPLY  0xFFAA
#define KEY_KP_DIVIDE    0xFFAF
#define KEY_KP_ENTER     0xFF8D
#define KEY_KP_DECIMAL   0xFFAE

// Цвета в формате RGB
typedef struct {
    double r, g, b;
} Color;

// Структура для настроек текста
typedef struct {
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
} textsettingstype;

// Основная структура графического контекста
typedef struct {
    Display *display;
    Window window;
    cairo_surface_t *surface;
    cairo_t *cr;
    int width, height;
    Color current_color;
    Color background_color;
    int line_width;
    bool auto_flush;
    textsettingstype text_settings;
    int current_x;
    int current_y;
} GraphicsContext;

// Инициализация и завершение
int initgraph(int width, int height, const char* title);
void closegraph();

// Управление цветом
void setcolor(int color);
void setbkcolor(int color);
void setrgbcolor(double r, double g, double b);
int getbkcolor();
int getcolor();

// Функции рисования
void arc(int x, int y, int stangle, int endangle, int radius);
void bar(int left, int top, int right, int bottom);
void bar3d(int left, int top, int right, int bottom, int depth, int topflag);
void circle(int x, int y, int radius);
void cleardevice();
void clearviewport();
void drawpoly(int n_points, int* points);
void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
void fillellipse(int x, int y, int xradius, int yradius);
void fillpoly(int n_points, int* points);
void line(int x1, int y1, int x2, int y2);
void linerel(int dx, int dy);
void lineto(int x, int y);
void moverel(int dx, int dy);
void moveto(int x, int y);
void pieslice(int x, int y, int stangle, int endangle, int radius);
void putpixel(int x, int y, int color);
//int getpixel(int x, int y);
void rectangle(int left, int top, int right, int bottom);
void sector(int x, int y, int stangle, int endangle, int xradius, int yradius);

// Текстовые функции
void gettextsettings(textsettingstype *texttypeinfo);
void outtext(char *textstring);
void outtextxy(int x, int y, char *textstring);
void settextjustify(int horiz, int vert);
void settextstyle(int font, int direction, int charsize);
//void setusercharsize(int multx, int divx, int multy, int divy);
int textheight(char *textstring);
int textwidth(char *textstring);

// Управление и ввод
void delay(int msec);
int getch();
int kbhit();

// Вспомогательные функции
void flush();
void setlinestyle(int style, int thickness);
int getmaxx();
int getmaxy();
int getmaxwidth();
int getmaxheight();
int getx();
int gety();

// Предопределенные цвета
enum Colors {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

// Константы для текста
enum TextConstants {
    DEFAULT_FONT = 0,
    HORIZ_DIR = 0,
    VERT_DIR = 1,
    LEFT_TEXT = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT = 2,
    BOTTOM_TEXT = 0,
    TOP_TEXT = 2
};

#endif