/*
 * system.c
 *
 *  Created on: Jun 3, 2018
 *      Author: Michael McCaffrey
 */

#include "system.h"

void SystemInit(system * sys){

    __enable_interrupt();
    CSCTL0 = 0;                             // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                 // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                    // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P4DIR = 0x01;
    P4OUT = 0;

    MSP_ADC_Assign(&(sys->adc));
    MSP_UART_Assign(&(sys->uart));

    LCD_Init();
    sys->adc.Start();
    while(sys->adc.ready_flag == 0){}
    uint8_t analog_value[2] = {sys->adc.value >> 8, sys->adc.value & 0xff};
    sys->uart.SendSequence(analog_value, 2);
    LCD_Display(SI_Lookup(sys->adc.value));

    P1DIR |= BIT4;
    P1OUT |= BIT4;

    TimerInit();


}

void Respond(system * sys){

    ADC_Update(sys);
    uint16_t value = SI_Lookup(sys->adc.value);
    sys->uart.SendSequence((uint8_t*)&value, 2);
    sys->uart.pending_response = 0;

}

void ADC_Update(system * sys){

    sys->adc.ready_flag = 0;
    sys->adc.Start();
    while(sys->adc.ready_flag == 0){}

}
