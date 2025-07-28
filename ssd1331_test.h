/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "ssd1331.h" // include processor files - each processor file is guarded. 
#include "gfx_pic.h" // include processor files - each processor file is guarded. 

// TODO Insert appropriate #include <>

/**
 * @brief Write a pixel to the current address window position
 * @note Must be called after SSD1331_SetAddrWindow and between Select/Deselect
 */
static void SSD1331_WritePixel(SSD1331_t *ssd, uint16_t color) {
    uint8_t buffer[2];
    buffer[0] = color >> 8;
    buffer[1] = color & 0xFF;
    SPI1_ExchangeBlock(buffer, 2);
}

/**
 * @brief Renders a simple test pattern on the LCD
 */
void SSD1331_TestPattern(SSD1331_t *ssd) {
    uint8_t w, h;
    
    // Não precisa de SetAddrWindow quando usando GFX_DrawPixel
    // pois cada DrawPixel configura sua própria janela
    
    for (h = 0; h < 64; h++) {
        for (w = 0; w < 96; w++) {
            uint16_t color;
            
            if (w > 83) {
                color = SSD1331_WHITE;
            } else if (w > 71) {
                color = SSD1331_BLUE;
            } else if (w > 59) {
                color = SSD1331_GREEN;
            } else if (w > 47) {
                color = SSD1331_CYAN;
            } else if (w > 35) {
                color = SSD1331_RED;
            } else if (w > 23) {
                color = SSD1331_MAGENTA;
            } else if (w > 11) {
                color = SSD1331_YELLOW;
            } else {
                color = SSD1331_BLACK;
            }
            
            // Write pixel using GFX function
            GFX_DrawPixel(&ssd->gfx, ssd, w, h, color);
        }
    }
}
    
    // End write transaction
    //SSD1331_Deselect(ssd);
}
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

