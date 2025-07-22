/**
 * @file gfx_pic.c
 * @brief Basic graphics library for PIC18F26K42 OLED display driver (Adafruit_GFX style)
 *
 * Provides functions to draw primitives (lines, rectangles, circles), text,
 * and handle pixel-level operations for generic displays.
 */

#include "gfx_pic.h"
#include <string.h>
#include <stdlib.h>

// Default 5x7 font (adapted from Adafruit glcdfont.c)
static const uint8_t font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,   // (space)
    0x00, 0x00, 0x5F, 0x00, 0x00,   // !
    0x00, 0x07, 0x00, 0x07, 0x00,   // "
    0x14, 0x7F, 0x14, 0x7F, 0x14,   // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12,   // $
    0x23, 0x13, 0x08, 0x64, 0x62,   // %
    0x36, 0x49, 0x55, 0x22, 0x50,   // &
    0x00, 0x05, 0x03, 0x00, 0x00,   // '
    0x00, 0x1C, 0x22, 0x41, 0x00,   // (
    0x00, 0x41, 0x22, 0x1C, 0x00,   // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08,   // *
    0x08, 0x08, 0x3E, 0x08, 0x08,   // +
    0x00, 0x50, 0x30, 0x00, 0x00,   // ,
    0x08, 0x08, 0x08, 0x08, 0x08,   // -
    0x00, 0x60, 0x60, 0x00, 0x00,   // .
    0x20, 0x10, 0x08, 0x04, 0x02,   // /
    0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
    0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
    0x42, 0x61, 0x51, 0x49, 0x46,   // 2
    0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
    0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
    0x27, 0x45, 0x45, 0x45, 0x39,   // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
    0x01, 0x71, 0x09, 0x05, 0x03,   // 7
    0x36, 0x49, 0x49, 0x49, 0x36,   // 8
    0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
    0x00, 0x36, 0x36, 0x00, 0x00,   // :
    0x00, 0x56, 0x36, 0x00, 0x00,   // ;
    0x00, 0x08, 0x14, 0x22, 0x41,   // <
    0x14, 0x14, 0x14, 0x14, 0x14,   // =
    0x41, 0x22, 0x14, 0x08, 0x00,   // >
    0x02, 0x01, 0x51, 0x09, 0x06,   // ?
    0x32, 0x49, 0x79, 0x41, 0x3E,   // @
    0x7E, 0x11, 0x11, 0x11, 0x7E,   // A
    0x7F, 0x49, 0x49, 0x49, 0x36,   // B
    0x3E, 0x41, 0x41, 0x41, 0x22,   // C
    0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
    0x7F, 0x49, 0x49, 0x49, 0x41,   // E
    0x7F, 0x09, 0x09, 0x01, 0x01,   // F
    0x3E, 0x41, 0x41, 0x51, 0x32,   // G
    0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
    0x00, 0x41, 0x7F, 0x41, 0x00,   // I
    0x20, 0x40, 0x41, 0x3F, 0x01,   // J
    0x7F, 0x08, 0x14, 0x22, 0x41,   // K
    0x7F, 0x40, 0x40, 0x40, 0x40,   // L
    0x7F, 0x02, 0x04, 0x02, 0x7F,   // M
    0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
    0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
    0x7F, 0x09, 0x09, 0x09, 0x06,   // P
    0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
    0x7F, 0x09, 0x19, 0x29, 0x46,   // R
    0x46, 0x49, 0x49, 0x49, 0x31,   // S
    0x01, 0x01, 0x7F, 0x01, 0x01,   // T
    0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
    0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
    0x7F, 0x20, 0x18, 0x20, 0x7F,   // W
    0x63, 0x14, 0x08, 0x14, 0x63,   // X
    0x03, 0x04, 0x78, 0x04, 0x03,   // Y
    0x61, 0x51, 0x49, 0x45, 0x43,   // Z
    0x00, 0x00, 0x7F, 0x41, 0x41,   // [
    0x02, 0x04, 0x08, 0x10, 0x20,   // backslash
    0x41, 0x41, 0x7F, 0x00, 0x00,   // ]
    0x04, 0x02, 0x01, 0x02, 0x04,   // ^
    0x40, 0x40, 0x40, 0x40, 0x40,   // _
    0x00, 0x01, 0x02, 0x04, 0x00,   // `
    0x20, 0x54, 0x54, 0x54, 0x78,   // a
    0x7F, 0x48, 0x44, 0x44, 0x38,   // b
    0x38, 0x44, 0x44, 0x44, 0x20,   // c
    0x38, 0x44, 0x44, 0x48, 0x7F,   // d
    0x38, 0x54, 0x54, 0x54, 0x18,   // e
    0x08, 0x7E, 0x09, 0x01, 0x02,   // f
    0x08, 0x14, 0x54, 0x54, 0x3C,   // g
    0x7F, 0x08, 0x04, 0x04, 0x78,   // h
    0x00, 0x44, 0x7D, 0x40, 0x00,   // i
    0x20, 0x40, 0x44, 0x3D, 0x00,   // j
    0x00, 0x7F, 0x10, 0x28, 0x44,   // k
    0x00, 0x41, 0x7F, 0x40, 0x00,   // l
    0x7C, 0x04, 0x18, 0x04, 0x78,   // m
    0x7C, 0x08, 0x04, 0x04, 0x78,   // n
    0x38, 0x44, 0x44, 0x44, 0x38,   // o
    0x7C, 0x14, 0x14, 0x14, 0x08,   // p
    0x08, 0x14, 0x14, 0x18, 0x7C,   // q
    0x7C, 0x08, 0x04, 0x04, 0x08,   // r
    0x48, 0x54, 0x54, 0x54, 0x20,   // s
    0x04, 0x3F, 0x44, 0x40, 0x20,   // t
    0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
    0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
    0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
    0x44, 0x28, 0x10, 0x28, 0x44,   // x
    0x0C, 0x50, 0x50, 0x50, 0x3C,   // y
    0x44, 0x64, 0x54, 0x4C, 0x44,   // z
    0x00, 0x08, 0x36, 0x41, 0x00,   // {
    0x00, 0x00, 0x7F, 0x00, 0x00,   // |
    0x00, 0x41, 0x36, 0x08, 0x00,   // }
    0x08, 0x08, 0x2A, 0x1C, 0x08,   // ~
    0x08, 0x1C, 0x2A, 0x08, 0x08    // DEL
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
    gfx->wrap = true;
    gfx->cp437 = false;

    gfx->drawPixel = NULL;
    gfx->fillScreen = NULL;
    gfx->drawFastVLine = NULL;
    gfx->drawFastHLine = NULL;
    gfx->fillRect = NULL;
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

