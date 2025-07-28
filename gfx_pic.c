/**
 * @file gfx_pic.c
 * @brief Basic graphics library implementation for PIC18F26K42 OLED display driver (Adafruit_GFX style)
 *
 * Provides functions to draw primitives (lines, rectangles, circles, triangles), text rendering,
 * and pixel-level operations for generic displays. This implementation serves as a hardware-
 * independent graphics layer that can be used with various display drivers.
 *
 * @author @btondin 
 * @date 2025
 */

#include "gfx_pic.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

//==============================================================================
// UTILITY MACROS
//==============================================================================

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

//==============================================================================
// DEFAULT FONT DATA (5x7 BITMAP FONT)
//==============================================================================

/**
 * @brief Default 5x7 pixel font data (adapted from Adafruit glcdfont.c)
 * 
 * Each character is represented by 5 bytes, with each byte representing
 * a column of the character. Bits are read from LSB to MSB (bottom to top).
 */
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

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

/**
 * @brief Swap values of two 16-bit integers
 * @param a Pointer to first integer
 * @param b Pointer to second integer
 */
void GFX_Swap(int16_t *a, int16_t *b) {
    int16_t t = *a;
    *a = *b;
    *b = t;
}

//==============================================================================
// INITIALIZATION FUNCTIONS
//==============================================================================

/**
 * @brief Initialize graphics context with default values
 * 
 * Sets up default parameters for text rendering, colors, and dimensions.
 * All hardware-specific function pointers are initialized to NULL and
 * should be assigned by the specific display driver.
 * 
 * @param gfx Pointer to graphics context structure
 * @param w Display width in pixels
 * @param h Display height in pixels
 */
void GFX_Init(GFX_t *gfx, int16_t w, int16_t h) {
    // Set display dimensions
    gfx->width = w;
    gfx->height = h;
    
    // Initialize text cursor position
    gfx->cursor_x = 0;
    gfx->cursor_y = 0;
    
    // Set default text properties
    gfx->textcolor = 0xFFFF;     // White text
    gfx->textbgcolor = 0xFFFF;   // White background (transparent)
    gfx->textsize_x = 1;         // Normal horizontal scale
    gfx->textsize_y = 1;         // Normal vertical scale
    
    // Set default display properties
    gfx->rotation = 0;           // No rotation
    gfx->wrap = true;            // Enable text wrapping
    gfx->cp437 = false;          // Use standard ASCII
    
    // Initialize function pointers to NULL (to be set by driver)
    gfx->drawPixel = NULL;
    gfx->fillScreen = NULL;
    gfx->drawFastVLine = NULL;
    gfx->drawFastHLine = NULL;
    gfx->fillRect = NULL;
    gfx->writePixel = NULL;
}

//==============================================================================
// BASIC PIXEL FUNCTIONS
//==============================================================================

/**
 * @brief Draw a pixel at specified coordinates
 * 
 * Calls the hardware-specific drawPixel function if available.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Pixel color
 */
void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color) {
    if (gfx->drawPixel) {
        gfx->drawPixel(display, x, y, color);
    }
}

/**
 * @brief Write pixel data directly to display
 * 
 * Used for writing pixel data when address window is already set.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param color Pixel color to write
 */
void GFX_WritePixel(GFX_t *gfx, void *display, uint16_t color) {
    if (gfx->writePixel) {
        gfx->writePixel(display, color);
    }
}

//==============================================================================
// SCREEN FILLING FUNCTIONS
//==============================================================================

/**
 * @brief Fill entire screen with specified color
 * 
 * Uses fillRect to fill the entire display area.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param color Fill color
 */
void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color) {
    GFX_FillRect(gfx, display, 0, 0, gfx->width, gfx->height, color);
}

//==============================================================================
// LINE DRAWING FUNCTIONS
//==============================================================================

/**
 * @brief Draw a line between two points using Bresenham's algorithm
 * 
 * Handles lines in any direction using the classic line drawing algorithm.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @param color Line color
 */
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

/**
 * @brief Draw a vertical line (optimized)
 * 
 * Uses hardware-optimized function if available, otherwise falls back to DrawLine.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of line
 * @param y Starting Y coordinate
 * @param h Height of line in pixels
 * @param color Line color
 */
