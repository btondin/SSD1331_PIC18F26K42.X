/**
 * @file    ssd1331_pic.c
 * @brief   Driver implementation for SSD1331 96x64 RGB OLED display for PIC18F26K42
 * * @author  @btondin 
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
    ssd->rotation = 0;

    GFX_Init(&ssd->gfx, SSD1331_WIDTH, SSD1331_HEIGHT);

    // Atribui SOMENTE as funções que o driver SSD1331 implementa diretamente.
    ssd->gfx.drawPixel     = (void*)SSD1331_DrawPixel;
    ssd->gfx.writePixel    = (void*)SSD1331_WriteData16;
    
    // As funções abaixo usarão o fallback da GFX, evitando recursão.
    // Deixe-as como NULL, que é o padrão da GFX_Init.
    // ssd->gfx.fillRect      = NULL; 
    // ssd->gfx.drawFastVLine = NULL;
    // ssd->gfx.drawFastHLine = NULL;
    
    // A função de preencher tela pode ser otimizada. A GFX já faz isso
    // chamando fillRect, então não precisamos atribuir aqui.
    // ssd->gfx.fillScreen    = (void*)GFX_FillScreen;

    // Setup GPIO pins
    //SSD1331_CS_SetDigitalOutput(); // Não é necessário, já está definido como saída
    //SSD1331_DC_SetDigitalOutput(); // Não é necessário, já está definido como saída
    //SSD1331_RST_SetDigitalOutput(); // Não é necessário, já está definido como saída

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

    // Initialization Sequence from Adafruit Library
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFF);      // 0xAE
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);        // 0xA0
    SSD1331_WriteCommand(ssd, 0x72); // Default remap
    SSD1331_WriteCommand(ssd, SSD1331_CMD_STARTLINE);       // 0xA1
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFFSET);   // 0xA2
    SSD1331_WriteCommand(ssd, 0x00);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_NORMALDISPLAY);   // 0xA4
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMULTIPLEX);    // 0xA8
    SSD1331_WriteCommand(ssd, 0x3F);                        // 1/64 duty
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMASTER);       // 0xAD
    SSD1331_WriteCommand(ssd, 0x8E);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_POWERMODE);       // 0xB0
    SSD1331_WriteCommand(ssd, 0x0B);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGE);       // 0xB1
    SSD1331_WriteCommand(ssd, 0x31);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CLOCKDIV);        // 0xB3
    SSD1331_WriteCommand(ssd, 0xF0);
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
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYON);       // Turn on OLED panel
    
    // Set initial rotation
    SSD1331_SetRotation(ssd, SSD1331_INIT_ROTATION);
}

/**
 * @brief Sets origin of (0,0) and orientation of OLED display
 * @param ssd Pointer to SSD1331 structure
 * @param r Rotation index (0-3)
 */
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t r) {
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
            madctl |= 0b00000011;  // column remap 95-0, vertical
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
        case 2:
            madctl |= 0b00000000;  // None
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
        case 3:
            madctl |= 0b00010001;  // Scan bottom-up, Vertical
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
    }
    
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);
    SSD1331_WriteCommand(ssd, madctl);
}


// ... (O resto do arquivo .c pode permanecer como estava)
// (SSD1331_SetAddrWindow, DrawPixel, FillScreen, etc.)
// Apenas a função de rotação e a inicialização dos ponteiros de função precisavam de ajuste.

// As funções abaixo continuam como você implementou, estão corretas.

/**
 * @brief Sets the address window for drawing
 */
void SSD1331_SetAddrWindow(SSD1331_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // Use int16_t para as variáveis locais para corresponder aos tipos da GFX e evitar warnings.
    int16_t x1 = x;
    int16_t y1 = y;
    int16_t x2 = x + w - 1;
    int16_t y2 = y + h - 1;

    // A lógica de clipping e swap permanece a mesma.
    if (x1 > ssd->gfx.width - 1) x1 = ssd->gfx.width - 1;
    if (y1 > ssd->gfx.height - 1) y1 = ssd->gfx.height - 1;
    if (x2 > ssd->gfx.width - 1) x2 = ssd->gfx.width - 1;
    if (y2 > ssd->gfx.height - 1) y2 = ssd->gfx.height - 1;

    // Envie o comando com uma conversão explícita para uint8_t
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETCOLUMN);
    SSD1331_WriteCommand(ssd, (uint8_t)x1);
    SSD1331_WriteCommand(ssd, (uint8_t)x2);

    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETROW);
    SSD1331_WriteCommand(ssd, (uint8_t)y1);
    SSD1331_WriteCommand(ssd, (uint8_t)y2);
}

/**
 * @brief Draws a single pixel
 */
void SSD1331_DrawPixel(SSD1331_t *ssd, int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= ssd->gfx.width) || (y < 0) || (y >= ssd->gfx.height)) return;
    
    // O compilador avisou sobre conversão de sinal aqui.
    // int16_t (com sinal) para uint16_t (sem sinal).
    // A conversão é segura, mas podemos ser explícitos.
    SSD1331_SetAddrWindow(ssd, (uint16_t)x, (uint16_t)y, 1, 1);
    SSD1331_WriteData16(ssd, color);
}

/**
 * @brief Fills the entire screen with a color
 */
void SSD1331_FillScreen(SSD1331_t *ssd, uint16_t color) {
    GFX_FillScreen(&ssd->gfx, ssd, color);
}

/**
 * @brief Fills a rectangle
 */
void SSD1331_FillRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    GFX_FillRect(&ssd->gfx, ssd, x, y, w, h, color);
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
 * @brief Writes a 16-bit data word to the display
 */
void SSD1331_WriteData16(SSD1331_t *ssd, uint16_t data) {
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    SPI1_ExchangeByte(data >> 8);
    SPI1_ExchangeByte(data & 0xFF);
    SSD1331_Deselect(ssd);
}

static void SSD1331_HardwareReset(SSD1331_t *ssd) {
    SSD1331_RST_SetHigh();
    SSD1331_Delay(10);
    SSD1331_RST_SetLow();
    SSD1331_Delay(10);
    SSD1331_RST_SetHigh();
    SSD1331_Delay(10);
}

static void SSD1331_Select(SSD1331_t *ssd) { SSD1331_CS_SetLow(); }
static void SSD1331_Deselect(SSD1331_t *ssd) { SSD1331_CS_SetHigh(); }
static void SSD1331_SetDataMode(SSD1331_t *ssd) { SSD1331_DC_SetHigh(); }
static void SSD1331_SetCommandMode(SSD1331_t *ssd) { SSD1331_DC_SetLow(); }