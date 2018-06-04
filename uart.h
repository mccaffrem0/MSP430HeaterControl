/*
 * uart.h
 *
 *  Created on: Jun 2, 2018
 *      Author: Michael McCaffrey
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>

typedef struct uarts {

    uint8_t rx;
    uint8_t rx_buffer[256];
    uint8_t buffer_head;
    uint8_t buffer_tail;
    uint8_t buffer_size;
    uint8_t pending_response;

    void (*Init)(void);
    void (*SendByte)(uint8_t byte);
    void (*SendSequence)(uint8_t * sequence_ptr, uint8_t length);
    uint8_t (*Busy)(void);

} uart_module;

void MSP_UART_Assign(uart_module * new_uart);
void MSP_UART_Init();
void MSP_UART_SendByte(uint8_t byte);
void MSP_UART_SendSequence(uint8_t * sequence_ptr, uint8_t length);
uint8_t MSP_UART_Busy();

#endif /* UART_H_ */