void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (gfx->drawFastVLine) {
        gfx->drawFastVLine(display, x, y, h, color);
    } else {
        GFX_DrawLine(gfx, display, x, y, x, y + h - 1, color);
    }
}

/**
 * @brief Draw a horizontal line (optimized)
 * 
 * Uses hardware-optimized function if available, otherwise falls back to DrawLine.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x Starting X coordinate
 * @param y Y coordinate of line
 * @param w Width of line in pixels
 * @param color Line color
 */
void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (gfx->drawFastHLine) {
        gfx->drawFastHLine(display, x, y, w, color);
    } else {
        GFX_DrawLine(gfx, display, x, y, x + w - 1, y, color);
    }
}

//==============================================================================
// RECTANGLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw rectangle outline
 * 
 * Draws the four sides of a rectangle using fast line functions.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param color Outline color
 */
void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    GFX_DrawFastHLine(gfx, display, x, y, w, color);             // Top edge
    GFX_DrawFastHLine(gfx, display, x, y + h - 1, w, color);     // Bottom edge
    GFX_DrawFastVLine(gfx, display, x, y, h, color);             // Left edge
    GFX_DrawFastVLine(gfx, display, x + w - 1, y, h, color);     // Right edge
}

/**
 * @brief Fill a rectangle with specified color
 * 
 * Uses hardware-optimized fillRect if available, otherwise draws vertical lines.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param color Fill color
 */
void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (gfx->fillRect) {
        gfx->fillRect(display, x, y, w, h, color);
    } else {
        // Fallback: draw vertical lines to fill rectangle
        for (int16_t i = x; i < x + w; i++) {
            GFX_DrawFastVLine(gfx, display, i, y, h, color);
        }
    }
}

//==============================================================================
// TRIANGLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw triangle outline
 * 
 * Draws the three edges of a triangle by connecting the vertices.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of first vertex
 * @param y0 Y coordinate of first vertex
 * @param x1 X coordinate of second vertex
 * @param y1 Y coordinate of second vertex
 * @param x2 X coordinate of third vertex
 * @param y2 Y coordinate of third vertex
 * @param color Outline color
 */
void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    GFX_DrawLine(gfx, display, x0, y0, x1, y1, color);
    GFX_DrawLine(gfx, display, x1, y1, x2, y2, color);
    GFX_DrawLine(gfx, display, x2, y2, x0, y0, color);
}

/**
 * @brief Fill a triangle with specified color
 * 
 * Uses scan-line filling algorithm to fill triangle efficiently.
 * Sorts vertices by Y coordinate and fills between edges.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of first vertex
 * @param y0 Y coordinate of first vertex
 * @param x1 X coordinate of second vertex
 * @param y1 Y coordinate of second vertex
 * @param x2 X coordinate of third vertex
 * @param y2 Y coordinate of third vertex
 * @param color Fill color
 */
void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t a, b, y, last;
    
    // Sort vertices by Y coordinate (y0 <= y1 <= y2)
    if (y0 > y1) { GFX_Swap(&y0, &y1); GFX_Swap(&x0, &x1); }
    if (y1 > y2) { GFX_Swap(&y2, &y1); GFX_Swap(&x2, &x1); }
    if (y0 > y1) { GFX_Swap(&y0, &y1); GFX_Swap(&x0, &x1); }
    
    // Handle degenerate case (horizontal line)
    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        GFX_DrawFastHLine(gfx, display, a, y0, b - a + 1, color);
        return;
    }
    
    // Calculate edge deltas
    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
            dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;
    
    // Fill upper part of triangle
    if (y1 == y2) last = y1;
    else last = y1 - 1;
    
    for (y = y0; y <= last; y++) {
        a = x0 + (int16_t)(sa / dy01);
        b = x0 + (int16_t)(sb / dy02);
        sa += dx01;
        sb += dx02;
        if (a > b) GFX_Swap(&a, &b);
        GFX_DrawFastHLine(gfx, display, a, y, b - a + 1, color);
    }
    
    // Fill lower part of triangle
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    
    for (; y <= y2; y++) {
        a = x1 + (int16_t)(sa / dy12);
        b = x0 + (int16_t)(sb / dy02);
        sa += dx12;
        sb += dx02;
        if (a > b) GFX_Swap(&a, &b);
        GFX_DrawFastHLine(gfx, display, a, y, b - a + 1, color);
    }
}

