/*
 * uart.c
 *
 *  Created on: Jun 2, 2018
 *      Author: dothe
 */

#include "uart.h"
#include <msp430.h>

void MSP_UART_Assign(uart_module * new_uart){
    new_uart->Init = MSP_UART_Init;
    new_uart->SendByte = MSP_UART_SendByte;
    new_uart->SendSequence = MSP_UART_SendSequence;
    new_uart->Busy = MSP_UART_Busy;
}


void MSP_UART_Init(){
    P1SEL0 |= BIT0 | BIT1; // UART function on P1.0 and P1.1
    UCA0CTLW0 = UCSSEL__SMCLK;
    UCA0BR0 = 52; // for 9600 baud
    UCA0BR1 = 0x00;
    UCA0MCTLW = 0x4900 | UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;
}

void MSP_UART_SendByte(uint8_t byte){
    UCA0TXBUF = byte;
    while(MSP_UART_Busy()){}
}

void MSP_UART_SendSequence(uint8_t * buffer_ptr, uint8_t length){
    int i;
    for (i = 1; i < length; i++)
    {
        MSP_UART_SendByte(*buffer_ptr++);
    }
}

void MSP_UART_BufferPush(uint8_t byte){

}

uint8_t MSP_UART_Busy(){
    return UCA0STATW & UCBUSY;
}

void HandleBuffer(uart_module * uart){
    uart->SendSequence(uart->rx_buffer, uart->buffer_size);
    uart->buffer_size = 0;
    uart->rx_pending_flag = 0;
}