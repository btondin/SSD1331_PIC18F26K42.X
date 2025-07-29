/**
 * @file ssd1331.c
 * @brief Driver implementation for SSD1331 96x64 RGB OLED display for PIC18F26K42
 *
 * This implementation provides hardware-specific functions for the SSD1331 OLED controller,
 * including SPI communication, display initialization, pixel drawing, and graphics operations.
 * Based on Adafruit's SSD1331 library adapted for PIC microcontrollers.
 *
 * @author @btondin 
 * @date 2025
 */

#include "ssd1331.h"
#include "gfx_pic.h"

//==============================================================================
// HELPER MACROS AND VARIABLES
//==============================================================================

/** @brief Macro to swap two 16-bit values */
#define ssd1331_swap(a, b) { int16_t t = a; a = b; b = t; }

/** @brief Dummy variable for SPI read operations */
uint8_t SPI_dummy;

//==============================================================================
// PRIVATE FUNCTION PROTOTYPES
//==============================================================================

static void SSD1331_HardwareReset(SSD1331_t *ssd);
static void SSD1331_Select(SSD1331_t *ssd);
static void SSD1331_Deselect(SSD1331_t *ssd);
static void SSD1331_SetDataMode(SSD1331_t *ssd);
static void SSD1331_SetCommandMode(SSD1331_t *ssd);

//==============================================================================
// UTILITY FUNCTIONS
//==============================================================================

/**
 * @brief Software delay function in milliseconds
 * @param ms Delay time in milliseconds
 */
void SSD1331_Delay(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}

//==============================================================================
// INITIALIZATION FUNCTIONS
//==============================================================================

/**
 * @brief Initialize SSD1331 driver structure and GPIO pins
 * 
 * Sets up the graphics context, function pointers, and GPIO pin states.
 * The GFX library will handle most drawing operations with fallback implementations.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_Init(SSD1331_t *ssd) {
    // Initialize rotation
    ssd->rotation = 0;

    // Initialize graphics context with display dimensions
    GFX_Init(&ssd->gfx, SSD1331_WIDTH, SSD1331_HEIGHT);

    // Assign only functions that SSD1331 driver implements directly
    // This prevents recursion by letting GFX library handle complex operations
    ssd->gfx.drawPixel = (void*)SSD1331_DrawPixel;
    ssd->gfx.writePixel = (void*)SSD1331_WriteData16;
    
    // Leave other functions as NULL to use GFX library fallbacks
    // This avoids recursion issues:
    // ssd->gfx.fillRect      = NULL; 
    // ssd->gfx.drawFastVLine = NULL;
    // ssd->gfx.drawFastHLine = NULL;
    // ssd->gfx.fillScreen    = NULL;

    // Initialize GPIO pins to default states
    SSD1331_CS_SetHigh();   // Chip select inactive (high)
    SSD1331_DC_SetHigh();   // Data mode (high)
    SSD1331_RST_SetHigh();  // Reset inactive (high)
}

/**
 * @brief Initialize SSD1331 hardware and configure display registers
 * 
 * Performs hardware reset, sends initialization commands, and sets up
 * the display for operation. Uses initialization sequence from Adafruit library.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 */