//==============================================================================
// CIRCLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw circle outline using Bresenham's circle algorithm
 * 
 * Draws 8-way symmetric circle using integer arithmetic.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param color Outline color
 */
void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    // Draw cardinal points
    GFX_DrawPixel(gfx, display, x0, y0 + r, color);
    GFX_DrawPixel(gfx, display, x0, y0 - r, color);
    GFX_DrawPixel(gfx, display, x0 + r, y0, color);
    GFX_DrawPixel(gfx, display, x0 - r, y0, color);
    
    // Draw octants
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        // Draw 8 symmetric points
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

/**
 * @brief Fill a circle with specified color
 * 
 * Draws vertical center line and uses helper function for sides.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param color Fill color
 */
void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    GFX_DrawFastVLine(gfx, display, x0, y0 - r, 2 * r + 1, color);
    GFX_FillCircleHelper(gfx, display, x0, y0, r, 3, 0, color);
}

/**
 * @brief Helper function to draw quarter-circle arcs
 * 
 * Used for drawing specific portions of circles for rounded rectangles.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param cornername Bitmask for corners to draw
 * @param color Outline color
 */
void GFX_DrawCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
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
        
        if (cornername & 0x4) { // Bottom right
            GFX_DrawPixel(gfx, display, x0 + x, y0 + y, color);
            GFX_DrawPixel(gfx, display, x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) { // Top right
            GFX_DrawPixel(gfx, display, x0 + x, y0 - y, color);
            GFX_DrawPixel(gfx, display, x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) { // Bottom left
            GFX_DrawPixel(gfx, display, x0 - y, y0 + x, color);
            GFX_DrawPixel(gfx, display, x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) { // Top left
            GFX_DrawPixel(gfx, display, x0 - y, y0 - x, color);
            GFX_DrawPixel(gfx, display, x0 - x, y0 - y, color);
        }
    }
}

/**
 * @brief Helper function to fill quarter-circle sections
 * 
 * Used internally by FillCircle and FillRoundRect functions.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param corners Bitmask for corners to fill
 * @param delta Additional offset for filling
 * @param color Fill color
 */
void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r,
                          uint8_t corners, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        if (x < (y + 1)) {
            if (corners & 1) GFX_DrawFastVLine(gfx, display, x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2) GFX_DrawFastVLine(gfx, display, x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1) GFX_DrawFastVLine(gfx, display, x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2) GFX_DrawFastVLine(gfx, display, x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

//==============================================================================
// ROUNDED RECTANGLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw rounded rectangle outline
 * 
 * Combines straight lines and quarter-circle arcs for rounded corners.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param r Corner radius
 * @param color Outline color
 */
void GFX_DrawRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if (r > max_radius) r = max_radius;
    
    // Draw straight edges
    GFX_DrawFastHLine(gfx, display, x + r, y, w - 2 * r, color);         // Top
    GFX_DrawFastHLine(gfx, display, x + r, y + h - 1, w - 2 * r, color); // Bottom
    GFX_DrawFastVLine(gfx, display, x, y + r, h - 2 * r, color);         // Left
    GFX_DrawFastVLine(gfx, display, x + w - 1, y + r, h - 2 * r, color); // Right
    
    // Draw four corner arcs
    GFX_DrawCircleHelper(gfx, display, x + r, y + r, r, 1, color);                   // Top-left
    GFX_DrawCircleHelper(gfx, display, x + w - r - 1, y + r, r, 2, color);          // Top-right
    GFX_DrawCircleHelper(gfx, display, x + w - r - 1, y + h - r - 1, r, 4, color);  // Bottom-right
    GFX_DrawCircleHelper(gfx, display, x + r, y + h - r - 1, r, 8, color);          // Bottom-left
}

/**
 * @brief Fill a rounded rectangle with specified color
 * 
 * Fills center rectangle and uses circle helper for rounded corners.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param r Corner radius
 * @param color Fill color
 */
void GFX_FillRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if (r > max_radius) r = max_radius;

    // Fill center rectangle
    GFX_FillRect(gfx, display, x + r, y, w - 2 * r, h, color);
    
    // Fill rounded corners
    GFX_FillCircleHelper(gfx, display, x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    GFX_FillCircleHelper(gfx, display, x + r, y + r, r, 2, h - 2 * r - 1, color);
}

//==============================================================================
// TEXT RENDERING FUNCTIONS
//==============================================================================

/**
 * @brief Draw a single character at specified position
 * 
 * Renders character using built-in 5x7 font with scaling support.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate for character
 * @param y Y coordinate for character
 * @param c Character to draw
 * @param color Foreground color
 * @param bg Background color
 * @param size_x Horizontal scaling factor
 * @param size_y Vertical scaling factor
 */
void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) {
    // Check if character fits on screen
    if ((x >= gfx->width) || (y >= gfx->height) || 
       ((x + 6 * size_x - 1) < 0) || ((y + 8 * size_y - 1) < 0)) return;
    
    // Handle non-printable characters
    if (c < ' ' || c > '~') c = ' ';
    
    // Draw character bitmap
    for (int8_t i = 0; i < 5; i++) {
        uint8_t line = font[(c - ' ') * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 0x01) {
                // Draw foreground pixel
                if (size_x == 1 && size_y == 1) {
                    GFX_DrawPixel(gfx, display, x + i, y + j, color);
                } else {
                    GFX_FillRect(gfx, display, x + i * size_x, y + j * size_y, size_x, size_y, color);
                }
            } else if (bg != color) {
                // Draw background pixel
                if (size_x == 1 && size_y == 1) {
                    GFX_DrawPixel(gfx, display, x + i, y + j, bg);
                } else {
                    GFX_FillRect(gfx, display, x + i * size_x, y + j * size_y, size_x, size_y, bg);
                }
            }
        }
    }
    
    // Draw spacing column
    if (bg != color) {
        if (size_x == 1 && size_y == 1) {
            GFX_DrawFastVLine(gfx, display, x + 5, y, 8, bg);
        } else {
            GFX_FillRect(gfx, display, x + 5 * size_x, y, size_x, 8 * size_y, bg);
        }
    }
}

