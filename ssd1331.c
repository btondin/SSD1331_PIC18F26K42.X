/**
 * @file    ssd1331_pic.c
 * @brief   Driver implementation for SSD1331 96x64 RGB OLED display for PIC18F26K42
 * 
 * @author  @btondin 
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

    // Initialization Sequence
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFF);      // 0xAE
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);        // 0xA0
    SSD1331_WriteCommand(ssd, SSD1331_CMD_STARTLINE);       // 0xA1
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFFSET);   // 0xA2
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_NORMALDISPLAY);   // 0xA4
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMULTIPLEX);    // 0xA8
    SSD1331_WriteCommand(ssd, 0x3F);                        // 0x3F 1/64 duty
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMASTER);       // 0xAD
    SSD1331_WriteCommand(ssd, 0x8E);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_POWERMODE);       // 0xB0
    SSD1331_WriteCommand(ssd, 0x0B);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGE);       // 0xB1
    SSD1331_WriteCommand(ssd, 0x31);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CLOCKDIV);        // 0xB3
    SSD1331_WriteCommand(ssd, 0xF0);                        // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio
                                                            // (A[3:0]+1 = 1..16)
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEA);      // 0x8A
    SSD1331_WriteCommand(ssd, 0x64);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEB);      // 0x8B
    SSD1331_WriteCommand(ssd, 0x78);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEC);      // 0x8C
    SSD1331_WriteCommand(ssd, 0x64);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGELEVEL);  // 0xBB
    SSD1331_WriteCommand(ssd, 0x3A);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_VCOMH);           // 0xBE
    SSD1331_WriteCommand(ssd, 0x3E);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_MASTERCURRENT);   // 0x87
    SSD1331_WriteCommand(ssd, 0x06);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTA);       // 0x81
    SSD1331_WriteCommand(ssd, 0x91);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTB);       // 0x82
    SSD1331_WriteCommand(ssd, 0x50);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTC);       // 0x83
    SSD1331_WriteCommand(ssd, 0x7D);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYON);       // --turn on oled panel
    
    // Set width and height
    ssd->gfx.width = SSD1331_WIDTH;
    ssd->gfx.height = SSD1331_HEIGHT;
    
    // Set rotation 0 (like Arduino does)
    SSD1331_SetRotation(ssd, SSD1331_INIT_ROTATION);
}

/**
 * @brief Sets origin of (0,0) and orientation of OLED display
 * @param ssd Pointer to SSD1331 structure
 * @param r Rotation index (0-3)
 * @note SSD1331 works differently than most other displays. With certain 
 *       rotation changes the screen contents may change immediately into a 
 *       peculiar format (mirrored, not necessarily rotated). Therefore, it's 
 *       recommended to clear the screen (fillScreen(0)) before changing rotation.
 */
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t r) {
    // madctl bits:
    // 6,7 Color depth (01 = 64K)
    // 5   Odd/even split COM (0: disable, 1: enable)
    // 4   Scan direction (0: top-down, 1: bottom-up)
    // 3   Left-Right swapping on COM (0: disable, 1: enable)
    // 2   Color remap (0: A->B->C, 1: C->B->A)
    // 1   Column remap (0: 0-95, 1: 95-0)
    // 0   Address increment (0: horizontal, 1: vertical)
    
    #ifdef SSD1331_COLORORDER_RGB
        uint8_t madctl = 0b01100000; // 64K, enable split, ABC
    #else
        uint8_t madctl = 0b01100100; // 64K, enable split, CBA (default)
    #endif
    
    ssd->rotation = r & 3;  // Clip input to valid range
    ssd->gfx.rotation = ssd->rotation;
    
    switch (ssd->rotation) {
        case 0:
            madctl |= 0b00010010;  // Scan bottom-up, column remap 95-0
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
            
        case 1:
            madctl |= 0b00000011;  // Column remap 95-0, vertical
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
            
        case 2:
            madctl |= 0b00000000;  // None
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
            
        case 3:
            madctl |= 0b00010001;  // Scan bottom-up, vertical
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
    }
    
    // Send the configuration to the display
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);
    SSD1331_WriteCommand(ssd, madctl);
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
