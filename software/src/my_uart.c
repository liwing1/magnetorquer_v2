/*
 * my_uart.c
 *
 *  Created on: 4 de abr de 2022
 *      Author: liwka
 */
#include <driverlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "my_uart.h"


uart_rx_t uart_rx = {0};
uart_tx_t uart_tx = {0};


__inline void init_uart(void)
{
    EUSCI_A_UART_initParam uart_param =
    {
     .clockPrescalar = 26,
     .firstModReg = 0,
     .secondModReg = 214,
     .overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,

     .selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     .parity = EUSCI_A_UART_NO_PARITY,
     .msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
     .numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
     .uartMode = EUSCI_A_UART_MODE
    };

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    if(STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, (EUSCI_A_UART_initParam*) &uart_param)){
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT);
}

uint8_t dbg_printf(const char *format, ...)
{
    va_list args;
    int done;

    va_start(args, format);

    done = vsnprintf((char*)uart_tx.buffer, DBG_BUFFER_SIZE-1, format, args);

    while(uart_tx.buffer[uart_tx.idx])
    {
        EUSCI_A_UART_transmitData(EUSCI_A1_BASE, uart_tx.buffer[uart_tx.idx++]);
    }
    uart_tx.idx = 0;

    va_end(args);

    return done;
}


uint8_t uart_mng(void)
{
    uint8_t buffer[DBG_BUFFER_SIZE];

    if(uart_rx.read)
    {
        sprintf((char*)buffer ,"chegou: %s\r\n", (char*)uart_rx.buffer);
        dbg_printf((char*)buffer);
        uart_rx.write = 0;
        uart_rx.read = 0;
        memset(uart_rx.buffer, 0, DBG_BUFFER_SIZE);

        //USCI_A0 TX buffer ready?
        while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
            EUSCI_A_SPI_TRANSMIT_INTERRUPT));

        //Send next value
        EUSCI_A_SPI_transmitData(EUSCI_A0_BASE,
            0x0A
            );

        //Delay between transmissions for slave to process information
        __delay_cycles(40);
    }

    return 0;
}


//******************************************************************************
//
//This is the USCI_A1 interrupt vector service routine.
//
//******************************************************************************

#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
    {
        uart_rx.buffer[uart_rx.write++] = EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
        if(uart_rx.buffer[uart_rx.write - 1] == '\r')
            uart_rx.read = 1;
    }
    break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