/**
 * @brief Write a character at current cursor position
 * 
 * Handles special characters like newline and carriage return.
 * Updates cursor position and handles text wrapping.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param c Character to write
 */
void GFX_Write(GFX_t *gfx, void *display, uint8_t c) {
    if (c == '\n') {
        // Newline: move to start of next line
        gfx->cursor_x = 0;
        gfx->cursor_y += gfx->textsize_y * 8;
    } else if (c == '\r') {
        // Carriage return: move to start of current line
        gfx->cursor_x = 0;
    } else {
        // Check for text wrapping
        if (gfx->wrap && (gfx->cursor_x + gfx->textsize_x * 6 > gfx->width)) {
            gfx->cursor_x = 0;
            gfx->cursor_y += gfx->textsize_y * 8;
        }
        
        // Draw character and advance cursor
        GFX_DrawChar(gfx, display, gfx->cursor_x, gfx->cursor_y, c, 
                     gfx->textcolor, gfx->textbgcolor, gfx->textsize_x, gfx->textsize_y);
        gfx->cursor_x += gfx->textsize_x * 6;
    }
}

/**
 * @brief Print a string at current cursor position
 * 
 * Prints each character in the string sequentially.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param str Null-terminated string to print
 */
void GFX_Print(GFX_t *gfx, void *display, const char *str) {
    while (*str) {
        GFX_Write(gfx, display, *str++);
    }
}

/**
 * @brief Print a string at specified position
 * 
 * Sets cursor position and prints string.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate for text
 * @param y Y coordinate for text
 * @param str Null-terminated string to print
 */
void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str) {
    GFX_SetCursor(gfx, x, y);
    GFX_Print(gfx, display, str);
}

