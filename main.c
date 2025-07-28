/**
 * @file main.c
 * @brief Main application for SSD1331 OLED display test using PIC18F26K42
 * * @details
 * This application initializes and tests the SSD1331 96x64 RGB OLED display.
 * It is a direct port of the Adafruit SSD1331 library example sketch to
 * ensure identical graphical output.
 * * Author @btondin, with conversion logic by Gemini
 * @date    2025
 */

#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ssd1331.h"
#include "screens.h"
// #include "screens.h" // Not needed for this test

// Global instance of the OLED display driver
SSD1331_t oled;

// Test delay times from Arduino example
#define DELAY_SHORT 500
#define DELAY_MEDIUM 1000
#define DELAY_LONG 2000

// Function prototypes for all tests from test.cpp
void testlines(uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testroundrects(void);
void testtriangles(void);
void tftPrintTest(void);
void mediabuttons(void);
void lcdTestPattern(void);
void testimages(void);


/**
 * @brief Main entry point of the application.
 */
void main(void) {
    // Initialize device and system peripherals
    SYSTEM_Initialize();

    // Open SPI1 with default settings
    SPI1_Open(SPI1_DEFAULT);

    // Initialize and start the OLED display
    SSD1331_Init(&oled);
    SSD1331_Begin(&oled);
    
    // This is the PIC equivalent of the setup() function in test.cpp
    
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    __delay_ms(DELAY_SHORT);

    
    
    lcdTestPattern();
    __delay_ms(DELAY_MEDIUM);

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    GFX_SetCursor(&oled.gfx, 0, 0);
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_SetTextWrap(&oled.gfx, true);
    GFX_Print(&oled.gfx, &oled, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
    __delay_ms(DELAY_MEDIUM);
    
 

    // tft print function!
    tftPrintTest();
    __delay_ms(DELAY_LONG);
    
    
    
    // a single pixel
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    GFX_DrawPixel(&oled.gfx, &oled, oled.gfx.width / 2, oled.gfx.height / 2, SSD1331_GREEN);
    __delay_ms(DELAY_SHORT);
    
     
    

    // line draw test
    testlines(SSD1331_YELLOW);
    __delay_ms(DELAY_SHORT);
    
    

    // optimized lines
    testfastlines(SSD1331_RED, SSD1331_BLUE);
    __delay_ms(DELAY_SHORT);
    
    

    testdrawrects(SSD1331_GREEN);
    __delay_ms(DELAY_MEDIUM);
    
    

    testfillrects(SSD1331_YELLOW, SSD1331_MAGENTA);
    __delay_ms(DELAY_MEDIUM);
    
    

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    testfillcircles(10, SSD1331_BLUE);
    testdrawcircles(10, SSD1331_WHITE);
    __delay_ms(DELAY_MEDIUM);
    
    

    testroundrects();
    __delay_ms(DELAY_SHORT);
    
    
    testtriangles();
    __delay_ms(DELAY_SHORT);
    
    
    testimages();
    __delay_ms(DELAY_MEDIUM);
    __delay_ms(DELAY_MEDIUM);    
    
    // Loop forever after tests are done
    while (1) {
        // The tests run only once, like in Arduino's setup()
        // You can add your main application logic here.
        LED0_Toggle();
        __delay_ms(100);
    }
}

void testlines(uint16_t color) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, 0, x, oled.gfx.height - 1, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, 0, oled.gfx.width - 1, y, color);
    }
    //__delay_ms(DELAY_SHORT);

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, 0, x, oled.gfx.height - 1, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, 0, 0, y, color);
    }
    //__delay_ms(DELAY_SHORT);

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, oled.gfx.height - 1, x, 0, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, 0, oled.gfx.height - 1, oled.gfx.width - 1, y, color);
    }
    //__delay_ms(DELAY_SHORT);

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.width; x += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, oled.gfx.height - 1, x, 0, color);
    }
    for (int16_t y = 0; y < oled.gfx.height; y += 6) {
        GFX_DrawLine(&oled.gfx, &oled, oled.gfx.width - 1, oled.gfx.height - 1, 0, y, color);
    }
}

void testfastlines(uint16_t color1, uint16_t color2) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t y = 0; y < oled.gfx.height; y += 5) {
        GFX_DrawFastHLine(&oled.gfx, &oled, 0, y, oled.gfx.width, color1);
    }
    for (int16_t x = 0; x < oled.gfx.width; x += 5) {
        GFX_DrawFastVLine(&oled.gfx, &oled, x, 0, oled.gfx.height, color2);
    }
}

