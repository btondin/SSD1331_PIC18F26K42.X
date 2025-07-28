/**
 * @file gfx_pic.h
 * @brief Basic graphics library for PIC18F26K42 (Adafruit_GFX style)
 *
 * Provides drawing primitives and text rendering using a hardware abstraction layer.
 * Compatible with various graphic displays.
 *
 * @author @btondin 
 * @date 2025
 */

#ifndef GFX_PIC_H
#define GFX_PIC_H

#include <stdint.h>
#include <stdbool.h>

//==============================================================================
// CONSTANTS
//==============================================================================

/** @brief Default font width in pixels */
#define GFX_FONT_WIDTH      6

/** @brief Default font height in pixels */
#define GFX_FONT_HEIGHT     8

//==============================================================================
// DATA STRUCTURES
//==============================================================================

/**
 * @brief 2D coordinate structure
 */
typedef struct {
    int16_t x; ///< X coordinate
    int16_t y; ///< Y coordinate
} GFX_Point_t;

/**
 * @brief Graphics context structure
 * 
 * Contains all settings and function pointers needed for hardware-independent
 * graphics operations.
 */
typedef struct {
    // Display dimensions
    int16_t width;           ///< Display width in pixels
    int16_t height;          ///< Display height in pixels
    
    // Text cursor
    int16_t cursor_x;        ///< Current text cursor X position
    int16_t cursor_y;        ///< Current text cursor Y position
    
    // Text settings
    uint16_t textcolor;      ///< Text color
    uint16_t textbgcolor;    ///< Text background color
    uint8_t textsize_x;      ///< Horizontal text scale
    uint8_t textsize_y;      ///< Vertical text scale
    
    // Display settings
    uint8_t rotation;        ///< Display rotation (0-3: 0°, 90°, 180°, 270°)
    bool wrap;               ///< Enable/disable automatic text wrapping
    bool cp437;              ///< Enable extended CP437 character set

    // Hardware-specific function pointers
    void (*drawPixel)(void *display, int16_t x, int16_t y, uint16_t color);
    void (*fillScreen)(void *display, uint16_t color);
    void (*drawFastVLine)(void *display, int16_t x, int16_t y, int16_t h, uint16_t color);
    void (*drawFastHLine)(void *display, int16_t x, int16_t y, int16_t w, uint16_t color);
    void (*fillRect)(void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void (*writePixel)(void *display, uint16_t color);
} GFX_t;

//==============================================================================
// INITIALIZATION FUNCTIONS
//==============================================================================

/**
 * @brief Initialize the graphics context
 * @param gfx Pointer to GFX_t structure
 * @param w Display width in pixels
 * @param h Display height in pixels
 */
void GFX_Init(GFX_t *gfx, int16_t w, int16_t h);

//==============================================================================
// BASIC DRAWING PRIMITIVES
//==============================================================================

/**
 * @brief Draw a pixel at the specified position
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Pixel color in display format
 */
void GFX_DrawPixel(GFX_t *gfx, void *display, int16_t x, int16_t y, uint16_t color);

/**
 * @brief Draw a line between two points
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of start point
 * @param y0 Y coordinate of start point
 * @param x1 X coordinate of end point
 * @param y1 Y coordinate of end point
 * @param color Line color
 */
void GFX_DrawLine(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

/**
 * @brief Draw a vertical line (optimized)
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of line
 * @param y Starting Y coordinate
 * @param h Height of line in pixels
 * @param color Line color
 */
void GFX_DrawFastVLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t h, uint16_t color);

/**
 * @brief Draw a horizontal line (optimized)
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x Starting X coordinate
 * @param y Y coordinate of line
 * @param w Width of line in pixels
 * @param color Line color
 */
void GFX_DrawFastHLine(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, uint16_t color);

/**
 * @brief Draw a rectangle outline
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param color Outline color
 */
void GFX_DrawRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Draw a filled rectangle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param color Fill color
 */
void GFX_FillRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Fill entire screen with a color
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param color Fill color
 */
void GFX_FillScreen(GFX_t *gfx, void *display, uint16_t color);

//==============================================================================
// CIRCLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw a circle outline
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param color Outline color
 */
void GFX_DrawCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Helper function to draw specific parts of a circle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param cornername Bitmask for corners to draw (1=top-right, 2=top-left, 4=bottom-left, 8=bottom-right)
 * @param color Outline color
 */
void GFX_DrawCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

/**
 * @brief Draw a filled circle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param color Fill color
 */
void GFX_FillCircle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Helper function to fill specific parts of a circle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of center
 * @param y0 Y coordinate of center
 * @param r Circle radius
 * @param cornername Bitmask for corners to fill
 * @param delta Additional offset for filling
 * @param color Fill color
 */
void GFX_FillCircleHelper(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

//==============================================================================
// TRIANGLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw a triangle outline
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of first vertex
 * @param y0 Y coordinate of first vertex
 * @param x1 X coordinate of second vertex
 * @param y1 Y coordinate of second vertex
 * @param x2 X coordinate of third vertex
 * @param y2 Y coordinate of third vertex
 * @param color Outline color
 */
void GFX_DrawTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**
 * @brief Draw a filled triangle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x0 X coordinate of first vertex
 * @param y0 Y coordinate of first vertex
 * @param x1 X coordinate of second vertex
 * @param y1 Y coordinate of second vertex
 * @param x2 X coordinate of third vertex
 * @param y2 Y coordinate of third vertex
 * @param color Fill color
 */
void GFX_FillTriangle(GFX_t *gfx, void *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

//==============================================================================
// ROUNDED RECTANGLE FUNCTIONS
//==============================================================================

/**
 * @brief Draw a rounded rectangle outline
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param r Corner radius
 * @param color Outline color
 */
void GFX_DrawRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

/**
 * @brief Draw a filled rounded rectangle
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width
 * @param h Rectangle height
 * @param r Corner radius
 * @param color Fill color
 */
void GFX_FillRoundRect(GFX_t *gfx, void *display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

//==============================================================================
// BITMAP FUNCTIONS
//==============================================================================

/**
 * @brief Draw a color RGB bitmap
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate for positioning
 * @param y Y coordinate for positioning
 * @param bitmap Pointer to bitmap data in RGB565 format
 * @param w Bitmap width in pixels
 * @param h Bitmap height in pixels
 */
void GFX_DrawBitmapRGB(GFX_t *gfx, void *display, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);

//==============================================================================
// TEXT RENDERING FUNCTIONS
//==============================================================================

/**
 * @brief Draw a character at specified position
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate of character
 * @param y Y coordinate of character
 * @param c Character to draw
 * @param color Character color
 * @param bg Character background color
 * @param size_x Horizontal scale (1 = normal size)
 * @param size_y Vertical scale (1 = normal size)
 */
void GFX_DrawChar(GFX_t *gfx, void *display, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

/**
 * @brief Set text cursor position
 * @param gfx Pointer to graphics context
 * @param x New cursor X coordinate
 * @param y New cursor Y coordinate
 */
void GFX_SetCursor(GFX_t *gfx, int16_t x, int16_t y);

/**
 * @brief Set text color (no background)
 * @param gfx Pointer to graphics context
 * @param c Text color
 */
void GFX_SetTextColor(GFX_t *gfx, uint16_t c);

/**
 * @brief Set text color and background color
 * @param gfx Pointer to graphics context
 * @param c Text color
 * @param bg Text background color
 */
void GFX_SetTextColorBg(GFX_t *gfx, uint16_t c, uint16_t bg);

/**
 * @brief Set text size (same for X and Y)
 * @param gfx Pointer to graphics context
 * @param s Scale factor (1 = normal size, 2 = double, etc.)
 */
void GFX_SetTextSize(GFX_t *gfx, uint8_t s);

/**
 * @brief Set text size separately for X and Y
 * @param gfx Pointer to graphics context
 * @param s_x Horizontal scale factor
 * @param s_y Vertical scale factor
 */
void GFX_SetTextSizeXY(GFX_t *gfx, uint8_t s_x, uint8_t s_y);

/**
 * @brief Enable/disable automatic text wrapping
 * @param gfx Pointer to graphics context
 * @param w true to enable, false to disable
 */
void GFX_SetTextWrap(GFX_t *gfx, bool w);

/**
 * @brief Set display rotation
 * @param gfx Pointer to graphics context
 * @param r Rotation (0=0°, 1=90°, 2=180°, 3=270°)
 */
void GFX_SetRotation(GFX_t *gfx, uint8_t r);

/**
 * @brief Enable/disable CP437 character set
 * @param gfx Pointer to graphics context
 * @param x true to enable CP437, false for standard ASCII
 */
void GFX_SetCP437(GFX_t *gfx, bool x);

/**
 * @brief Write a character at current cursor position
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param c Character to write
 */
void GFX_Write(GFX_t *gfx, void *display, uint8_t c);

/**
 * @brief Print a string at current cursor position
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param str String to print (null-terminated)
 */
void GFX_Print(GFX_t *gfx, void *display, const char *str);

/**
 * @brief Print a string at specific position
 * @param gfx Pointer to graphics context
 * @param display Pointer to display driver
 * @param x X coordinate where to print
 * @param y Y coordinate where to print
 * @param str String to print (null-terminated)
 */
void GFX_PrintAt(GFX_t *gfx, void *display, int16_t x, int16_t y, const char *str);

//==============================================================================
// GETTER FUNCTIONS
//==============================================================================

/**
 * @brief Get current cursor X coordinate
 * @param gfx Pointer to graphics context
 * @return Cursor X coordinate
 */
int16_t GFX_GetCursorX(GFX_t *gfx);

/**
 * @brief Get current cursor Y coordinate
 * @param gfx Pointer to graphics context
 * @return Cursor Y coordinate
 */
int16_t GFX_GetCursorY(GFX_t *gfx);

/**
 * @brief Get current display rotation
 * @param gfx Pointer to graphics context
 * @return Rotation value (0-3)
 */
uint8_t GFX_GetRotation(GFX_t *gfx);

/**
 * @brief Get current display width (considering rotation)
 * @param gfx Pointer to graphics context
 * @return Width in pixels
 */
int16_t GFX_Width(GFX_t *gfx);

/**
 * @brief Get current display height (considering rotation)
 * @param gfx Pointer to graphics context
 * @return Height in pixels
 */
int16_t GFX_Height(GFX_t *gfx);

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

/**
 * @brief Swap values of two integer variables
 * @param a Pointer to first variable
 * @param b Pointer to second variable
 */
void GFX_Swap(int16_t *a, int16_t *b);

//==============================================================================
// CONVENIENCE MACROS (ARDUINO STYLE)
//==============================================================================

/** @brief Macro to print string (Arduino style) */
#define print(gfx, display, str) GFX_Print(gfx, display, str)

/** @brief Macro to print string with newline (Arduino style) */
#define println(gfx, display, str) do { \
    GFX_Print(gfx, display, str); \
    GFX_Write(gfx, display, '\n'); \
} while(0)

#endif // GFX_PIC_H