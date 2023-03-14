/*
 * my_uart.h
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_UART_H_
#define SRC_INC_MY_UART_H_


void uart_init(void);
uint8_t uart_tx(const char *format, ...);
void uart_manager(void);


#endif /* SRC_INC_MY_UART_H_ */
