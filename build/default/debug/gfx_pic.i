# 1 "gfx_pic.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 285 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "gfx_pic.c" 2








# 1 "./gfx_pic.h" 1
# 11 "./gfx_pic.h"
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 1 3



# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3
# 26 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 3
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 3
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdint.h" 2 3
# 12 "./gfx_pic.h" 2
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdbool.h" 1 3
# 13 "./gfx_pic.h" 2








typedef struct {
    int16_t x;
    int16_t y;
} GFX_Point_t;




typedef struct {
    int16_t width;
    int16_t height;
    int16_t cursor_x;
    int16_t cursor_y;
    uint16_t textcolor;
    uint16_t textbgcolor;
    uint8_t textsize_x;
    uint8_t textsize_y;
    uint8_t rotation;
    _Bool wrap;
    _Bool cp437;


    void (*drawPixel)(void *display, int16_t x, int16_t y, uint16_t color);
    void (*fillScreen)(void *display, uint16_t color);
    void (*drawFastVLine)(void *display, int16_t x, int16_t y, int16_t h, uint16_t color);
    void (*drawFastHLine)(void *display, int16_t x, int16_t y, int16_t w, uint16_t color);
    void (*fillRect)(void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
} GFX_t;





void GFX_Init(GFX_t *gfx, int16_t w, int16_t h);





void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color);




void GFX_DrawLine(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);




void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color);




void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color);




void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);




void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);




void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color);





void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);




void GFX_DrawCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);




void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);




void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);





void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);




void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);





void GFX_DrawRoundRect(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);




void GFX_FillRoundRect(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);





void GFX_DrawBitmap(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);




void GFX_DrawBitmapBg(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);




void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);





void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);




void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y);




void GFX_SetTextColor(GFX_t *gfx, uint16_t c);




void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg);




void GFX_SetTextSize(GFX_t *gfx, uint8_t s);




void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y);




void GFX_SetTextWrap(GFX_t *gfx, _Bool w);




void GFX_SetRotation(GFX_t *gfx, uint8_t r);




void GFX_SetCP437(GFX_t *gfx, _Bool x);




void GFX_Write(GFX_t *gfx, void *display, uint8_t c);




void GFX_Print(GFX_t *gfx, void *display, const char *str);




void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str);


int16_t GFX_GetCursorX(GFX_t *gfx);
int16_t GFX_GetCursorY(GFX_t *gfx);
uint8_t GFX_GetRotation(GFX_t *gfx);
int16_t GFX_Width(GFX_t *gfx);
int16_t GFX_Height(GFX_t *gfx);





void GFX_Swap(int16_t *a, int16_t *b);
# 10 "gfx_pic.c" 2
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/string.h" 1 3
# 10 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/string.h" 3
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/features.h" 1 3
# 11 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/string.h" 2 3
# 25 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/string.h" 3
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 1 3
# 128 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned size_t;
# 421 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef struct __locale_struct * locale_t;
# 26 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/string.h" 2 3

void *memcpy (void *restrict, const void *restrict, size_t);
void *memmove (void *, const void *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void *memchr (const void *, int, size_t);

char *strcpy (char *restrict, const char *restrict);
char *strncpy (char *restrict, const char *restrict, size_t);

char *strcat (char *restrict, const char *restrict);
char *strncat (char *restrict, const char *restrict, size_t);

int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);

int strcoll (const char *, const char *);
size_t strxfrm (char *restrict, const char *restrict, size_t);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *restrict, const char *restrict);

size_t strlen (const char *);

char *strerror (int);




char *strtok_r (char *restrict, const char *restrict, char **restrict);
int strerror_r (int, char *, size_t);
char *stpcpy(char *restrict, const char *restrict);
char *stpncpy(char *restrict, const char *restrict, size_t);
size_t strnlen (const char *, size_t);
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *restrict, const char *restrict, size_t, locale_t);