void SSD1331_Begin(SSD1331_t *ssd) {
    // Initialize SPI peripheral
    SPI1_Open(SPI1_DEFAULT);
    
    // Perform hardware reset sequence
    SSD1331_HardwareReset(ssd);

    // Initialization sequence from Adafruit Library
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFF);      // Turn off display during init
    
    // Set display re-mapping and color depth
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);        
    SSD1331_WriteCommand(ssd, 0x72);                        // Default remap settings
    
    // Set display start line
    SSD1331_WriteCommand(ssd, SSD1331_CMD_STARTLINE);       
    SSD1331_WriteCommand(ssd, 0x00);                        // Start from line 0
    
    // Set display offset
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYOFFSET);   
    SSD1331_WriteCommand(ssd, 0x00);                        // No offset
    
    // Set normal display mode
    SSD1331_WriteCommand(ssd, SSD1331_CMD_NORMALDISPLAY);   
    
    // Set multiplex ratio (1/64 duty cycle)
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMULTIPLEX);    
    SSD1331_WriteCommand(ssd, 0x3F);                        
    
    // Set master configuration
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETMASTER);       
    SSD1331_WriteCommand(ssd, 0x8E);
    
    // Set power mode
    SSD1331_WriteCommand(ssd, SSD1331_CMD_POWERMODE);       
    SSD1331_WriteCommand(ssd, 0x0B);
    
    // Set pre-charge period
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGE);       
    SSD1331_WriteCommand(ssd, 0x31);
    
    // Set display clock divide ratio
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CLOCKDIV);        
    SSD1331_WriteCommand(ssd, 0xF0);
    
    // Set second pre-charge speeds for each color
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEA);      // Red
    SSD1331_WriteCommand(ssd, 0x64);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEB);      // Green
    SSD1331_WriteCommand(ssd, 0x78);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGEC);      // Blue
    SSD1331_WriteCommand(ssd, 0x64);
    
    // Set pre-charge voltage level
    SSD1331_WriteCommand(ssd, SSD1331_CMD_PRECHARGELEVEL);  
    SSD1331_WriteCommand(ssd, 0x3A);
    
    // Set VCOMH voltage
    SSD1331_WriteCommand(ssd, SSD1331_CMD_VCOMH);           
    SSD1331_WriteCommand(ssd, 0x3E);
    
    // Set master current control
    SSD1331_WriteCommand(ssd, SSD1331_CMD_MASTERCURRENT);   
    SSD1331_WriteCommand(ssd, 0x06);
    
    // Set individual RGB contrast levels
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTA);       // Red contrast
    SSD1331_WriteCommand(ssd, 0x91);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTB);       // Green contrast
    SSD1331_WriteCommand(ssd, 0x50);
    SSD1331_WriteCommand(ssd, SSD1331_CMD_CONTRASTC);       // Blue contrast
    SSD1331_WriteCommand(ssd, 0x7D);
    
    // Turn on display
    SSD1331_WriteCommand(ssd, SSD1331_CMD_DISPLAYON);       
    
    // Set initial rotation
    SSD1331_SetRotation(ssd, SSD1331_INIT_ROTATION);
}

//==============================================================================
// DISPLAY CONFIGURATION FUNCTIONS
//==============================================================================

/**
 * @brief Set display rotation and update dimensions
 * 
 * Configures the display orientation by setting the appropriate re-map register.
 * Updates the graphics context dimensions based on rotation.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param r Rotation index (0=0°, 1=90°, 2=180°, 3=270°)
 */
void SSD1331_SetRotation(SSD1331_t *ssd, uint8_t r) {
    // Set base re-map configuration based on color order
    #ifdef SSD1331_COLORORDER_RGB
        uint8_t madctl = 0b01100000; // 64K color, enable split, RGB order
    #else
        uint8_t madctl = 0b01100100; // 64K color, enable split, BGR order
    #endif
    
    // Limit rotation to valid range (0-3)
    ssd->rotation = r & 3;
    ssd->gfx.rotation = ssd->rotation;
    
    // Configure re-map bits and dimensions based on rotation
    switch (ssd->rotation) {
        case 0: // 0° rotation (normal)
            madctl |= 0b00010010;  // Scan bottom-up, column remap 95-0
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
            
        case 1: // 90° rotation (portrait)
            madctl |= 0b00000011;  // Column remap 95-0, vertical increment
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
            
        case 2: // 180° rotation (inverted)
            madctl |= 0b00000000;  // No additional flags
            ssd->gfx.width = SSD1331_WIDTH;
            ssd->gfx.height = SSD1331_HEIGHT;
            break;
            
        case 3: // 270° rotation (landscape)
            madctl |= 0b00010001;  // Scan bottom-up, vertical increment
            ssd->gfx.width = SSD1331_HEIGHT;
            ssd->gfx.height = SSD1331_WIDTH;
            break;
    }
    
    // Send re-map command to display
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETREMAP);
    SSD1331_WriteCommand(ssd, madctl);
}

//==============================================================================
// ADDRESS WINDOW AND DRAWING FUNCTIONS
//==============================================================================

/**
 * @brief Set address window for pixel data writing
 * 
 * Defines the rectangular area where subsequent pixel data will be written.
 * Automatically clips coordinates to display boundaries.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x Starting X coordinate
 * @param y Starting Y coordinate
 * @param w Window width in pixels
 * @param h Window height in pixels
 */
void SSD1331_SetAddrWindow(SSD1331_t *ssd, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // Calculate end coordinates
    int16_t x1 = x;
    int16_t y1 = y;
    int16_t x2 = x + w - 1;
    int16_t y2 = y + h - 1;

    // Clip coordinates to display boundaries
    if (x1 > ssd->gfx.width - 1) x1 = ssd->gfx.width - 1;
    if (y1 > ssd->gfx.height - 1) y1 = ssd->gfx.height - 1;
    if (x2 > ssd->gfx.width - 1) x2 = ssd->gfx.width - 1;
    if (y2 > ssd->gfx.height - 1) y2 = ssd->gfx.height - 1;

    // Set column address range
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETCOLUMN);
    SSD1331_WriteCommand(ssd, (uint8_t)x1);
    SSD1331_WriteCommand(ssd, (uint8_t)x2);

    // Set row address range
    SSD1331_WriteCommand(ssd, SSD1331_CMD_SETROW);
    SSD1331_WriteCommand(ssd, (uint8_t)y1);
    SSD1331_WriteCommand(ssd, (uint8_t)y2);
}

