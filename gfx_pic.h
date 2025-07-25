/**
 * @file gfx_pic.h
 * @brief Basic graphics library header for PIC18F26K42 (Adafruit_GFX style)
 *
 * Provides drawing primitives and text rendering using a hardware abstraction layer.
 * * @author  @btondin 
 * @date    2025
 */

#ifndef GFX_PIC_H
#define GFX_PIC_H

#include <stdint.h>
#include <stdbool.h>

// Default font dimensions
#define GFX_FONT_WIDTH      6
#define GFX_FONT_HEIGHT     8

/**
 * @brief 2D coordinate structure
 */
typedef struct {
    int16_t x; ///< X coordinate
    int16_t y; ///< Y coordinate
} GFX_Point_t;

/**
 * @brief Graphics context structure
 */
typedef struct {
    int16_t width;           ///< Display width in pixels
    int16_t height;          ///< Display height in pixels
    int16_t cursor_x;        ///< Current text cursor X position
    int16_t cursor_y;        ///< Current text cursor Y position
    uint16_t textcolor;      ///< Text color
    uint16_t textbgcolor;    ///< Text background color
    uint8_t textsize_x;      ///< Horizontal text scale
    uint8_t textsize_y;      ///< Vertical text scale
    uint8_t rotation;        ///< Display rotation (0-3)
    bool wrap;               ///< Enable/disable text wrapping
    bool cp437;              ///< Enable extended CP437 charset

    // Hardware-specific function pointers
    void (*drawPixel)(void *display, int16_t x, int16_t y, uint16_t color);
    void (*fillScreen)(void *display, uint16_t color);
    void (*drawFastVLine)(void *display, int16_t x, int16_t y, int16_t h, uint16_t color);
    void (*drawFastHLine)(void *display, int16_t x, int16_t y, int16_t w, uint16_t color);
    void (*fillRect)(void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void (*writePixel)(void *display, uint16_t color);
} GFX_t;

// Initialization
void GFX_Init(GFX_t *gfx, int16_t w, int16_t h);

// Drawing primitives
void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color);
void GFX_DrawLine(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color);
void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color);
void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color);

// Circle functions
void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void GFX_DrawCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

// Triangle functions
void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

// Rounded rectangle functions
void GFX_DrawRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void GFX_FillRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

// Bitmap functions
void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);

// Text rendering
void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y);
void GFX_SetTextColor(GFX_t *gfx, uint16_t c);
void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg);
void GFX_SetTextSize(GFX_t *gfx, uint8_t s);
void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y);
void GFX_SetTextWrap(GFX_t *gfx, bool w);
void GFX_SetRotation(GFX_t *gfx, uint8_t r);
void GFX_SetCP437(GFX_t *gfx, bool x);
void GFX_Write(GFX_t *gfx, void *display, uint8_t c);
void GFX_Print(GFX_t *gfx, void *display, const char *str);
void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str);

// Getters
int16_t GFX_GetCursorX(GFX_t *gfx);
int16_t GFX_GetCursorY(GFX_t *gfx);
uint8_t GFX_GetRotation(GFX_t *gfx);
int16_t GFX_Width(GFX_t *gfx);
int16_t GFX_Height(GFX_t *gfx);

// Helpers
void GFX_Swap(int16_t *a, int16_t *b);

// Arduino-style aliases for convenience
#define print(gfx, display, str) GFX_Print(gfx, display, str)
#define println(gfx, display, str) do { GFX_Print(gfx, display, str); GFX_Write(gfx, display, '\n'); } while(0)

#endif // GFX_PIC_H