void *memccpy (void *restrict, const void *restrict, int, size_t);
# 11 "gfx_pic.c" 2
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdlib.h" 1 3
# 21 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdlib.h" 3
# 1 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 1 3
# 24 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/bits/alltypes.h" 3
typedef long int wchar_t;
# 22 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdlib.h" 2 3

int atoi (const char *);
long atol (const char *);

long long atoll (const char *);

double atof (const char *);


float strtof (const char *restrict, char **restrict);
double strtod (const char *restrict, char **restrict);
long double strtold (const char *restrict, char **restrict);



long strtol (const char *restrict, char **restrict, int);
unsigned long strtoul (const char *restrict, char **restrict, int);

long long strtoll (const char *restrict, char **restrict, int);
unsigned long long strtoull (const char *restrict, char **restrict, int);


unsigned long __strtoxl(const char * s, char ** endptr, int base, char is_signed);

unsigned long long __strtoxll(const char * s, char ** endptr, int base, char is_signed);
# 55 "D:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include\\c99/stdlib.h" 3
int rand (void);
void srand (unsigned);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);

          void abort (void);
int atexit (void (*) (void));
          void exit (int);
          void _Exit (int);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));







__attribute__((nonreentrant)) void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);

long long llabs (long long);


typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;

typedef struct { long long quot, rem; } lldiv_t;


div_t div (int, int);
ldiv_t ldiv (long, long);

lldiv_t lldiv (long long, long long);


typedef struct { unsigned int quot, rem; } udiv_t;
typedef struct { unsigned long quot, rem; } uldiv_t;
udiv_t udiv (unsigned int, unsigned int);
uldiv_t uldiv (unsigned long, unsigned long);
# 12 "gfx_pic.c" 2


