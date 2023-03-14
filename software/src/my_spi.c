/*
 * my_spi.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */


#include "driverlib.h"
#include "my_spi.h"
#include "my_uart.h"


#define WHO_AM_I    0x0F


typedef struct SPI_HANDLER{
    uint8_t tx_data[2];
    uint8_t tx_idx;

    uint8_t rx_data;
    uint8_t rx_flag;
}spi_handler_t;

spi_handler_t spi_handler = {0};

void spi_init(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1);
//    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN1);
//    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN1);

    // P2.0 -> UCA0SIMO ; P2.1 -> UCA0SOMI ; P2.2 -> UCA0CLK ; P1.3 -> UCA0STE
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P2,
        GPIO_PIN3 + GPIO_PIN2 + GPIO_PIN1 + GPIO_PIN0,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_A_SPI_select4PinFunctionality(EUSCI_A0_BASE, EUSCI_A_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE);

    //Initialize Master
    EUSCI_A_SPI_initMasterParam spi_param = {
        .selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK,
        .clockSourceFrequency = CS_getSMCLK(),
        .desiredSpiClock = 10000000,
        .msbFirst = EUSCI_A_SPI_MSB_FIRST,
        .clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
        .clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
        .spiMode = EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW,
    };

    EUSCI_A_SPI_initMaster(EUSCI_A0_BASE, &spi_param);

    //Enable SPI module
    EUSCI_A_SPI_enable(EUSCI_A0_BASE);

    EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE,
            EUSCI_A_SPI_RECEIVE_INTERRUPT);
    // Enable USCI_A0 RX interrupt
    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE,
        EUSCI_A_SPI_RECEIVE_INTERRUPT);
}


void spi_tx(uint8_t TXData){
    //USCI_A0 TX buffer ready?
    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
        EUSCI_A_SPI_TRANSMIT_INTERRUPT)) ;

    //Transmit Data to slave
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, TXData);

    UCA0TXBUF = 0xFF;

//    //USCI_A0 TX buffer ready?
//    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A0_BASE,
//        EUSCI_A_SPI_TRANSMIT_INTERRUPT)) ;
//
//    //Transmit Data to slave
//    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, TXData);
}


void spi_manager(void){
    if(spi_handler.rx_flag){
        spi_handler.rx_flag = 0;

        uart_tx("%d\r\n", spi_handler.rx_data);
        spi_handler.rx_data = 0;
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV, USCI_SPI_UCTXIFG))
    {
        case USCI_SPI_UCRXIFG:{     // UCRXIFG

            spi_handler.rx_data = EUSCI_A_SPI_receiveData(EUSCI_A0_BASE);

            if(spi_handler.rx_data != 0){
                spi_handler.rx_flag++;
            }

            break;
        }

        default:
            break;
    }
}