/**
 * @brief Draw a single pixel at specified coordinates
 * 
 * Sets address window to single pixel and writes color data.
 * Automatically clips coordinates to display boundaries.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Pixel color in RGB565 format
 */
void SSD1331_DrawPixel(SSD1331_t *ssd, int16_t x, int16_t y, uint16_t color) {
    // Check bounds
    if ((x < 0) || (x >= ssd->gfx.width) || (y < 0) || (y >= ssd->gfx.height)) {
        return;
    }
    
    // Set address window for single pixel and write color
    SSD1331_SetAddrWindow(ssd, (uint16_t)x, (uint16_t)y, 1, 1);
    SSD1331_WriteData16(ssd, color);
}

/**
 * @brief Fill entire screen with specified color
 * 
 * Uses GFX library's fillScreen implementation which will call
 * the assigned drawPixel function for each pixel.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param color Fill color in RGB565 format
 */
void SSD1331_FillScreen(SSD1331_t *ssd, uint16_t color) {
    //GFX_FillScreen(&ssd->gfx, ssd, color);
    SSD1331_FillRect_Fast(ssd, 0, 0, 96, 64, color);
}

/**
 * @brief Fill rectangular area with specified color
 * 
 * Uses GFX library's fillRect implementation which will call
 * the assigned drawPixel function for each pixel in the rectangle.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width in pixels
 * @param h Rectangle height in pixels
 * @param color Fill color in RGB565 format
 */
void SSD1331_FillRect(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    GFX_FillRect(&ssd->gfx, ssd, x, y, w, h, color);    
}

/**
 * @brief Fill rectangular area with specified color, but faster
 *  
 * the assigned drawPixel function for each pixel in the rectangle.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param w Rectangle width in pixels
 * @param h Rectangle height in pixels
 * @param color Fill color in RGB565 format
 */
