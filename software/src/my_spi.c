/*
 * my_spi.c
 *
 *  Created on: 26 de abr de 2022
 *      Author: liwka
 */

#include <stdint.h>
#include "driverlib.h"
#include "my_spi.h"

uint8_t RXData = 0;

__inline void init_spi(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P2,
        /*GPIO_PIN0 + GPIO_PIN1*/ + GPIO_PIN2 + GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4,
        GPIO_PIN2 + GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_A_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = 500000;
    param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    param.spiMode = EUSCI_A_SPI_3PIN;

    EUSCI_A_SPI_initMaster(EUSCI_A0_BASE, &param);

    //Enable SPI module
    EUSCI_A_SPI_enable(EUSCI_A0_BASE);

    EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE,
            EUSCI_A_SPI_RECEIVE_INTERRUPT);
    // Enable USCI_A0 RX interrupt
    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE,
        EUSCI_A_SPI_RECEIVE_INTERRUPT);

    //Wait for slave to initialize
    __delay_cycles(100);

    //USCI_A0 TX buffer ready?
    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
        EUSCI_A_SPI_TRANSMIT_INTERRUPT)) ;

    //Transmit Data to slave
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, 0);
}

uint8_t spi_send(uint8_t data)
{
    //USCI_A0 TX buffer ready?
    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
        EUSCI_A_SPI_TRANSMIT_INTERRUPT));

    //Send next value
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE,
        data
        );

    return 0;
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV, USCI_SPI_UCTXIFG))
    {
        case USCI_SPI_UCRXIFG:      // UCRXIFG
            RXData = EUSCI_A_SPI_receiveData(EUSCI_A0_BASE);

//            //USCI_A0 TX buffer ready?
//            while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
//                EUSCI_A_SPI_TRANSMIT_INTERRUPT));
//
//            //Increment data
//            TXData = TXData == 3 ? 2:3;
//
//            //Send next value
//            EUSCI_A_SPI_transmitData(EUSCI_A0_BASE,
//                TXData
//                );

            //Delay between transmissions for slave to process information
            __delay_cycles(40);
            break;
        default:
            break;
    }
}