static const uint8_t font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x5F, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x07, 0x00,
    0x14, 0x7F, 0x14, 0x7F, 0x14,
    0x24, 0x2A, 0x7F, 0x2A, 0x12,
    0x23, 0x13, 0x08, 0x64, 0x62,
    0x36, 0x49, 0x55, 0x22, 0x50,
    0x00, 0x05, 0x03, 0x00, 0x00,
    0x00, 0x1C, 0x22, 0x41, 0x00,
    0x00, 0x41, 0x22, 0x1C, 0x00,
    0x08, 0x2A, 0x1C, 0x2A, 0x08,
    0x08, 0x08, 0x3E, 0x08, 0x08,
    0x00, 0x50, 0x30, 0x00, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x60, 0x60, 0x00, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02,
    0x3E, 0x51, 0x49, 0x45, 0x3E,
    0x00, 0x42, 0x7F, 0x40, 0x00,
    0x42, 0x61, 0x51, 0x49, 0x46,
    0x21, 0x41, 0x45, 0x4B, 0x31,
    0x18, 0x14, 0x12, 0x7F, 0x10,
    0x27, 0x45, 0x45, 0x45, 0x39,
    0x3C, 0x4A, 0x49, 0x49, 0x30,
    0x01, 0x71, 0x09, 0x05, 0x03,
    0x36, 0x49, 0x49, 0x49, 0x36,
    0x06, 0x49, 0x49, 0x29, 0x1E,
    0x00, 0x36, 0x36, 0x00, 0x00,
    0x00, 0x56, 0x36, 0x00, 0x00,
    0x00, 0x08, 0x14, 0x22, 0x41,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x41, 0x22, 0x14, 0x08, 0x00,
    0x02, 0x01, 0x51, 0x09, 0x06,
    0x32, 0x49, 0x79, 0x41, 0x3E,
    0x7E, 0x11, 0x11, 0x11, 0x7E,
    0x7F, 0x49, 0x49, 0x49, 0x36,
    0x3E, 0x41, 0x41, 0x41, 0x22,
    0x7F, 0x41, 0x41, 0x22, 0x1C,
    0x7F, 0x49, 0x49, 0x49, 0x41,
    0x7F, 0x09, 0x09, 0x01, 0x01,
    0x3E, 0x41, 0x41, 0x51, 0x32,
    0x7F, 0x08, 0x08, 0x08, 0x7F,
    0x00, 0x41, 0x7F, 0x41, 0x00,
    0x20, 0x40, 0x41, 0x3F, 0x01,
    0x7F, 0x08, 0x14, 0x22, 0x41,
    0x7F, 0x40, 0x40, 0x40, 0x40,
    0x7F, 0x02, 0x04, 0x02, 0x7F,
    0x7F, 0x04, 0x08, 0x10, 0x7F,
    0x3E, 0x41, 0x41, 0x41, 0x3E,
    0x7F, 0x09, 0x09, 0x09, 0x06,
    0x3E, 0x41, 0x51, 0x21, 0x5E,
    0x7F, 0x09, 0x19, 0x29, 0x46,
    0x46, 0x49, 0x49, 0x49, 0x31,
    0x01, 0x01, 0x7F, 0x01, 0x01,
    0x3F, 0x40, 0x40, 0x40, 0x3F,
    0x1F, 0x20, 0x40, 0x20, 0x1F,
    0x7F, 0x20, 0x18, 0x20, 0x7F,
    0x63, 0x14, 0x08, 0x14, 0x63,
    0x03, 0x04, 0x78, 0x04, 0x03,
    0x61, 0x51, 0x49, 0x45, 0x43,
    0x00, 0x00, 0x7F, 0x41, 0x41,
    0x02, 0x04, 0x08, 0x10, 0x20,
    0x41, 0x41, 0x7F, 0x00, 0x00,
    0x04, 0x02, 0x01, 0x02, 0x04,
    0x40, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x01, 0x02, 0x04, 0x00,
    0x20, 0x54, 0x54, 0x54, 0x78,
    0x7F, 0x48, 0x44, 0x44, 0x38,
    0x38, 0x44, 0x44, 0x44, 0x20,
    0x38, 0x44, 0x44, 0x48, 0x7F,
    0x38, 0x54, 0x54, 0x54, 0x18,
    0x08, 0x7E, 0x09, 0x01, 0x02,
    0x08, 0x14, 0x54, 0x54, 0x3C,
    0x7F, 0x08, 0x04, 0x04, 0x78,
    0x00, 0x44, 0x7D, 0x40, 0x00,
    0x20, 0x40, 0x44, 0x3D, 0x00,
    0x00, 0x7F, 0x10, 0x28, 0x44,
    0x00, 0x41, 0x7F, 0x40, 0x00,
    0x7C, 0x04, 0x18, 0x04, 0x78,
    0x7C, 0x08, 0x04, 0x04, 0x78,
    0x38, 0x44, 0x44, 0x44, 0x38,
    0x7C, 0x14, 0x14, 0x14, 0x08,
    0x08, 0x14, 0x14, 0x18, 0x7C,
    0x7C, 0x08, 0x04, 0x04, 0x08,
    0x48, 0x54, 0x54, 0x54, 0x20,
    0x04, 0x3F, 0x44, 0x40, 0x20,
    0x3C, 0x40, 0x40, 0x20, 0x7C,
    0x1C, 0x20, 0x40, 0x20, 0x1C,
    0x3C, 0x40, 0x30, 0x40, 0x3C,
    0x44, 0x28, 0x10, 0x28, 0x44,
    0x0C, 0x50, 0x50, 0x50, 0x3C,
    0x44, 0x64, 0x54, 0x4C, 0x44,
    0x00, 0x08, 0x36, 0x41, 0x00,
    0x00, 0x00, 0x7F, 0x00, 0x00,
    0x00, 0x41, 0x36, 0x08, 0x00,
    0x08, 0x08, 0x2A, 0x1C, 0x08,
    0x08, 0x1C, 0x2A, 0x08, 0x08
};

void GFX_Swap(int16_t *a, int16_t *b) {
    int16_t t = *a;
    *a = *b;
    *b = t;
}

