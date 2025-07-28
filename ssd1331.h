/**
 * @file ssd1331.h
 * @brief SSD1331 OLED 96x64 RGB driver for PIC18F26K42
 *
 * This driver is based on the Adafruit_SSD1331 library, adapted for PIC 
 * microcontrollers using MPLAB's SPI1.h. Provides hardware-accelerated 
 * drawing functions and full color display capabilities.
 *
 * @author @btondin 
 * @date 2025
 */

#ifndef SSD1331_H
#define SSD1331_H

#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/mcc.h"
#include "gfx_pic.h"

//==============================================================================
// COLOR ORDER CONFIGURATION
//==============================================================================

/**
 * @brief Select pixel color order - choose ONE of these defines
 * RGB: Red-Green-Blue byte order
 * BGR: Blue-Green-Red byte order
 */
#define SSD1331_COLORORDER_RGB
// #define SSD1331_COLORORDER_BGR

#if defined SSD1331_COLORORDER_RGB && defined SSD1331_COLORORDER_BGR
#error "RGB and BGR can not both be defined for SSD1331_COLORORDER."
#endif

//==============================================================================
// DISPLAY SPECIFICATIONS
//==============================================================================

/** @brief Display width in pixels */
#define SSD1331_WIDTH         96

/** @brief Display height in pixels */
#define SSD1331_HEIGHT        64

/** @brief Default display rotation on initialization */
#define SSD1331_INIT_ROTATION 0

//==============================================================================
// RGB565 COLOR DEFINITIONS
//==============================================================================

#define SSD1331_BLACK         0x0000  ///< Black color (RGB565)
#define SSD1331_WHITE         0xFFFF  ///< White color (RGB565)
#define SSD1331_RED           0xF800  ///< Red color (RGB565)
#define SSD1331_GREEN         0x07E0  ///< Green color (RGB565)
#define SSD1331_BLUE          0x001F  ///< Blue color (RGB565)
#define SSD1331_CYAN          0x07FF  ///< Cyan color (RGB565)
#define SSD1331_MAGENTA       0xF81F  ///< Magenta color (RGB565)
#define SSD1331_YELLOW        0xFFE0  ///< Yellow color (RGB565)

//==============================================================================
// SSD1331 COMMAND DEFINITIONS
//==============================================================================

/* Drawing Commands */
#define SSD1331_CMD_DRAWLINE        0x21  ///< Draw line command
#define SSD1331_CMD_DRAWRECT        0x22  ///< Draw rectangle command
#define SSD1331_CMD_COPY            0x23  ///< Copy area command
#define SSD1331_CMD_DIM             0x24  ///< Dim window command
#define SSD1331_CMD_CLEAR           0x25  ///< Clear window command
#define SSD1331_CMD_FILL            0x26  ///< Fill enable/disable command

/* Scrolling Commands */
#define SSD1331_CMD_SCROLLSETUP     0x27  ///< Setup scrolling parameters
#define SSD1331_CMD_SCROLLSTOP      0x2E  ///< Stop scrolling
#define SSD1331_CMD_SCROLLSTART     0x2F  ///< Start scrolling

/* Address Setting Commands */
#define SSD1331_CMD_SETCOLUMN       0x15  ///< Set column address range
#define SSD1331_CMD_SETROW          0x75  ///< Set row address range

/* Contrast Control Commands */
#define SSD1331_CMD_CONTRASTA       0x81  ///< Set contrast for color A (Red)
#define SSD1331_CMD_CONTRASTB       0x82  ///< Set contrast for color B (Green)
#define SSD1331_CMD_CONTRASTC       0x83  ///< Set contrast for color C (Blue)
#define SSD1331_CMD_MASTERCURRENT   0x87  ///< Set master current control

/* Display Configuration Commands */
#define SSD1331_CMD_SETREMAP        0xA0  ///< Set re-map and color depth
#define SSD1331_CMD_STARTLINE       0xA1  ///< Set display start line
#define SSD1331_CMD_DISPLAYOFFSET   0xA2  ///< Set display offset
#define SSD1331_CMD_NORMALDISPLAY   0xA4  ///< Normal display mode
#define SSD1331_CMD_DISPLAYALLON    0xA5  ///< Turn on all pixels
#define SSD1331_CMD_DISPLAYALLOFF   0xA6  ///< Turn off all pixels
#define SSD1331_CMD_INVERTDISPLAY   0xA7  ///< Invert display
#define SSD1331_CMD_SETMULTIPLEX    0xA8  ///< Set multiplex ratio
#define SSD1331_CMD_SETMASTER       0xAD  ///< Set master configuration

