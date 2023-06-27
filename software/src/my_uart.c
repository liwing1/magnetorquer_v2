/*
 * my_uart.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "driverlib.h"
#include "my_uart.h"


//#define _EXT_OSC_
#define UART_BUFFER_SIZE    255


typedef struct UART_HANDLER{
    char tx_buffer[UART_BUFFER_SIZE];
    uint8_t tx_idx;

    char rx_buffer[UART_BUFFER_SIZE];
    uint8_t rx_idx;
    uint8_t rx_flag;
}uart_handler_t;

uart_handler_t uart_handler = {0};


void uart_init(void){
    // Configure UART pins
    //Set P4.2 and P4.3 as Secondary Module Function Input.

#ifdef __MSP430FR5994__
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P2,
        GPIO_PIN5 + GPIO_PIN6,
        GPIO_SECONDARY_MODULE_FUNCTION
    );
#else
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4,
        GPIO_PIN2 + GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
    );
#endif

    // Configure UART
    EUSCI_A_UART_initParam uart_param = {
#ifdef _EXT_OSC_ // Config for 32768
        .clockPrescalar = 3,
        .firstModReg = 0,
        .secondModReg = 146,
        .overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION,
#else           // Config for 1MHz
        .clockPrescalar = 104,
        .firstModReg = 2,
        .secondModReg = 182,
        .overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
#endif
        .selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .parity = EUSCI_A_UART_NO_PARITY,
        .msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
        .numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
        .uartMode = EUSCI_A_UART_MODE,
    };

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &uart_param)) {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE,
      EUSCI_A_UART_RECEIVE_INTERRUPT);

    // Enable USCI_A0 RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE,
    EUSCI_A_UART_RECEIVE_INTERRUPT);                     // Enable interrupt
}


// Output message through UART interface
uint8_t uart_tx(const char* format, ...){
    va_list args;
    int done;

    va_start(args, format);

    done = vsnprintf((char*)uart_handler.tx_buffer, UART_BUFFER_SIZE-1, format, args);

    while(uart_handler.tx_buffer[uart_handler.tx_idx])
    {
        EUSCI_A_UART_transmitData(EUSCI_A0_BASE, uart_handler.tx_buffer[uart_handler.tx_idx++]);
    }
    uart_handler.tx_idx = 0;

    va_end(args);

    return done;
}


// Process the received data to execute commands
void uart_manager(void){
    if(uart_handler.rx_flag){
        uart_handler.rx_flag = 0;

        uart_tx("Chegou msg: %s\r\n", uart_handler.rx_buffer);

        memset(uart_handler.rx_buffer, 0, uart_handler.rx_idx);
        uart_handler.rx_idx = 0;
    }
}


//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:{
        uart_handler.rx_buffer[uart_handler.rx_idx++] = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        if(uart_handler.rx_buffer[uart_handler.rx_idx - 1] == '\r'){
            uart_handler.rx_flag = 1;
        }
        break;
    }
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
