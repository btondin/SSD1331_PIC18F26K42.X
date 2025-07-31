/**
 * @file main.c
 * @brief Main application for SSD1331 OLED display test using PIC18F26K42
 *
 * This application initializes and tests the SSD1331 96x64 RGB OLED display.
 * It is a direct port of the Adafruit SSD1331 library example sketch to ensure
 * identical graphical output. The application runs a continuous loop of various
 * graphics tests including lines, rectangles, circles, text rendering, and images.
 *
 * @author @btondin, with conversion logic by Gemini
 * @date 2025
 */

#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ssd1331.h"
#include "screens.h"

//==============================================================================
// GLOBAL VARIABLES
//==============================================================================

/** @brief Global instance of the OLED display driver */
SSD1331_t oled;

/** @brief Buffer for formatted text output */
char buffer[20];

//==============================================================================
// TIMING CONSTANTS
//==============================================================================

/** @brief Short delay duration in milliseconds */
#define DELAY_SHORT 500

/** @brief Medium delay duration in milliseconds */
#define DELAY_MEDIUM 1000

/** @brief Long delay duration in milliseconds */
#define DELAY_LONG 2000

//==============================================================================
// FUNCTION PROTOTYPES
//==============================================================================

// Line drawing tests
void testlines(uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);

// Rectangle drawing tests
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testroundrects(void);

// Circle drawing tests
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);

// Triangle drawing tests
void testtriangles(void);

// Text and display tests
void tftPrintTest(void);
void lcdTestPattern(void);

// Image display tests
void testimages(void);

//==============================================================================
// MAIN FUNCTION
//==============================================================================

/**
 * @brief Main entry point of the application
 * 
 * Initializes the system, SPI peripheral, and OLED display, then runs
 * a continuous loop of graphics tests to demonstrate all library features.
 */
void main(void) {
    // Initialize device and system peripherals
    SYSTEM_Initialize();

    // Open SPI1 with default settings
    SPI1_Open(SPI1_DEFAULT);

    // Initialize and start the OLED display
    SSD1331_Init(&oled);
    SSD1331_Begin(&oled);
    
    // Main application loop - equivalent to Arduino loop() function
    while(1) {
        // Clear screen to start fresh
        SSD1331_FillScreen(&oled, SSD1331_BLACK);
        
        //while(1);
        
        
        __delay_ms(DELAY_SHORT);

        // Display color test pattern
        lcdTestPattern();
        __delay_ms(DELAY_MEDIUM);

        // Text wrapping demonstration
        SSD1331_FillScreen(&oled, SSD1331_BLACK);
        GFX_SetCursor(&oled.gfx, 0, 0);
        GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
        GFX_SetTextWrap(&oled.gfx, true);
        GFX_Print(&oled.gfx, &oled, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
        __delay_ms(DELAY_MEDIUM);

        // Comprehensive text formatting test
        tftPrintTest();
        __delay_ms(DELAY_LONG);
        
        // Single pixel test - draw center pixel
        SSD1331_FillScreen(&oled, SSD1331_BLACK);
        GFX_DrawPixel(&oled.gfx, &oled, oled.gfx.width / 2, oled.gfx.height / 2, SSD1331_GREEN);
        __delay_ms(DELAY_SHORT);

        // Line drawing tests
        testlines(SSD1331_YELLOW);
        __delay_ms(DELAY_SHORT);

        // Optimized fast line tests
        testfastlines(SSD1331_RED, SSD1331_BLUE);
        __delay_ms(DELAY_SHORT);

        // Rectangle drawing tests
        testdrawrects(SSD1331_GREEN);
        __delay_ms(DELAY_MEDIUM);

        // Filled rectangle tests
        testfillrects(SSD1331_YELLOW, SSD1331_MAGENTA);
        __delay_ms(DELAY_MEDIUM);

        // Circle tests
        SSD1331_FillScreen(&oled, SSD1331_BLACK);
        testfillcircles(10, SSD1331_BLUE);
        testdrawcircles(10, SSD1331_WHITE);
        __delay_ms(DELAY_MEDIUM);

        // Rounded rectangle tests
        testroundrects();
        __delay_ms(DELAY_SHORT);
        
        // Triangle tests
        testtriangles();
        __delay_ms(DELAY_SHORT);
         
         
        
        // Image display tests (remove if memory is limited)
        testimages();           
        
        // Toggle LED to indicate loop completion
        LED0_Toggle();
    }
}

//==============================================================================
// LINE DRAWING TESTS
//==============================================================================

/**
 * @brief Test line drawing from corners to all edges
 * 
 * Draws lines from each corner of the display to all points along
 * the opposite edges, creating a fan pattern from each corner.
 * 
 * @param color Color to use for drawing lines
 */
void testlines(uint16_t color) {
    // Lines from top-left corner
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, 0, x, oled.gfx.height - 1, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, 0, oled.gfx.width - 1, y, color);
    }

    // Lines from top-right corner
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, 0, x, oled.gfx.height - 1, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, 0, 0, y, color);
    }

    // Lines from bottom-left corner
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, oled.gfx.height - 1, x, 0, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, oled.gfx.height - 1, oled.gfx.width - 1, y, color);
    }

    // Lines from bottom-right corner
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, oled.gfx.height - 1, x, 0, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, oled.gfx.height - 1, 0, y, color);
    }
}

