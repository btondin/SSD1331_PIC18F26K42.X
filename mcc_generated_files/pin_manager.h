/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F26K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SSD1331_RST aliases
#define SSD1331_RST_TRIS                 TRISAbits.TRISA6
#define SSD1331_RST_LAT                  LATAbits.LATA6
#define SSD1331_RST_PORT                 PORTAbits.RA6
#define SSD1331_RST_WPU                  WPUAbits.WPUA6
#define SSD1331_RST_OD                   ODCONAbits.ODCA6
#define SSD1331_RST_ANS                  ANSELAbits.ANSELA6
#define SSD1331_RST_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define SSD1331_RST_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define SSD1331_RST_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define SSD1331_RST_GetValue()           PORTAbits.RA6
#define SSD1331_RST_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define SSD1331_RST_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define SSD1331_RST_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define SSD1331_RST_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define SSD1331_RST_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define SSD1331_RST_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define SSD1331_RST_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define SSD1331_RST_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set LED0 aliases
#define LED0_TRIS                 TRISBbits.TRISB5
#define LED0_LAT                  LATBbits.LATB5
#define LED0_PORT                 PORTBbits.RB5
#define LED0_WPU                  WPUBbits.WPUB5
#define LED0_OD                   ODCONBbits.ODCB5
#define LED0_ANS                  ANSELBbits.ANSELB5
#define LED0_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define LED0_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define LED0_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define LED0_GetValue()           PORTBbits.RB5
#define LED0_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define LED0_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define LED0_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define LED0_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define LED0_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define LED0_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define LED0_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define LED0_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set SSD1331_DC aliases
#define SSD1331_DC_TRIS                 TRISCbits.TRISC0
#define SSD1331_DC_LAT                  LATCbits.LATC0
#define SSD1331_DC_PORT                 PORTCbits.RC0
#define SSD1331_DC_WPU                  WPUCbits.WPUC0
#define SSD1331_DC_OD                   ODCONCbits.ODCC0
#define SSD1331_DC_ANS                  ANSELCbits.ANSELC0
#define SSD1331_DC_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define SSD1331_DC_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define SSD1331_DC_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define SSD1331_DC_GetValue()           PORTCbits.RC0
#define SSD1331_DC_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define SSD1331_DC_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define SSD1331_DC_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define SSD1331_DC_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define SSD1331_DC_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define SSD1331_DC_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define SSD1331_DC_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define SSD1331_DC_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set SSD1331_CS aliases
#define SSD1331_CS_TRIS                 TRISCbits.TRISC1
#define SSD1331_CS_LAT                  LATCbits.LATC1
#define SSD1331_CS_PORT                 PORTCbits.RC1
#define SSD1331_CS_WPU                  WPUCbits.WPUC1
#define SSD1331_CS_OD                   ODCONCbits.ODCC1
#define SSD1331_CS_ANS                  ANSELCbits.ANSELC1
#define SSD1331_CS_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SSD1331_CS_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SSD1331_CS_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SSD1331_CS_GetValue()           PORTCbits.RC1
#define SSD1331_CS_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SSD1331_CS_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SSD1331_CS_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SSD1331_CS_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SSD1331_CS_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SSD1331_CS_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SSD1331_CS_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define SSD1331_CS_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()              PORTCbits.RC2
#define RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetPullup()             do { WPUCbits.WPUC2 = 1; } while(0)
#define RC2_ResetPullup()           do { WPUCbits.WPUC2 = 0; } while(0)
#define RC2_SetAnalogMode()         do { ANSELCbits.ANSELC2 = 1; } while(0)
#define RC2_SetDigitalMode()        do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/