/* Power Control Commands */
#define SSD1331_CMD_DISPLAYOFF      0xAE  ///< Turn display off
#define SSD1331_CMD_DISPLAYON       0xAF  ///< Turn display on
#define SSD1331_CMD_POWERMODE       0xB0  ///< Set power save mode

/* Timing Commands */
#define SSD1331_CMD_PRECHARGE       0xB1  ///< Set phase 1 and 2 periods
#define SSD1331_CMD_CLOCKDIV        0xB3  ///< Set display clock divide ratio
#define SSD1331_CMD_PRECHARGEA      0x8A  ///< Set second pre-charge speed for color A
#define SSD1331_CMD_PRECHARGEB      0x8B  ///< Set second pre-charge speed for color B
#define SSD1331_CMD_PRECHARGEC      0x8C  ///< Set second pre-charge speed for color C
#define SSD1331_CMD_PRECHARGELEVEL  0xBB  ///< Set pre-charge voltage level
#define SSD1331_CMD_VCOMH           0xBE  ///< Set VCOMH voltage

//==============================================================================
// TIMING DELAYS
//==============================================================================

/** @brief Delay after hardware fill operations (3ms recommended) */
#define SSD1331_DELAY_HWFILL()      __delay_ms(3)

/** @brief Delay after hardware line operations (1ms recommended) */
#define SSD1331_DELAY_HWLINE()      __delay_ms(1)

//==============================================================================
// DATA STRUCTURES
//==============================================================================

/**
 * @brief SSD1331 OLED driver structure
 * 
 * Contains the graphics context inherited from GFX_t and additional
 * SSD1331-specific parameters for display control.
 */
typedef struct {
    GFX_t gfx;        ///< Inherited graphics context from GFX library
    uint8_t rotation; ///< Current display rotation (0-3: 0°, 90°, 180°, 270°)
} SSD1331_t;

//==============================================================================
// INITIALIZATION AND CONTROL FUNCTIONS
//==============================================================================

/**
 * @brief Initialize SSD1331 driver structure
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_Init(SSD1331_t *ssd);

/**
 * @brief Initialize SSD1331 hardware and configure display
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_Begin(SSD1331_t *ssd);

/**
 * @brief Set display rotation
 * @param ssd Pointer to SSD1331 driver structure
 * @param rotation Rotation value (0=0°, 1=90°, 2=180°, 3=270°)
 */
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t rotation);

/**
 * @brief Enable or disable the display
 * @param ssd Pointer to SSD1331 driver structure
 * @param enable true to turn display on, false to turn off
 */
void SSD1331_EnableDisplay(SSD1331_t *ssd, bool enable);

//==============================================================================
// BASIC DRAWING FUNCTIONS
//==============================================================================

/**
 * @brief Draw a single pixel at specified coordinates
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Pixel color in RGB565 format
 */
void SSD1331_DrawPixel(SSD1331_t *ssd, int16_t x, int16_t y, uint16_t color);

/**
 * @brief Fill entire screen with specified color
 * @param ssd Pointer to SSD1331 driver structure
 * @param color Fill color in RGB565 format
 */
void SSD1331_FillScreen(SSD1331_t *ssd, uint16_t color);

/**
 * @brief Draw a line between two points using hardware acceleration
 * @param ssd Pointer to SSD1331 driver structure
 * @param x0 X coordinate of start point
 * @param y0 Y coordinate of start point
 * @param x1 X coordinate of end point
 * @param y1 Y coordinate of end point
 * @param color Line color in RGB565 format
 */