void GFX_Init(GFX_t *gfx, int16_t w, int16_t h) {
    gfx->width = w;
    gfx->height = h;
    gfx->cursor_x = 0;
    gfx->cursor_y = 0;
    gfx->textcolor = 0xFFFF;
    gfx->textbgcolor = 0xFFFF;
    gfx->textsize_x = 1;
    gfx->textsize_y = 1;
    gfx->rotation = 0;
    gfx->wrap = 1;
    gfx->cp437 = 0;

    gfx->drawPixel = ((void*)0);
    gfx->fillScreen = ((void*)0);
    gfx->drawFastVLine = ((void*)0);
    gfx->drawFastHLine = ((void*)0);
    gfx->fillRect = ((void*)0);
}

void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color) {
    if (gfx->drawPixel) {
        gfx->drawPixel(display, x, y, color);
    }
}

void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color) {
    if (gfx->fillScreen) {
        gfx->fillScreen(display, color);
    } else {
        GFX_FillRect(gfx, display, 0, 0, gfx->width, gfx->height, color);
    }
}

void GFX_DrawLine(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep) {
        GFX_Swap(&x0, &y0);
        GFX_Swap(&x1, &y1);
    }
    if (x0 > x1) {
        GFX_Swap(&x0, &x1);
        GFX_Swap(&y0, &y1);
    }

    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) {
            GFX_DrawPixel(gfx, display, y0, x0, color);
        } else {
            GFX_DrawPixel(gfx, display, x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (gfx->drawFastVLine) {
        gfx->drawFastVLine(display, x, y, h, color);
    } else {
        GFX_DrawLine(gfx, display, x, y, x, y + h - 1, color);
    }
}

void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (gfx->drawFastHLine) {
        gfx->drawFastHLine(display, x, y, w, color);
    } else {
        GFX_DrawLine(gfx, display, x, y, x + w - 1, y, color);
    }
}

void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    GFX_DrawFastHLine(gfx, display, x, y, w, color);
    GFX_DrawFastHLine(gfx, display, x, y + h - 1, w, color);
    GFX_DrawFastVLine(gfx, display, x, y, h, color);
    GFX_DrawFastVLine(gfx, display, x + w - 1, y, h, color);
}

void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (gfx->fillRect) {
        gfx->fillRect(display, x, y, w, h, color);
    } else {
        for (int16_t i = x; i < x + w; i++) {
            GFX_DrawFastVLine(gfx, display, i, y, h, color);
        }
    }
}

void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    GFX_DrawPixel(gfx, display, x0, y0 + r, color);
    GFX_DrawPixel(gfx, display, x0, y0 - r, color);
    GFX_DrawPixel(gfx, display, x0 + r, y0, color);
    GFX_DrawPixel(gfx, display, x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        GFX_DrawPixel(gfx, display, x0 + x, y0 + y, color);
        GFX_DrawPixel(gfx, display, x0 - x, y0 + y, color);
        GFX_DrawPixel(gfx, display, x0 + x, y0 - y, color);
        GFX_DrawPixel(gfx, display, x0 - x, y0 - y, color);
        GFX_DrawPixel(gfx, display, x0 + y, y0 + x, color);
        GFX_DrawPixel(gfx, display, x0 - y, y0 + x, color);
        GFX_DrawPixel(gfx, display, x0 + y, y0 - x, color);
        GFX_DrawPixel(gfx, display, x0 - y, y0 - x, color);
    }
}

void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    GFX_DrawFastVLine(gfx, display, x0, y0 - r, 2 * r + 1, color);
    GFX_FillCircleHelper(gfx, display, x0, y0, r, 3, 0, color);
}

