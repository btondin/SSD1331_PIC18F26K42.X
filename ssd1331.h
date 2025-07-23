/**
 * @file    ssd1331_pic.h
 * @brief   SSD1331 OLED 96x64 RGB driver for PIC18F26K42
 *
 * This driver is based on the Adafruit_SSD1331 library,
 * adapted for PIC microcontrollers using MPLAB's SPI1.h
 *
 * @author  @btondin 
 * @date    2025
 */

#ifndef SSD1331_H
#define SSD1331_H

#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/mcc.h"
#include "gfx_pic.h"


/*!
 * @brief Select one of these defines to set the pixel color order
 */
#define SSD1331_COLORORDER_RGB
// #define SSD1331_COLORORDER_BGR

#if defined SSD1331_COLORORDER_RGB && defined SSD1331_COLORORDER_BGR
#error "RGB and BGR can not both be defined for SSD1331_COLORORDER."
#endif

/* Display dimensions */
#define SSD1331_WIDTH         96
#define SSD1331_HEIGHT        64

/* Default rotation */
#define SSD1331_INIT_ROTATION 0

/* RGB565 color definitions */
#define SSD1331_BLACK         0x0000
#define SSD1331_WHITE         0xFFFF
#define SSD1331_RED           0xF800
#define SSD1331_GREEN         0x07E0
#define SSD1331_BLUE          0x001F
#define SSD1331_CYAN          0x07FF
#define SSD1331_MAGENTA       0xF81F
#define SSD1331_YELLOW        0xFFE0

/* SSD1331 Command Definitions */
#define SSD1331_CMD_DRAWLINE        0x21
#define SSD1331_CMD_DRAWRECT        0x22
#define SSD1331_CMD_COPY            0x23
#define SSD1331_CMD_DIM             0x24
#define SSD1331_CMD_CLEAR           0x25
#define SSD1331_CMD_FILL            0x26
#define SSD1331_CMD_SCROLLSETUP     0x27
#define SSD1331_CMD_SCROLLSTOP      0x2E
#define SSD1331_CMD_SCROLLSTART     0x2F
#define SSD1331_CMD_SETCOLUMN       0x15
#define SSD1331_CMD_SETROW          0x75
#define SSD1331_CMD_CONTRASTA       0x81
#define SSD1331_CMD_CONTRASTB       0x82
#define SSD1331_CMD_CONTRASTC       0x83
#define SSD1331_CMD_MASTERCURRENT   0x87
#define SSD1331_CMD_SETREMAP        0xA0
#define SSD1331_CMD_STARTLINE       0xA1
#define SSD1331_CMD_DISPLAYOFFSET   0xA2
#define SSD1331_CMD_NORMALDISPLAY   0xA4
#define SSD1331_CMD_DISPLAYALLON    0xA5
#define SSD1331_CMD_DISPLAYALLOFF   0xA6
#define SSD1331_CMD_INVERTDISPLAY   0xA7
#define SSD1331_CMD_SETMULTIPLEX    0xA8
#define SSD1331_CMD_SETMASTER       0xAD
#define SSD1331_CMD_DISPLAYOFF      0xAE
#define SSD1331_CMD_DISPLAYON       0xAF
#define SSD1331_CMD_POWERMODE       0xB0
#define SSD1331_CMD_PRECHARGE       0xB1
#define SSD1331_CMD_CLOCKDIV        0xB3
#define SSD1331_CMD_PRECHARGEA      0x8A
#define SSD1331_CMD_PRECHARGEB      0x8B
#define SSD1331_CMD_PRECHARGEC      0x8C
#define SSD1331_CMD_PRECHARGELEVEL  0xBB
#define SSD1331_CMD_VCOMH           0xBE

/* Recommended delays (ms) */
#define SSD1331_DELAY_HWFILL()      __delay_ms(3)
#define SSD1331_DELAY_HWLINE()      __delay_ms(1)

/**
 * @brief SSD1331 driver structure
 */
typedef struct {
    GFX_t gfx;        ///< Inherits from GFX structure
    uint8_t rotation; ///< Current rotation (0-3)
} SSD1331_t;

/* Initialization and control */
void SSD1331_Init(SSD1331_t *ssd);
void SSD1331_Begin(SSD1331_t *ssd);
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t rotation);
void SSD1331_EnableDisplay(SSD1331_t *ssd, bool enable);

/* Basic drawing functions */
void SSD1331_DrawPixel(SSD1331_t *ssd, int16_t x, int16_t y, uint16_t color);
void SSD1331_FillScreen(SSD1331_t *ssd, uint16_t color);
void SSD1331_DrawLine(SSD1331_t *ssd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void SSD1331_DrawRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void SSD1331_FillRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/* Address window configuration */
void SSD1331_SetAddrWindow(SSD1331_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/* SPI communication */
void SSD1331_WriteCommand(SSD1331_t *ssd, uint8_t cmd);
void SSD1331_WriteData(SSD1331_t *ssd, uint8_t data);
void SSD1331_WriteData16(SSD1331_t *ssd, uint16_t data);

/* Contrast control */
void SSD1331_SetContrast(SSD1331_t *ssd, uint8_t r, uint8_t g, uint8_t b);

/* Utility functions */
uint16_t SSD1331_Color565(uint8_t r, uint8_t g, uint8_t b);
void SSD1331_Delay(uint16_t ms);

#endif // SSD1331_H
