/*
 * lcd.h
 *
 *  Created on: Jun 3, 2018
 *      Author: dothe
 */

#ifndef LCD_H_
#define LCD_H_

#include <msp430.h>
#include <stdint.h>

#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18

// Define word access definitions to LCD memories
#define LCDMEMW ((int*)LCDMEM)

void LCD_Init();

void LCD_Display(uint16_t number);

#endif /* LCD_H_ */