/**
 * @brief Test optimized fast line drawing functions
 * 
 * Draws a grid pattern using the optimized horizontal and vertical
 * line drawing functions to test their performance and accuracy.
 * 
 * @param color1 Color for horizontal lines
 * @param color2 Color for vertical lines
 */
void testfastlines(uint16_t color1, uint16_t color2) {
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    
    // Draw horizontal lines
    for (int16_t y = 0; y < oled.gfx.height; y += 5) {
        GFX_DrawFastHLine(&oled.gfx, &oled, 0, y, oled.gfx.width, color1);
    }
    
    // Draw vertical lines
    for (int16_t x = 0; x < oled.gfx.width; x += 5) {
        GFX_DrawFastVLine(&oled.gfx, &oled, x, 0, oled.gfx.height, color2);
    }
}

//==============================================================================
// RECTANGLE DRAWING TESTS
//==============================================================================

/**
 * @brief Test rectangle outline drawing
 * 
 * Draws concentric rectangle outlines from the center outward,
 * creating a nested rectangle pattern.
 * 
 * @param color Color to use for rectangle outlines
 */
void testdrawrects(uint16_t color) {
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    
    for (int16_t x = 0; x < oled.gfx.height - 1; x += 6) {
        GFX_DrawRect(&oled.gfx, &oled, 
                     (oled.gfx.width - 1) / 2 - x / 2, 
                     (oled.gfx.height - 1) / 2 - x / 2, 
                     x, x, color);
    }
}

/**
 * @brief Test filled rectangle drawing
 * 
 * Draws concentric filled rectangles with alternating colors,
 * creating a layered effect from outside to inside.
 * 
 * @param color1 Primary fill color
 * @param color2 Secondary outline color
 */
void testfillrects(uint16_t color1, uint16_t color2) {
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    
    for (int16_t x = oled.gfx.height - 1; x > 6; x -= 6) {
        //GFX_FillRect(&oled.gfx, &oled, 
        SSD1331_FillRect_Fast(&oled,
                     (oled.gfx.width - 1) / 2 - x / 2, 
                     (oled.gfx.height - 1) / 2 - x / 2, 
                     x, x, color1);
        GFX_DrawRect(&oled.gfx, &oled, 
                     (oled.gfx.width - 1) / 2 - x / 2, 
                     (oled.gfx.height - 1) / 2 - x / 2, 
                     x, x, color2);
    }
}

/**
 * @brief Test rounded rectangle drawing
 * 
 * Draws multiple sets of nested rounded rectangles with varying
 * colors and positions to test the rounded corner functionality.
 */
void testroundrects(void) {
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    uint16_t color = 100;
    int i, t;
    
    for (t = 0; t <= 4; t += 1) {
        int x = 0;
        int y = 0;
        int w = oled.gfx.width;
        int h = oled.gfx.height;
        
        for (i = 0; i <= 8; i += 1) {
            GFX_DrawRoundRect(&oled.gfx, &oled, x, y, w, h, 5, color);
            x += 2;
            y += 3;
            w -= 4;
            h -= 6;
            color += 1100;
        }
        color += 100;
    }
}

//==============================================================================
// CIRCLE DRAWING TESTS
//==============================================================================

/**
 * @brief Test filled circle drawing
 * 
 * Draws a grid of filled circles across the display to test
 * the circle filling algorithm and spacing.
 * 
 * @param radius Radius of circles to draw
 * @param color Color to fill circles with
 */
void testfillcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = radius; x < oled.gfx.width; x += radius * 2) {
        for (int16_t y = radius; y < oled.gfx.height; y += radius * 2) {
            GFX_FillCircle(&oled.gfx, &oled, x, y, radius, color);
        }
    }
}

/**
 * @brief Test circle outline drawing
 * 
 * Draws a grid of circle outlines, overlapping the filled circles
 * to create a combined pattern that tests both circle functions.
 * 
 * @param radius Radius of circles to draw
 * @param color Color for circle outlines
 */
void testdrawcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = 0; x < oled.gfx.width + radius; x += radius * 2) {
        for (int16_t y = 0; y < oled.gfx.height + radius; y += radius * 2) {
            GFX_DrawCircle(&oled.gfx, &oled, x, y, radius, color);
        }
    }
}

