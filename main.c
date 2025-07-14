/**
 * @file main.c
 * @brief Main application for SSD1331 OLED display test using PIC18F26K42
 * 
 * @details
 * This application initializes and tests the SSD1331 96x64 RGB OLED display.
 * It demonstrates various graphics operations such as color fill, shape drawing,
 * text rendering, and simple animations using the GFX and SSD1331 libraries.
 */

#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include "ssd1331.h"
#include "screens.h"

// Global instance of the OLED display driver
SSD1331_t oled;

/**
 * @brief Main entry point of the application.
 * 
 * Initializes system and SPI, sets up the SSD1331 OLED display, and runs
 * a series of graphical tests, including colors, shapes, text, pixels,
 * and bitmap display. Finally, runs a simple counter animation.
 */
void main(void) {
    // Initialize device and system peripherals
    SYSTEM_Initialize();

    // Short delay to stabilize power supply
    __delay_ms(100);

    // Open SPI1 with default settings
    SPI1_Open(SPI1_DEFAULT);

    // Initialize and start the OLED display
    SSD1331_Init(&oled);
    SSD1331_Begin(&oled);

    // ==========================
    // TEST 1: BASIC COLORS
    // ==========================
    SSD1331_FillScreen(&oled, SSD1331_RED);
    __delay_ms(500);
    SSD1331_FillScreen(&oled, SSD1331_GREEN);
    __delay_ms(500);
    SSD1331_FillScreen(&oled, SSD1331_BLUE);
    __delay_ms(500);
    SSD1331_FillScreen(&oled, SSD1331_WHITE);
    __delay_ms(500);
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    __delay_ms(500);
    
    
    // ==========================
    // TEST 2: RGB COLOR STRIPES
    // ==========================
    // Draw 3 vertical stripes: red, green, blue
    GFX_FillRect(&oled.gfx, &oled, 0, 0, 32, 64, SSD1331_RED);
    GFX_FillRect(&oled.gfx, &oled, 32, 0, 32, 64, SSD1331_GREEN);
    GFX_FillRect(&oled.gfx, &oled, 64, 0, 32, 64, SSD1331_BLUE);
    __delay_ms(1000);

    // ==========================
    // TEST 3: CROSS LINES
    // ==========================
    SSD1331_FillScreen(&oled, SSD1331_BLACK);
    SSD1331_DrawLine(&oled, 0, 0, 95, 63, SSD1331_WHITE);    // Diagonal from top-left to bottom-right
    SSD1331_DrawLine(&oled, 95, 0, 0, 63, SSD1331_WHITE);    // Diagonal from top-right to bottom-left
    GFX_DrawRect(&oled.gfx, &oled, 0, 0, 96, 64, SSD1331_YELLOW); // Yellow rectangle border
    __delay_ms(1000);

    // ==========================
    // TEST 4: TEXT RENDERING
    // ==========================
    SSD1331_FillScreen(&oled, SSD1331_BLACK);

    GFX_SetTextSize(&oled.gfx, 1);
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_PrintAt(&oled.gfx, &oled, 0, 0, "SSD1331 OK!");

    GFX_SetTextColorBg(&oled.gfx, SSD1331_YELLOW, SSD1331_BLUE);
    GFX_PrintAt(&oled.gfx, &oled, 0, 16, "PIC18F26K42");

    GFX_SetTextSize(&oled.gfx, 2);
    GFX_SetTextColor(&oled.gfx, SSD1331_GREEN);
    GFX_PrintAt(&oled.gfx, &oled, 10, 35, "TESTE");
    __delay_ms(2000);

    // ==========================
    // TEST 5: GEOMETRIC SHAPES
    // ==========================
    SSD1331_FillScreen(&oled, SSD1331_BLACK);

    GFX_DrawRect(&oled.gfx, &oled, 5, 5, 40, 25, SSD1331_RED);
    GFX_FillRect(&oled.gfx, &oled, 50, 5, 40, 25, SSD1331_GREEN);
    GFX_DrawCircle(&oled.gfx, &oled, 25, 45, 15, SSD1331_BLUE);
    GFX_FillCircle(&oled.gfx, &oled, 70, 45, 15, SSD1331_YELLOW);
    __delay_ms(2000);

    // ==========================
    // TEST 6: INDIVIDUAL PIXELS PATTERN
    // ==========================
    SSD1331_FillScreen(&oled, SSD1331_BLACK);

    for (int x = 0; x < 96; x += 2) {
        for (int y = 0; y < 64; y += 2) {
            // Create a checkerboard pattern with white and black pixels
            uint16_t color = ((x / 2) % 2 == (y / 2) % 2) ? SSD1331_WHITE : SSD1331_BLACK;
            SSD1331_DrawPixel(&oled, x, y, color);
        }
    }
    __delay_ms(1000);

    // ==========================
    // TEST 7: DISPLAY RGB565 IMAGE
    // ==========================
    // Display the bunmi_img bitmap for 2 seconds
    GFX_DrawBitmapRGB(&oled.gfx, &oled, 0, 0, bunmi_img, SSD1331_WIDTH, SSD1331_HEIGHT);
    __delay_ms(2000);
    __delay_ms(2000);    
    
    GFX_DrawBitmapRGB(&oled.gfx, &oled, 0, 0, manas, SSD1331_WIDTH, SSD1331_HEIGHT);
    __delay_ms(2000);
    __delay_ms(2000);

    // ==========================
    // MAIN LOOP: SIMPLE COUNTER
    // ==========================
    // This part is unreachable due to the infinite loop above,
    // but kept here for reference or future use.

    SSD1331_FillScreen(&oled, SSD1331_BLACK);

    GFX_SetTextSize(&oled.gfx, 1);
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_PrintAt(&oled.gfx, &oled, 25, 0, "Counter:");

        
    uint16_t counter = 0;
    uint8_t progress = 0;
    char buffer[16];
    
    GFX_DrawRect(&oled.gfx, &oled, 10, 40, 76, 10, SSD1331_WHITE);  // Progress bar border

    while (1) {
        LED0_Toggle();

        sprintf(buffer, "%05u", counter);

        GFX_FillRect(&oled.gfx, &oled, 30, 20, 36, 8, SSD1331_BLACK);  // Clear previous number
        GFX_SetTextColor(&oled.gfx, SSD1331_CYAN);
        GFX_PrintAt(&oled.gfx, &oled, 30, 20, buffer);                  // Print counter value

        progress = (counter % 100);

        
        GFX_FillRect(&oled.gfx, &oled, 12, 42, ((progress * 72) / 100)+1, 6, SSD1331_GREEN);  // Progress bar fill
        if (progress == 99) GFX_FillRect(&oled.gfx, &oled, 12, 42, 72 , 6, SSD1331_BLACK);  // Progress bar clear

        counter++;
        if (counter > 9999) counter = 0;

        __delay_ms(100);
    }
}
