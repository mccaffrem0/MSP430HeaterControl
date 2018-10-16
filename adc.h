/*
 * adc.h
 *
 *  Created on: Jun 3, 2018
 *      Author: Michael McCaffrey
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <msp430.h>

typedef struct adc_module {

    void (*Init)(void);
    void (*Start)(void);

    uint16_t value;
    uint8_t ready_flag;

} adc_module;

void MSP_ADC_Assign();
void MSP_ADC_Init();
void MSP_ADC_Start();
uint16_t MSP_ADC_Get();

#endif /* ADC_H_ */
