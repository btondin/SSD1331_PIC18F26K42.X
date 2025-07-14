/**
 * @file    ssd1331_pic.c
 * @brief   Driver implementation for SSD1331 96x64 RGB OLED display for PIC18F26K42
 * 
 * @author  Adapted for PIC
 * @date    2025
 */

#include "ssd1331.h"
#include "gfx_pic.h"

// Helper macro
#define ssd1331_swap(a, b) { int16_t t = a; a = b; b = t; }

// Helper variable
uint8_t SPI_dummy;

// Private function prototypes
static void SSD1331_HardwareReset(SSD1331_t *ssd);
static void SSD1331_Select(SSD1331_t *ssd);
static void SSD1331_Deselect(SSD1331_t *ssd);
static void SSD1331_SetDataMode(SSD1331_t *ssd);
static void SSD1331_SetCommandMode(SSD1331_t *ssd);

/**
 * @brief Millisecond delay (adjust to your system clock)
 */
void SSD1331_Delay(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}

/**
 * @brief Initializes SSD1331 driver structure
 */
void SSD1331_Init(SSD1331_t *ssd) {
    ssd->rotation = SSD1331_INIT_ROTATION;

    GFX_Init(&ssd->gfx, SSD1331_WIDTH, SSD1331_HEIGHT);

    ssd->gfx.drawPixel     = (void*)SSD1331_DrawPixel;
    ssd->gfx.fillScreen    = (void*)SSD1331_FillScreen;
    ssd->gfx.drawFastVLine = NULL;
    ssd->gfx.drawFastHLine = NULL;
    ssd->gfx.fillRect      = (void*)SSD1331_FillRect;

    SSD1331_CS_SetDigitalOutput();
    SSD1331_DC_SetDigitalOutput();
    SSD1331_RST_SetDigitalOutput();

    SSD1331_CS_SetHigh();
    SSD1331_DC_SetHigh();
    SSD1331_RST_SetHigh();
}

/**
 * @brief Initializes the SSD1331 display
 */
void SSD1331_Begin(SSD1331_t *ssd) {
    SPI1_Open(SPI1_DEFAULT);
    SSD1331_HardwareReset(ssd);

    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFF);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);
    SSD1331_WriteCommand(ssd, 0x72);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_STARTLINE);
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFFSET);
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_NORMALDISPLAY);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMULTIPLEX);
    SSD1331_WriteCommand(ssd, 0x3F);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMASTER);
    SSD1331_WriteCommand(ssd, 0x8E);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_POWERMODE);
    SSD1331_WriteCommand(ssd, 0x0B);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGE);
    SSD1331_WriteCommand(ssd, 0x31);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CLOCKDIV);
    SSD1331_WriteCommand(ssd, 0xF0);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEA);
    SSD1331_WriteCommand(ssd, 0x64);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEB);
    SSD1331_WriteCommand(ssd, 0x78);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEC);
    SSD1331_WriteCommand(ssd, 0x64);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGELEVEL);
    SSD1331_WriteCommand(ssd, 0x3A);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_VCOMH);
    SSD1331_WriteCommand(ssd, 0x3E);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_MASTERCURRENT);
    SSD1331_WriteCommand(ssd, 0x06);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTA);
    SSD1331_WriteCommand(ssd, 0x91);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTB);
    SSD1331_WriteCommand(ssd, 0x50);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTC);
    SSD1331_WriteCommand(ssd, 0x7D);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYON);

    SSD1331_FillScreen(ssd, SSD1331_CYAN);
}

/**
 * @brief Sets display rotation
 */
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t rotation) {
    ssd->rotation = rotation & 3;
    ssd->gfx.rotation = ssd->rotation;

    switch (ssd->rotation) {
        case 0:
        case 2:
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
        case 1:
        case 3:
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
    }
}

/**
 * @brief Sets the address window for drawing
 */