//==============================================================================
// TRIANGLE DRAWING TESTS
//==============================================================================

/**
 * @brief Test triangle drawing
 * 
 * Draws a series of triangles with progressively changing dimensions
 * and colors to test the triangle drawing algorithm.
 */
void testtriangles(void) {
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    uint16_t color = 0xF800; // Start with red
    int t;
    int w = oled.gfx.width / 2;
    int x = oled.gfx.height;
    int y = 0;
    int z = oled.gfx.width;
    
    for (t = 0; t <= 15; t++) {
        GFX_DrawTriangle(&oled.gfx, &oled, w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

//==============================================================================
// TEXT AND DISPLAY TESTS
//==============================================================================

/**
 * @brief Test text printing and formatting functions
 * 
 * Demonstrates various text sizes, colors, background colors,
 * and number formatting capabilities of the text rendering system.
 */
void tftPrintTest(void) {
    // Test 1: Basic text with different sizes and colors
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    GFX_SetCursor(&oled.gfx, 0, 5);
    GFX_SetTextColor(&oled.gfx, SSD1331_RED);
    GFX_SetTextSize(&oled.gfx, 1);
    GFX_Print(&oled.gfx, &oled, "Hello World!\n");

    GFX_SetTextColorBg(&oled.gfx, SSD1331_YELLOW, SSD1331_GREEN);
    GFX_SetTextSize(&oled.gfx, 2);
    GFX_Print(&oled.gfx, &oled, "Hello Wo");
    
    GFX_SetTextColor(&oled.gfx, SSD1331_BLUE);
    GFX_SetTextSize(&oled.gfx, 3);
    sprintf(buffer, "%3.2f", 123.45);
    GFX_Print(&oled.gfx, &oled, buffer);
    
    __delay_ms(DELAY_LONG);

    // Test 2: More text formatting options
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    GFX_SetCursor(&oled.gfx, 0, 5);
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_SetTextSize(&oled.gfx, 1);
    GFX_Print(&oled.gfx, &oled, "Hello World!\n");
    
    // Float number formatting
    float pi = 3.14159;
    sprintf(buffer, "%.5f Want pi?\n", pi);
    GFX_SetTextSize(&oled.gfx, 1);
    GFX_SetTextColor(&oled.gfx, SSD1331_GREEN);
    GFX_Print(&oled.gfx, &oled, buffer);
    
    // Hexadecimal number formatting
    sprintf(buffer, "%lX", 8675309L);
    GFX_Print(&oled.gfx, &oled, buffer);
    GFX_Print(&oled.gfx, &oled, " Print HEX\n");
    
    // Runtime information display
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_Print(&oled.gfx, &oled, "Sketch has been\n");
    GFX_Print(&oled.gfx, &oled, "running for: \n");
    
    GFX_SetTextColor(&oled.gfx, SSD1331_MAGENTA);
    uint8_t sim_millis = 6; // Simulated runtime
    sprintf(buffer, "%d ", sim_millis);
    GFX_Print(&oled.gfx, &oled, buffer); 
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_Print(&oled.gfx, &oled, "seconds.");
}

/**
 * @brief Display LCD test pattern
 * 
 * Creates a color bar test pattern commonly used for display testing.
 * Draws vertical color bands across the screen to verify color accuracy
 * and display uniformity.
 */
void lcdTestPattern(void) {
    static const uint16_t colors[] = {
        SSD1331_BLACK, SSD1331_YELLOW, SSD1331_MAGENTA, SSD1331_RED,
        SSD1331_CYAN, SSD1331_GREEN, SSD1331_BLUE, SSD1331_WHITE
    };

    for (uint8_t h = 0; h < oled.gfx.height; h++) {
        for (uint8_t w = 0; w < oled.gfx.width; w++) {
            // Calculate color band based on horizontal position
            int band = w / 12;
            if (band > 7) band = 7;
            GFX_DrawPixel(&oled.gfx, &oled, w, h, colors[band]);
        }
    }
}

//==============================================================================
// IMAGE DISPLAY TESTS
//==============================================================================

/**
 * @brief Test bitmap image display
 * 
 * Displays pre-stored bitmap images to test the RGB bitmap
 * rendering functionality. Images are stored in the screens.h file.
 * 
 * @note Remove this function if memory resources are limited
 */
void testimages(void) {
    // Display first test image (16 bit array)       
    SSD1331_DrawFastRGBBitmap16(&oled, 0, 0, bunmi_img, SSD1331_WIDTH, SSD1331_HEIGHT);
    __delay_ms(DELAY_LONG); 
    
    
    // Display second test image (8 bit array)           
    SSD1331_DrawFastRGBBitmap8(&oled, 0, 0, lena8b, SSD1331_WIDTH, SSD1331_HEIGHT);
    __delay_ms(DELAY_LONG);
}