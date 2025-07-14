/**
 * @file gfx_pic.h
 * @brief Basic graphics library header for PIC18F26K42 (Adafruit_GFX style)
 *
 * Provides drawing primitives and text rendering using a hardware abstraction layer.
 * 
 * @author  @btondin 
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
    uint8_t rotation;        ///< Display rotation (0?3)
    bool wrap;               ///< Enable/disable text wrapping
    bool cp437;              ///< Enable extended CP437 charset

    // Hardware-specific function pointers
    void (*drawPixel)(void *display, int16_t x, int16_t y, uint16_t color);
    void (*fillScreen)(void *display, uint16_t color);
    void (*drawFastVLine)(void *display, int16_t x, int16_t y, int16_t h, uint16_t color);
    void (*drawFastHLine)(void *display, int16_t x, int16_t y, int16_t w, uint16_t color);
    void (*fillRect)(void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
} GFX_t;

// Initialization
/**
 * @brief Initialize GFX context with given width and height.
 */
void GFX_Init(GFX_t *gfx, int16_t w, int16_t h);

// Drawing primitives
/**
 * @brief Draw a pixel at the specified coordinates.
 */
void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color);

/**
 * @brief Draw a line between two points.
 */
void GFX_DrawLine(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

/**
 * @brief Draw a vertical line.
 */
void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color);

/**
 * @brief Draw a horizontal line.
 */
void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color);

/**
 * @brief Draw a rectangle outline.
 */
void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Fill a rectangle area.
 */
void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Fill the entire screen.
 */
void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color);

// Circle functions
/**
 * @brief Draw a circle outline.
 */
void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Draw helper arcs for circles.
 */
void GFX_DrawCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

/**
 * @brief Fill a circle area.
 */
void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Fill circle quadrants.
 */
void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

// Triangle functions
/**
 * @brief Draw a triangle outline.
 */
void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**
 * @brief Fill a triangle.
 */
void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

// Rounded rectangle functions
/**
 * @brief Draw a rounded rectangle outline.
 */
void GFX_DrawRoundRect(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

/**
 * @brief Fill a rounded rectangle.
 */
void GFX_FillRoundRect(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

// Bitmap functions
/**
 * @brief Draw a monochrome bitmap (1-bit per pixel, foreground only).
 */
void GFX_DrawBitmap(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Draw a monochrome bitmap with background color.
 */
void GFX_DrawBitmapBg(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);

/**
 * @brief Draw a full-color RGB565 bitmap.
 */
void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);

// Text rendering
/**
 * @brief Draw a single character at the specified position.
 */
void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

/**
 * @brief Set text cursor position.
 */
void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y);

/**
 * @brief Set text color (foreground and background same).
 */
void GFX_SetTextColor(GFX_t *gfx, uint16_t c);

/**
 * @brief Set text color and background color.
 */
void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg);

/**
 * @brief Set uniform text size scaling.
 */
void GFX_SetTextSize(GFX_t *gfx, uint8_t s);

/**
 * @brief Set text size scaling separately in X and Y directions.
 */
void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y);

/**
 * @brief Enable or disable text wrapping.
 */
void GFX_SetTextWrap(GFX_t *gfx, bool w);

/**
 * @brief Set display rotation (0?3).
 */
void GFX_SetRotation(GFX_t *gfx, uint8_t r);

/**
 * @brief Enable or disable CP437 character set.
 */
void GFX_SetCP437(GFX_t *gfx, bool x);

/**
 * @brief Write a character with automatic cursor update.
 */
void GFX_Write(GFX_t *gfx, void *display, uint8_t c);

/**
 * @brief Print a null-terminated string.
 */
void GFX_Print(GFX_t *gfx, void *display, const char *str);

/**
 * @brief Print string at specified screen position.
 */
void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str);

// Getters
int16_t GFX_GetCursorX(GFX_t *gfx);
int16_t GFX_GetCursorY(GFX_t *gfx);
uint8_t GFX_GetRotation(GFX_t *gfx);
int16_t GFX_Width(GFX_t *gfx);
int16_t GFX_Height(GFX_t *gfx);

// Helpers
/**
 * @brief Swap two integer values.
 */
void GFX_Swap(int16_t *a, int16_t *b);

#endif // GFX_PIC_H