void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{   
   GFX_DrawLine(gfx, display, x0, y0, x1, y1, color);
   GFX_DrawLine(gfx, display, x1, y1, x2, y2, color);
   GFX_DrawLine(gfx, display, x2, y2, x0, y0, color);
}

/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    gfx     Pointer to the GFX structure (display graphics context)
    @param    display Pointer to the specific display
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
*/
/**************************************************************************/
void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, 
                      int16_t x1, int16_t y1, int16_t x2, int16_t y2, 
                      uint16_t color) {
    int16_t a, b, y, last;
    
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        GFX_Swap(&y0, &y1);
        GFX_Swap(&x0, &x1);
    }
    if (y1 > y2) {
        GFX_Swap(&y2, &y1);
        GFX_Swap(&x2, &x1);
    }
    if (y0 > y1) {
        GFX_Swap(&y0, &y1);
        GFX_Swap(&x0, &x1);
    }
    
    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        GFX_DrawFastHLine(gfx, display, a, y0, b - a + 1, color);
        return;
    }
    
    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
            dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;
    
    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1; // Include y1 scanline
    else
        last = y1 - 1; // Skip it
        
    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            GFX_Swap(&a, &b);
        GFX_DrawFastHLine(gfx, display, a, y, b - a + 1, color);
    }
    
    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            GFX_Swap(&a, &b);
        GFX_DrawFastHLine(gfx, display, a, y, b - a + 1, color);
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

/**
 * @brief Draw a single character to the screen
 */
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

/**
 * @brief Draws a colored RGB565 image to the screen.
 *
 * This function iterates through the provided pixel matrix and draws each pixel on the display
 * using the pixel drawing function configured in gfx->drawPixel.
 * 
 * @param gfx     Pointer to the GFX structure (display graphics context)
 * @param display Pointer to the specific display (e.g., SSD1331_t *)
 * @param x       X position where the image will be drawn
 * @param y       Y position where the image will be drawn
 * @param bitmap  Pointer to the RGB565 image data (16 bits per pixel)
 * @param w       Width of the image in pixels
 * @param h       Height of the image in pixels
 */
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


/**
 * @brief Write a character to the screen with cursor update
 */
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

/**
 * @brief Print a null-terminated string
 */
void GFX_Print(GFX_t *gfx, void *display, const char *str) {
    while (*str) {
        GFX_Write(gfx, display, *str++);
    }
}

/**
 * @brief Print a string at a specific coordinate
 */
void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str) {
    GFX_SetCursor(gfx, x, y);
    GFX_Print(gfx, display, str);
}

/**
 * @brief Set the text cursor position
 */
void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y) {
    gfx->cursor_x = x;
    gfx->cursor_y = y;
}

/**
 * @brief Set the text color
 */
void GFX_SetTextColor(GFX_t *gfx, uint16_t c) {
    gfx->textcolor = c;
    gfx->textbgcolor = c;
}


/**
 * @brief Set the text color and background color
 */
void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg) {
    gfx->textcolor = c;
    gfx->textbgcolor = bg;
}

/**
 * @brief Set the text size uniformly
 */
void GFX_SetTextSize(GFX_t *gfx, uint8_t s) {
    gfx->textsize_x = s;
    gfx->textsize_y = s;
}

/**
 * @brief Set the text size independently for X and Y
 */
void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y) {
    gfx->textsize_x = s_x;
    gfx->textsize_y = s_y;
}

/**
 * @brief Enable or disable automatic text wrapping
 */
void GFX_SetTextWrap(GFX_t *gfx, bool w) {
    gfx->wrap = w;
}

/**
 * @brief Set the display rotation
 */
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

/**
 * @brief Get the current cursor X position
 */
int16_t GFX_GetCursorX(GFX_t *gfx) {
    return gfx->cursor_x;
}

/**
 * @brief Get the current cursor Y position
 */
int16_t GFX_GetCursorY(GFX_t *gfx) {
    return gfx->cursor_y;
}

/**
 * @brief Get the current display rotation
 */
uint8_t GFX_GetRotation(GFX_t *gfx) {
    return gfx->rotation;
}

/**
 * @brief Get the current display width
 */
int16_t GFX_Width(GFX_t *gfx) {
    return gfx->width;
}

/**
 * @brief Get the current display height
 */
int16_t GFX_Height(GFX_t *gfx) {
    return gfx->height;
}