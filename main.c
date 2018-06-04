#include <msp430.h> 

/**
 * main.c
 * Author: Michael McCaffrey
 * Email: mccaffrem0@students.rowan.edu
 * UART WORKS
 * ADC WORKS
 * BASIC LCD WORKS
 *
 * TODO:
 * SCPI-Lite Protocol
 */

#include "system.h"

system sys;

int main(void)
{

    SystemInit(&sys);

    while (1)
    {
        if (sys.uart.pending_response)
        {
            Respond(&sys);
        }
    }

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_UART_UCRXIFG:
        sys.uart.rx = UCA0RXBUF;
        if (sys.uart.rx == 13)
        {
            sys.uart.pending_response = 1;
        }
        else
        {
            sys.uart.rx_buffer[sys.uart.buffer_size] = sys.uart.rx;
            sys.uart.buffer_size++;
        }
        UCA0IFG &= ~ UCRXIFG; // Clear interrupt
        break;
    case USCI_NONE:
    case USCI_UART_UCTXIFG:
    case USCI_UART_UCSTTIFG:
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {

        case ADCIV_ADCIFG:
            sys.adc.value = ADCMEM0;
            sys.adc.ready_flag = 1;
//            __bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
            break;
        case ADCIV_NONE:
        case ADCIV_ADCOVIFG:
        case ADCIV_ADCTOVIFG:
        case ADCIV_ADCHIIFG:
        case ADCIV_ADCLOIFG:
        case ADCIV_ADCINIFG:
        default:
            break;
    }
}

