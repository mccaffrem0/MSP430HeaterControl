#include <msp430.h> 

/**
 * main.c
 * Author: Michael McCaffrey
 * Email: mccaffrem0@students.rowan.edu
 * LEFT OFF:
 * UART Module is mostly done. TX and RX interrupt works well so far.
 * UART RX fills up buffer and handles it upon receiving a CR.
 *
 * TODO:
 * LCD Stuff
 * ADC Stuff
 * SCPI-Lite Protocol
 */

#include "uart.h"

uart_module uart;
uint8_t rx;

int main(void)
{
    __enable_interrupt();
    CSCTL0 = 0;                              // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);             // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                     // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                   // DCODIV = 8MHz
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P4DIR = 0x01;
    P4OUT = 0;

    MSP_UART_Assign(&uart);
    uart.Init();
    uint8_t sequence[] = "Test\n\r";
    long i = 0;
    while (1)
    {
        for (i = 0x5ffff; i != 0; i--)
        {
        }
        uart.SendSequence(sequence, sizeof(sequence));
        if(uart.rx_pending_flag) HandleBuffer(&uart);
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
    case USCI_NONE:
        break;
    case USCI_UART_UCRXIFG:
        rx = UCA0RXBUF;
        if(rx == 13) uart.rx_pending_flag = 1;
        else {
            uart.rx_buffer[uart.buffer_size] = rx;
            uart.buffer_size++;
        }
        UCA0IFG &= ~ UCRXIFG; // Clear interrupt
        break;
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}