void SSD1331_SetAddrWindow(SSD1331_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint8_t x1 = x;
    uint8_t y1 = y;

    if (x1 > ssd->gfx.width - 1) x1 = ssd->gfx.width - 1;
    if (y1 > ssd->gfx.height - 1) y1 = ssd->gfx.height - 1;

    uint8_t x2 = (x + w - 1);
    uint8_t y2 = (y + h - 1);

    if (x2 > ssd->gfx.width - 1) x2 = ssd->gfx.width - 1;
    if (y2 > ssd->gfx.height - 1) y2 = ssd->gfx.height - 1;

    if (x1 > x2) ssd1331_swap(x1, x2);
    if (y1 > y2) ssd1331_swap(y1, y2);

    if (ssd->rotation & 1) {
        ssd1331_swap(x1, y1);
        ssd1331_swap(x2, y2);
    }

    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETCOLUMN);
    SSD1331_WriteCommand(ssd, x1);
    SSD1331_WriteCommand(ssd, x2);

    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETROW);
    SSD1331_WriteCommand(ssd, y1);
    SSD1331_WriteCommand(ssd, y2);
}

/**
 * @brief Draws a single pixel
 */
void SSD1331_DrawPixel(SSD1331_t *ssd, int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= ssd->gfx.width) || (y < 0) || (y >= ssd->gfx.height)) return;

    switch (ssd->rotation) {
        case 1:
            ssd1331_swap(x, y);
            x = SSD1331_WIDTH - 1 - x;
            break;
        case 2:
            x = SSD1331_WIDTH - 1 - x;
            y = SSD1331_HEIGHT - 1 - y;
            break;
        case 3:
            ssd1331_swap(x, y);
            y = SSD1331_HEIGHT - 1 - y;
            break;
    }

    SSD1331_SetAddrWindow(ssd, x, y, 1, 1);
    SSD1331_WriteData16(ssd, color);
}

/**
 * @brief Fills the entire screen with a color
 */
void SSD1331_FillScreen(SSD1331_t *ssd, uint16_t color) {
    SSD1331_FillRect(ssd, 0, 0, ssd->gfx.width, ssd->gfx.height, color);
}

/**
 * @brief Draws a line using hardware acceleration
 */
void SSD1331_DrawLine(SSD1331_t *ssd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if ((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0)) {
        GFX_DrawLine(&ssd->gfx, ssd, x0, y0, x1, y1, color);
        return;
    }

    if (x0 >= ssd->gfx.width) x0 = ssd->gfx.width - 1;
    if (y0 >= ssd->gfx.height) y0 = ssd->gfx.height - 1;
    if (x1 >= ssd->gfx.width) x1 = ssd->gfx.width - 1;
    if (y1 >= ssd->gfx.height) y1 = ssd->gfx.height - 1;

    SSD1331_WriteCommand(ssd, SSD1331_CMD_DRAWLINE);
    SSD1331_WriteCommand(ssd, x0);
    SSD1331_WriteCommand(ssd, y0);
    SSD1331_WriteCommand(ssd, x1);
    SSD1331_WriteCommand(ssd, y1);
    SSD1331_WriteCommand(ssd, (color >> 11) << 1);
    SSD1331_WriteCommand(ssd, (color >> 5) & 0x3F);
    SSD1331_WriteCommand(ssd, (color << 1) & 0x3F);

    SSD1331_DELAY_HWLINE();
}


/**
 * @brief Draws a rectangle using hardware acceleration (no fill)
 */
void SSD1331_DrawRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    SSD1331_WriteCommand(ssd, SSD1331_CMD_FILL);
    SSD1331_WriteCommand(ssd, 0x00); // Disable fill

    SSD1331_WriteCommand(ssd, SSD1331_CMD_DRAWRECT);
    SSD1331_WriteCommand(ssd, x);
    SSD1331_WriteCommand(ssd, y);
    SSD1331_WriteCommand(ssd, x + w - 1);
    SSD1331_WriteCommand(ssd, y + h - 1);
    SSD1331_WriteCommand(ssd, (color >> 11) << 1);
    SSD1331_WriteCommand(ssd, (color >> 5) & 0x3F);
    SSD1331_WriteCommand(ssd, (color << 1) & 0x3F);
    SSD1331_WriteCommand(ssd, 0);
    SSD1331_WriteCommand(ssd, 0);
    SSD1331_WriteCommand(ssd, 0);

    SSD1331_DELAY_HWLINE();
}

