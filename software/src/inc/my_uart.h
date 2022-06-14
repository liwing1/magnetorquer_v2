/*
 * my_uart.h
 *
 *  Created on: 4 de abr de 2022
 *      Author: liwka
 */

#ifndef MY_UART_H_
#define MY_UART_H_

__inline void init_uart(void);
uint8_t dbg_printf(const char *format, ...);

//#define XTCLK_ON
#define DBG_BUFFER_SIZE 255

typedef struct{
    uint8_t buffer[DBG_BUFFER_SIZE];
    uint8_t read;
    uint8_t write;
    uint8_t error;
}uart_rx_t;

typedef struct{
    uint8_t buffer[DBG_BUFFER_SIZE];
    uint8_t idx;
    uint8_t error;
}uart_tx_t;

extern uart_rx_t uart_rx;
extern uart_tx_t uart_tx;

uint8_t uart_mng(void);

#endif /* MY_UART_H_ */