void SSD1331_FillRect_Fast(SSD1331_t *ssd, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    
    // Set address window for the bitmap area
    SSD1331_SetAddrWindow(ssd, (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h);
    
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    
    // Calculate total number of pixels to transfer
    uint32_t total_pixels = (uint32_t)w * h;
    
    // Send each pixel as two bytes (high byte first, then low byte)
    for (uint32_t p = 0; p < total_pixels; p++) {
        SPI1_ExchangeByte(color >> 8);    // Send high byte (bits 15-8)
        SPI1_ExchangeByte(color & 0xFF);  // Send low byte (bits 7-0)
    }    
    
    SSD1331_Deselect(ssd);   
}



/**
 * @brief Draw RGB565 bitmap from 16-bit data array
 * 
 * This function draws a bitmap using 16-bit RGB565 color data. Each pixel
 * is represented as a single uint16_t value in RGB565 format (5 bits red,
 * 6 bits green, 5 bits blue). This is the preferred format for SSD1331
 * displays as it matches the native color format.
 * 
 * The function uses direct SPI byte exchanges for optimal performance,
 * sending the high byte followed by the low byte for each pixel.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x Starting X coordinate for bitmap placement (top-left corner)
 * @param y Starting Y coordinate for bitmap placement (top-left corner)
 * @param bitmap Pointer to RGB565 bitmap data array (uint16_t per pixel)
 * @param w Bitmap width in pixels
 * @param h Bitmap height in pixels
 * 
 * @note The bitmap data should be organized row by row, left to right
 * @note Total array size should be w * h uint16_t elements
 * @note Function validates input parameters and returns early if invalid
 */
void SSD1331_DrawFastRGBBitmap16(SSD1331_t *ssd, int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
    // Validate input parameters
    if (bitmap == NULL || w <= 0 || h <= 0) {
        return;
    }
    
    // Set address window for the bitmap area
    SSD1331_SetAddrWindow(ssd, (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h);
    
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    
    // Calculate total number of pixels to transfer
    uint32_t total_pixels = (uint32_t)w * h;
    
    // Send each pixel as two bytes (high byte first, then low byte)
    for (uint32_t p = 0; p < total_pixels; p++) {
        SPI1_ExchangeByte(bitmap[p] >> 8);    // Send high byte (bits 15-8)
        SPI1_ExchangeByte(bitmap[p] & 0xFF);  // Send low byte (bits 7-0)
    }    
    
    SSD1331_Deselect(ssd);
}

/**
 * @brief Draw RGB bitmap from 8-bit data array (raw byte format)
 * 
 * This function draws a bitmap using raw 8-bit byte data where each pixel
 * is represented as two consecutive bytes in the array (high byte, low byte).
 * This format is useful when working with pre-converted image data or when
 * interfacing with systems that provide RGB565 data in byte arrays.
 * 
 * The function uses SPI block transfer for maximum performance, sending
 * the entire bitmap data in a single operation. The input data should already
 * be in the correct RGB565 byte order (high byte, low byte per pixel).
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param x Starting X coordinate for bitmap placement (top-left corner)
 * @param y Starting Y coordinate for bitmap placement (top-left corner)
 * @param bitmap Pointer to raw RGB565 byte data (2 bytes per pixel)
 * @param w Bitmap width in pixels
 * @param h Bitmap height in pixels
 * 
 * @note The bitmap array should contain w * h * 2 bytes total
 * @note Data format: [pixel0_high, pixel0_low, pixel1_high, pixel1_low, ...]
 * @note This function assumes the byte data is already in correct RGB565 format
 * @note Uses SPI block transfer for optimal performance
 */
void SSD1331_DrawFastRGBBitmap8(SSD1331_t *ssd, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
    // Validate input parameters
    if (bitmap == NULL || w <= 0 || h <= 0) {
        return;
    }
    
    // Set address window for the bitmap area
    SSD1331_SetAddrWindow(ssd, (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h);
    
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    
    // Calculate total number of pixels and bytes to transfer
    uint32_t total_pixels = (uint32_t)w * h;
    uint32_t total_bytes = total_pixels * 2;  // 2 bytes per pixel in RGB565
    
    // Send entire bitmap data in one block transfer for maximum performance
    // The bitmap data should already be in correct byte order (high, low per pixel)
    SPI1_ExchangeBlock(bitmap, total_bytes);
    
    SSD1331_Deselect(ssd);
}

//==============================================================================
// SPI COMMUNICATION FUNCTIONS
//==============================================================================

/**
 * @brief Send command byte to SSD1331 via SPI
 * 
 * Sets command mode (DC low), selects chip, sends byte, then deselects.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param cmd Command byte to send
 */
void SSD1331_WriteCommand(SSD1331_t *ssd, uint8_t cmd) {
    SSD1331_Select(ssd);
    SSD1331_SetCommandMode(ssd);
    SPI_dummy = SPI1_ExchangeByte(cmd);
    SSD1331_Deselect(ssd);
}

/**
 * @brief Send 16-bit data word to SSD1331 via SPI
 * 
 * Sets data mode (DC high), selects chip, sends high byte then low byte,
 * then deselects. Used for RGB565 pixel data.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 * @param data 16-bit data word to send (RGB565 pixel data)
 */
void SSD1331_WriteData16(SSD1331_t *ssd, uint16_t data) {
    SSD1331_Select(ssd);
    SSD1331_SetDataMode(ssd);
    SPI1_ExchangeByte(data >> 8);    // Send high byte first
    SPI1_ExchangeByte(data & 0xFF);  // Send low byte
    SSD1331_Deselect(ssd);
}

//==============================================================================
// PRIVATE HELPER FUNCTIONS
//==============================================================================

/**
 * @brief Perform hardware reset sequence on SSD1331
 * 
 * Toggles reset pin with appropriate delays to reset the display controller.
 * 
 * @param ssd Pointer to SSD1331 driver structure
 */
static void SSD1331_HardwareReset(SSD1331_t *ssd) {
    SSD1331_RST_SetHigh();  // Ensure reset is inactive
    SSD1331_Delay(10);      // Wait for stability
    SSD1331_RST_SetLow();   // Assert reset
    SSD1331_Delay(10);      // Hold reset active
    SSD1331_RST_SetHigh();  // Release reset
    SSD1331_Delay(10);      // Wait for controller to initialize
}

/**
 * @brief Assert chip select (select SSD1331 for communication)
 * @param ssd Pointer to SSD1331 driver structure
 */
static void SSD1331_Select(SSD1331_t *ssd) { 
    SSD1331_CS_SetLow(); 
}

/**
 * @brief Deassert chip select (end communication with SSD1331)
 * @param ssd Pointer to SSD1331 driver structure
 */
static void SSD1331_Deselect(SSD1331_t *ssd) { 
    SSD1331_CS_SetHigh(); 
}

/**
 * @brief Set data mode (DC high for data transmission)
 * @param ssd Pointer to SSD1331 driver structure
 */
static void SSD1331_SetDataMode(SSD1331_t *ssd) { 
    SSD1331_DC_SetHigh(); 
}

/**
 * @brief Set command mode (DC low for command transmission)
 * @param ssd Pointer to SSD1331 driver structure
 */
static void SSD1331_SetCommandMode(SSD1331_t *ssd) { 
    SSD1331_DC_SetLow(); 
}