/**
 * @brief Enables or disables the display
 */
void SSD1331_EnableDisplay(SSD1331_t *ssd, bool enable) {
    SSD1331_WriteCommand(ssd, enable ? SSD1331_CMD_DISPLAYON : SSD1331_CMD_DISPLAYOFF);
}

/**
 * @brief Fills a rectangle using hardware acceleration
 */
void SSD1331_FillRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if ((x >= ssd->gfx.width) || (y >= ssd->gfx.height)) return;
    if ((x + w - 1) >= ssd->gfx.width) w = ssd->gfx.width - x;
    if ((y + h - 1) >= ssd->gfx.height) h = ssd->gfx.height - y;

    SSD1331_WriteCommand(ssd, SSD1331_CMD_FILL);
    SSD1331_WriteCommand(ssd, 0x01); // Enable fill

    SSD1331_WriteCommand(ssd, SSD1331_CMD_DRAWRECT);
    SSD1331_WriteCommand(ssd, x);
    SSD1331_WriteCommand(ssd, y);
    SSD1331_WriteCommand(ssd, x + w - 1);
    SSD1331_WriteCommand(ssd, y + h - 1);
    SSD1331_WriteCommand(ssd, (color >> 11) << 1);
    SSD1331_WriteCommand(ssd, (color >> 5) & 0x3F);
    SSD1331_WriteCommand(ssd, (color << 1) & 0x3F);
    SSD1331_WriteCommand(ssd, (color >> 11) << 1);
    SSD1331_WriteCommand(ssd, (color >> 5) & 0x3F);
    SSD1331_WriteCommand(ssd, (color << 1) & 0x3F);

    SSD1331_DELAY_HWFILL();
}

/**
 * @brief Writes a command byte to the display
 */
void SSD1331_WriteCommand(SSD1331_t *ssd, uint8_t cmd) {
    SSD1331_Select(ssd);
    SSD1331_SetCommandMode(ssd);
    SPI_dummy = SPI1_ExchangeByte(cmd);
    SSD1331_Deselect(ssd);
}

/**
 * @brief Writes a single data byte to the display
 */
void SSD1331_WriteData(SSD1331_t *ssd, uint8_t data) {
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    SPI_dummy = SPI1_ExchangeByte(data);
    SSD1331_Deselect(ssd);
}

/**
 * @brief Writes a 16-bit data word to the display
 */
void SSD1331_WriteData16(SSD1331_t *ssd, uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = data >> 8;
    buffer[1] = data & 0xFF;

    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    SPI1_ExchangeBlock(buffer, 2);
    SSD1331_Deselect(ssd);
}

/**
 * @brief Sets contrast for each RGB channel
 */
void SSD1331_SetContrast(SSD1331_t *ssd, uint8_t r, uint8_t g, uint8_t b) {
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTA);
    SSD1331_WriteCommand(ssd, r);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTB);
    SSD1331_WriteCommand(ssd, g);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTC);
    SSD1331_WriteCommand(ssd, b);
}

/**
 * @brief Converts RGB values to RGB565 format
 */
uint16_t SSD1331_Color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/* ==== Private Low-Level Functions ==== */

static void SSD1331_HardwareReset(SSD1331_t *ssd) {
    SSD1331_RST_SetHigh();
    SSD1331_Delay(10);
    SSD1331_RST_SetLow();
    SSD1331_Delay(10);
    SSD1331_RST_SetHigh();
    SSD1331_Delay(10);
}

static void SSD1331_Select(SSD1331_t *ssd) {
    SSD1331_CS_SetLow();
}

static void SSD1331_Deselect(SSD1331_t *ssd) {
    SSD1331_CS_SetHigh();
}

static void SSD1331_SetDataMode(SSD1331_t *ssd) {
    SSD1331_DC_SetHigh();
}

static void SSD1331_SetCommandMode(SSD1331_t *ssd) {
    SSD1331_DC_SetLow();
}
