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
#include "timer.h"
#include "temp.h"

typedef struct system {

    uart_module uart;
    adc_module adc;

    uint8_t timer_flag;

} system;

void ADC_Update(system * sys);
void SystemInit(system * sys);
void Respond(system * sys);

#endif /* SYSTEM_H_ */
