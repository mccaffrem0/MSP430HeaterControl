/*
 * lcd.c
 *
 *  Created on: Jun 3, 2018
 *      Author: dothe
 */

#ifndef LCD_C_
#define LCD_C_

#include "lcd.h"

const char digit[10] = {
        0xFC,                                                      // "0"
        0x60,                                                      // "1"
        0xDB,                                                      // "2"
        0xF3,                                                      // "3"
        0x67,                                                      // "4"
        0xB7,                                                      // "5"
        0xBF,                                                      // "6"
        0xE4,                                                      // "7"
        0xFF,                                                      // "8"
        0xF7                                                       // "9"
        };

void LCD_Init()
{

    P4SEL0 |= BIT1 | BIT2;                            // P4.2~P4.1: crystal pins

    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }
    while (SFRIFG1 & OFIFG);                       // Test oscillator fault flag

    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2; // Higher drive strength and current consumption for XT1 oscillator

    // Configure LCD pins
    SYSCFG2 |= LCDPCTL;             // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

    LCDPCTL0 = 0xFFFF;
    LCDPCTL1 = 0x07FF;
    LCDPCTL2 = 0x00F0;                         // L0~L26 & L36~L39 pins selected

    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                    // flcd ref freq is xtclk

    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6
            | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);

    LCDMEMCTL |= LCDCLRM;                                    // Clear LCD memory

    LCDCSSEL0 = 0x000F;                               // Configure COMs and SEGs
    LCDCSSEL1 = 0x0000;                              // L0, L1, L2, L3: COM pins
    LCDCSSEL2 = 0x0000;

    LCDM0 = 0x21;                                        // L0 = COM0, L1 = COM1
    LCDM1 = 0x84;                                        // L2 = COM2, L3 = COM3

    LCDCTL0 |= LCD4MUX | LCDON;                   // Turn on LCD, 4-mux selected

}

void LCD_Display(uint16_t number)
{

    LCDMEM[pos1] = digit[number / 1000];
    LCDMEM[pos2] = digit[(number % 1000) / 100];
    LCDMEM[pos3] = digit[(number % 100) / 10];
    LCDMEM[pos4] = digit[number % 10];
    LCDMEMW[9] = 0x220E;
    LCDMEM[9] = 0x1; //Decimal point

}

#endif /* LCD_C_ */