void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r,
                          uint8_t corners, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corners & 0x1) {
            GFX_DrawFastVLine(gfx, display, x0 + x, y0 - y, 2 * y + 1 + delta, color);
            GFX_DrawFastVLine(gfx, display, x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (corners & 0x2) {
            GFX_DrawFastVLine(gfx, display, x0 - x, y0 - y, 2 * y + 1 + delta, color);
            GFX_DrawFastVLine(gfx, display, x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}




void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) {
    if ((x >= gfx->width) || (y >= gfx->height) || ((x + 6 * size_x - 1) < 0) || ((y + 8 * size_y - 1) < 0)) return;

    if (c < ' ') c = ' ';
    if (c > '~') c = '~';

    for (int8_t i = 0; i < 5; i++) {
        uint8_t line = font[(c - ' ') * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 0x01) {
                if (size_x == 1 && size_y == 1) {
                    GFX_DrawPixel(gfx, display, x + i, y + j, color);
                } else {
                    GFX_FillRect(gfx, display, x + i * size_x, y + j * size_y, size_x, size_y, color);
                }
            } else if (bg != color) {
                if (size_x == 1 && size_y == 1) {
                    GFX_DrawPixel(gfx, display, x + i, y + j, bg);
                } else {
                    GFX_FillRect(gfx, display, x + i * size_x, y + j * size_y, size_x, size_y, bg);
                }
            }
        }
    }

    if (bg != color) {
        if (size_x == 1 && size_y == 1) {
            GFX_DrawFastVLine(gfx, display, x + 5, y, 8, bg);
        } else {
            GFX_FillRect(gfx, display, x + 5 * size_x, y, size_x, 8 * size_y, bg);
        }
    }
}
# 335 "gfx_pic.c"
void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y,
                       const uint16_t *bitmap, int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            int16_t index = j * w + i;
            uint16_t color = bitmap[index];
            gfx->drawPixel(display, x + i, y + j, color);
        }
    }
}





void GFX_Write(GFX_t *gfx, void *display, uint8_t c) {
    if(c == '\n') {
        gfx->cursor_x = 0;
        gfx->cursor_y += gfx->textsize_y * 8;
    } else if(c == '\r') {
        gfx->cursor_x = 0;
    } else {
        GFX_DrawChar(gfx, display, gfx->cursor_x, gfx->cursor_y, c, gfx->textcolor, gfx->textbgcolor, gfx->textsize_x, gfx->textsize_y);
        gfx->cursor_x += gfx->textsize_x * 6;
        if(gfx->wrap && (gfx->cursor_x > (gfx->width - gfx->textsize_x * 6))) {
            gfx->cursor_x = 0;
            gfx->cursor_y += gfx->textsize_y * 8;
        }
    }
}




void GFX_Print(GFX_t *gfx, void *display, const char *str) {
    while (*str) {
        GFX_Write(gfx, display, *str++);
    }
}




void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str) {
    GFX_SetCursor(gfx, x, y);
    GFX_Print(gfx, display, str);
}




void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y) {
    gfx->cursor_x = x;
    gfx->cursor_y = y;
}




void GFX_SetTextColor(GFX_t *gfx, uint16_t c) {
    gfx->textcolor = c;
    gfx->textbgcolor = c;
}





void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg) {
    gfx->textcolor = c;
    gfx->textbgcolor = bg;
}




void GFX_SetTextSize(GFX_t *gfx, uint8_t s) {
    gfx->textsize_x = s;
    gfx->textsize_y = s;
}




void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y) {
    gfx->textsize_x = s_x;
    gfx->textsize_y = s_y;
}




void GFX_SetTextWrap(GFX_t *gfx, _Bool w) {
    gfx->wrap = w;
}




void GFX_SetRotation(GFX_t *gfx, uint8_t r) {
    gfx->rotation = (r & 3);
    switch (gfx->rotation) {
        case 0:
        case 2:
            break;
        case 1:
        case 3:
            GFX_Swap(&gfx->width, &gfx->height);
            break;
    }
}




int16_t GFX_GetCursorX(GFX_t *gfx) {
    return gfx->cursor_x;
}




int16_t GFX_GetCursorY(GFX_t *gfx) {
    return gfx->cursor_y;
}




uint8_t GFX_GetRotation(GFX_t *gfx) {
    return gfx->rotation;
}




int16_t GFX_Width(GFX_t *gfx) {
    return gfx->width;
}




int16_t GFX_Height(GFX_t *gfx) {
    return gfx->height;
}
