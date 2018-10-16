/*
 * adc.c
 *
 *  Created on: Jun 3, 2018
 *      Author: Michael McCaffrey
 */

#include "adc.h"

void MSP_ADC_Assign(adc_module * new_adc){

    new_adc->Init = MSP_ADC_Init;
    new_adc->Start = MSP_ADC_Start;
    new_adc->value = 0;

    new_adc->Init();

}

void MSP_ADC_Init(){

    // Configure ADC10
    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_5;                                   // A1 ADC input select; Vref=AVCC
    ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

}

void MSP_ADC_Start(){

    ADCCTL0 |= ADCENC | ADCSC;

}