//==============================================================================
// BITMAP FUNCTIONS
//==============================================================================

/**
 * @brief Draw RGB bitmap image
 * 
 * Draws bitmap data directly to display using RGB565 format.
 * 
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver instance
 * @param x X coordinate for bitmap placement
 * @param y Y coordinate for bitmap placement
 * @param bitmap Pointer to RGB565 bitmap data
 * @param w Bitmap width in pixels
 * @param h Bitmap height in pixels
 */
void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            gfx->drawPixel(display, x + i, y + j, bitmap[j * w + i]);
        }
    }
}

//==============================================================================
// TEXT CONFIGURATION FUNCTIONS
//==============================================================================

/**
 * @brief Set text cursor position
 * @param gfx Pointer to graphics context
 * @param x New cursor X coordinate
 * @param y New cursor Y coordinate
 */
void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y) {
    gfx->cursor_x = x;
    gfx->cursor_y = y;
}

/**
 * @brief Set text color (transparent background)
 * @param gfx Pointer to graphics context
 * @param c Text color
 */
void GFX_SetTextColor(GFX_t *gfx, uint16_t c) {
    gfx->textcolor = gfx->textbgcolor = c;
}

/**
 * @brief Set text color with background
 * @param gfx Pointer to graphics context
 * @param c Text color
 * @param bg Background color
 */
void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg) {
    gfx->textcolor = c;
    gfx->textbgcolor = bg;
}

/**
 * @brief Set text size (same for both X and Y)
 * @param gfx Pointer to graphics context
 * @param s Scale factor (1 = normal, 2 = double, etc.)
 */
void GFX_SetTextSize(GFX_t *gfx, uint8_t s) {
    gfx->textsize_x = s;
    gfx->textsize_y = s;
}

/**
 * @brief Set text size separately for X and Y axes
 * @param gfx Pointer to graphics context
 * @param s_x Horizontal scale factor
 * @param s_y Vertical scale factor
 */
void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y) {
    gfx->textsize_x = s_x;
    gfx->textsize_y = s_y;
}

/**
 * @brief Enable/disable text wrapping
 * @param gfx Pointer to graphics context
 * @param w true to enable wrapping, false to disable
 */
void GFX_SetTextWrap(GFX_t *gfx, bool w) {
    gfx->wrap = w;
}

/**
 * @brief Set display rotation
 * Note: Hardware-specific driver should handle dimension swapping
 * @param gfx Pointer to graphics context
 * @param r Rotation value (0-3)
 */
void GFX_SetRotation(GFX_t *gfx, uint8_t r) {
    gfx->rotation = (r & 3);
}

/**
 * @brief Enable/disable CP437 character set
 * @param gfx Pointer to graphics context
 * @param x true to enable CP437, false for standard ASCII
 */
void GFX_SetCP437(GFX_t *gfx, bool x) {
    gfx->cp437 = x;
}

//==============================================================================
// GETTER FUNCTIONS
//==============================================================================

/**
 * @brief Get current cursor X position
 * @param gfx Pointer to graphics context
 * @return Current cursor X coordinate
 */
int16_t GFX_GetCursorX(GFX_t *gfx) { 
    return gfx->cursor_x; 
}

/**
 * @brief Get current cursor Y position
 * @param gfx Pointer to graphics context
 * @return Current cursor Y coordinate
 */
int16_t GFX_GetCursorY(GFX_t *gfx) { 
    return gfx->cursor_y; 
}

/**
 * @brief Get current rotation setting
 * @param gfx Pointer to graphics context
 * @return Current rotation value (0-3)
 */
uint8_t GFX_GetRotation(GFX_t *gfx) { 
    return gfx->rotation; 
}

/**
 * @brief Get display width (considering rotation)
 * @param gfx Pointer to graphics context
 * @return Current display width in pixels
 */
int16_t GFX_Width(GFX_t *gfx) { 
    return gfx->width; 
}

/**
 * @brief Get display height (considering rotation)
 * @param gfx Pointer to graphics context
 * @return Current display height in pixels
 */
int16_t GFX_Height(GFX_t *gfx) { 
    return gfx->height; 
}