void testdrawrects(uint16_t color) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    for (int16_t x = 0; x < oled.gfx.height-1; x += 6) {
        GFX_DrawRect(&oled.gfx, &oled, (oled.gfx.width - 1) / 2 - x / 2, (oled.gfx.height - 1) / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    //for (int16_t x = oled.gfx.width - 1; x > 6; x -= 6) {
     //   GFX_FillRect(&oled.gfx, &oled, (oled.gfx.width - 1) / 2 - x / 2, (oled.gfx.height - 1) / 2 - x / 2, x, x, color1);
     //   GFX_DrawRect(&oled.gfx, &oled, (oled.gfx.width - 1) / 2 - x / 2, (oled.gfx.height - 1) / 2 - x / 2, x, x, color2);
    //}
    for (int16_t x = oled.gfx.height - 1; x > 6; x -= 6) {
        GFX_FillRect(&oled.gfx, &oled, (oled.gfx.width - 1) / 2 - x / 2, (oled.gfx.height - 1) / 2 - x / 2, x, x, color1);
        GFX_DrawRect(&oled.gfx, &oled, (oled.gfx.width - 1) / 2 - x / 2, (oled.gfx.height - 1) / 2 - x / 2, x, x, color2);
    }
}

void testfillcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = radius; x < oled.gfx.width; x += radius * 2) {
        for (int16_t y = radius; y < oled.gfx.height; y += radius * 2) {
            GFX_FillCircle(&oled.gfx, &oled, x, y, radius, color);
        }
    }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = 0; x < oled.gfx.width + radius; x += radius * 2) {
        for (int16_t y = 0; y < oled.gfx.height + radius; y += radius * 2) {
            GFX_DrawCircle(&oled.gfx, &oled, x, y, radius, color);
        }
    }
}

void testroundrects(void) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    uint32_t color = 100; // Mude de 'int' para 'uint16_t'
    int i;
    int t;
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

void testtriangles(void) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    uint16_t color = 0xF800; // Mude de 'int' para 'uint16_t'
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

// 2. Crie um buffer para armazenar o texto formatado
    char buffer[20]; 

void tftPrintTest(void) {
    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
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
    GFX_Print(&oled.gfx, &oled, buffer); // PIC doesn't have easy float printing, using string
    
    
    
    __delay_ms(DELAY_LONG);

    GFX_FillScreen(&oled.gfx, &oled, SSD1331_BLACK);
    GFX_SetCursor(&oled.gfx, 0, 5);
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_SetTextSize(&oled.gfx, 1); // NOTE: Adafruit GFX treats size 0 as 1
    GFX_Print(&oled.gfx, &oled, "Hello World!\n");
    
    float pi = 3.14159;

    // 2. Crie um buffer para armazenar o texto formatado
    char buffer[20]; 

    // 3. Use sprintf para "imprimir" o float dentro do buffer
    sprintf(buffer, "%.5f Want pi?\n", pi);
    
    GFX_SetTextSize(&oled.gfx, 1);
    GFX_SetTextColor(&oled.gfx, SSD1331_GREEN);
    GFX_Print(&oled.gfx, &oled, buffer);

       
    
    // Printing numbers requires a buffer in C
    //char buffer[20];
    sprintf(buffer, "%lX", 8675309L); // C requires explicit hex conversion
    GFX_Print(&oled.gfx, &oled, buffer);
    GFX_Print(&oled.gfx, &oled, " Print HEX\n");
    
    

    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_Print(&oled.gfx, &oled, "Sketch has been\n");
    GFX_Print(&oled.gfx, &oled, "running for: \n");
    
    GFX_SetTextColor(&oled.gfx, SSD1331_MAGENTA);
    // millis() is not available, we can just print a static number for the test
    uint8_t sim_millis = 6;
    sprintf(buffer, "%d ", sim_millis);
    GFX_Print(&oled.gfx, &oled, buffer); 
    GFX_SetTextColor(&oled.gfx, SSD1331_WHITE);
    GFX_Print(&oled.gfx, &oled, "seconds.");
    
    
}


void testimages(void) {
    // play
     GFX_DrawBitmapRGB(&oled.gfx, &oled, 0, 0, bunmi_img, SSD1331_WIDTH, SSD1331_HEIGHT);
    //__delay_ms(2000);
    //__delay_ms(2000);    
    
    //GFX_DrawBitmapRGB(&oled.gfx, &oled, 0, 0, manas, SSD1331_WIDTH, SSD1331_HEIGHT);
}

void lcdTestPattern(void) {
    static const uint16_t colors[] = {
        SSD1331_BLACK, SSD1331_YELLOW, SSD1331_MAGENTA, SSD1331_RED,
        SSD1331_CYAN, SSD1331_GREEN, SSD1331_BLUE, SSD1331_WHITE
    };

    for (uint8_t h = 0; h < oled.gfx.height; h++) {
        for (uint8_t w = 0; w < oled.gfx.width; w++) {
            // This logic is a bit different from Adafruit's which uses writePixel
            // To be identical, we must use the GFX drawPixel
            int band = w / 12;
            if (band > 7) band = 7;
            GFX_DrawPixel(&oled.gfx, &oled, w, h, colors[band]);
        }
    }
}