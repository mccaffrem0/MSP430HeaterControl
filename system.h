/*
 * system.h
 *
 *  Created on: Jun 3, 2018
 *      Author: Michael McCaffrey
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <msp430.h>
#include <stdarg.h>
#include "uart.h"
#include "adc.h"
#include "lcd.h"

typedef struct system {

    uart_module uart;
    adc_module adc;

} system;

void SystemInit(system * sys);
void Respond(system * sys);

#endif /* SYSTEM_H_ */