void SSD1331_DrawLine(SSD1331_t *ssd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

/**
 * @brief Draw rectangle outline using hardware acceleration
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width in pixels
 * @param h Rectangle height in pixels
 * @param color Outline color in RGB565 format
 */
void SSD1331_DrawRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Draw filled rectangle using hardware acceleration
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width in pixels
 * @param h Rectangle height in pixels
 * @param color Fill color in RGB565 format
 */
void SSD1331_FillRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

//==============================================================================
// ADDRESS WINDOW CONFIGURATION
//==============================================================================

/**
 * @brief Set address window for pixel data writing
 * @param ssd Pointer to SSD1331 driver structure
 * @param x Starting X coordinate
 * @param y Starting Y coordinate
 * @param w Window width in pixels
 * @param h Window height in pixels
 */
void SSD1331_SetAddrWindow(SSD1331_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

//==============================================================================
// SPI COMMUNICATION FUNCTIONS
//==============================================================================

/**
 * @brief Send command byte to SSD1331 via SPI
 * @param ssd Pointer to SSD1331 driver structure
 * @param cmd Command byte to send
 */
void SSD1331_WriteCommand(SSD1331_t *ssd, uint8_t cmd);

/**
 * @brief Send data byte to SSD1331 via SPI
 * @param ssd Pointer to SSD1331 driver structure
 * @param data Data byte to send
 */
void SSD1331_WriteData(SSD1331_t *ssd, uint8_t data);

/**
 * @brief Send 16-bit data word to SSD1331 via SPI
 * @param ssd Pointer to SSD1331 driver structure
 * @param data 16-bit data word to send (RGB565 pixel data)
 */
void SSD1331_WriteData16(SSD1331_t *ssd, uint16_t data);

//==============================================================================
// CONTRAST CONTROL FUNCTIONS
//==============================================================================

/**
 * @brief Set RGB contrast levels individually
 * @param ssd Pointer to SSD1331 driver structure
 * @param r Red contrast level (0-255)
 * @param g Green contrast level (0-255)
 * @param b Blue contrast level (0-255)
 */
void SSD1331_SetContrast(SSD1331_t *ssd, uint8_t r, uint8_t g, uint8_t b);

//==============================================================================
// UTILITY FUNCTIONS
//==============================================================================

/**
 * @brief Convert RGB888 color to RGB565 format
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return Color value in RGB565 format
 */
uint16_t SSD1331_Color565(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Software delay function
 * @param ms Delay time in milliseconds
 */
void SSD1331_Delay(uint16_t ms);

//==============================================================================
// HARDWARE-ACCELERATED SCROLL FUNCTIONS
//==============================================================================

/**
 * @brief Start hardware scrolling with previously configured parameters
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_StartScroll(SSD1331_t *ssd);

/**
 * @brief Stop hardware scrolling
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_StopScroll(SSD1331_t *ssd);

/**
 * @brief Configure hardware scrolling parameters
 * @param ssd Pointer to SSD1331 driver structure
 * @param startRow Starting row for scroll area (0-63)
 * @param endRow Ending row for scroll area (0-63)
 * @param startCol Starting column for scroll area (0-95)
 * @param endCol Ending column for scroll area (0-95)
 * @param interval Scroll speed interval (frames between scroll steps)
 */
void SSD1331_SetupScroll(SSD1331_t *ssd, uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol, uint8_t interval);

//==============================================================================
// HARDWARE-ACCELERATED SPECIAL FUNCTIONS
//==============================================================================

/**
 * @brief Copy rectangular area to another location using hardware acceleration
 * @param ssd Pointer to SSD1331 driver structure
 * @param x0 Source area top-left X coordinate
 * @param y0 Source area top-left Y coordinate
 * @param x1 Source area bottom-right X coordinate
 * @param y1 Source area bottom-right Y coordinate
 * @param x2 Destination area top-left X coordinate
 * @param y2 Destination area top-left Y coordinate
 */
void SSD1331_CopyArea(SSD1331_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * @brief Dim (reduce brightness of) specified window area
 * @param ssd Pointer to SSD1331 driver structure
 * @param x0 Window top-left X coordinate
 * @param y0 Window top-left Y coordinate
 * @param x1 Window bottom-right X coordinate
 * @param y1 Window bottom-right Y coordinate
 */
void SSD1331_DimWindow(SSD1331_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/**
 * @brief Clear (fill with black) specified window area using hardware acceleration
 * @param ssd Pointer to SSD1331 driver structure
 * @param x0 Window top-left X coordinate
 * @param y0 Window top-left Y coordinate
 * @param x1 Window bottom-right X coordinate
 * @param y1 Window bottom-right Y coordinate
 */
void SSD1331_ClearWindow(SSD1331_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

//==============================================================================
// DISPLAY MODE FUNCTIONS
//==============================================================================

/**
 * @brief Invert display colors (black becomes white, white becomes black)
 * @param ssd Pointer to SSD1331 driver structure
 * @param invert true to invert display, false for normal display
 */
void SSD1331_InvertDisplay(SSD1331_t *ssd, bool invert);

//==============================================================================
// CONVENIENCE MACROS (ARDUINO STYLE)
//==============================================================================

/** @brief Turn display on (requires 'oled' as SSD1331_t variable name) */
#define display() SSD1331_EnableDisplay(&oled, true)

/** @brief Turn display off (requires 'oled' as SSD1331_t variable name) */
#define noDisplay() SSD1331_EnableDisplay(&oled, false)

/** @brief Invert display colors (requires 'oled' as SSD1331_t variable name) */
#define invertDisplay(x) SSD1331_InvertDisplay(&oled, x)

/** @brief Start scrolling (requires 'oled' as SSD1331_t variable name) */
#define startscroll() SSD1331_StartScroll(&oled)

/** @brief Stop scrolling (requires 'oled' as SSD1331_t variable name) */
#define stopscroll() SSD1331_StopScroll(&oled)

#endif // SSD1331_H