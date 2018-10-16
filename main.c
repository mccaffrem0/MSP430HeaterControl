
/**
 * main.c
 * Author: Michael McCaffrey
 * Email: mccaffrem0@students.rowan.edu
 * UART WORKS
 * ADC WORKS
 * BASIC LCD WORKS
 *
 * This code allows an MSP430FR4133 to fetch temperature from an SI diode via ADC and lookup table.
 * Data is displayed on LCD and can also be delivered over UART connection.
 * Currently, temperature data is sent in response to an incoming carriage return.
 * Additional functionality would require more complex messages, current idea is a simple version of SCPI protocol.
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

        if (sys.timer_flag)
        {
            ADC_Update(&sys);
            LCD_Display(SI_Lookup(sys.adc.value));
            sys.timer_flag = 0;
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
        else    // only necessary if using some protocol
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

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    sys.timer_flag = 1;
//    P4OUT ^= BIT0;

}
