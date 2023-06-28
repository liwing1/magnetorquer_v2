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
//    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
//    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
//
//    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);

    // P2.0 -> UCA1SIMO ; P2.1 -> UCA1SOMI ; P2.2 -> UCA1CLK ; P1.3 -> UCA1STE
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_A_SPI_select4PinFunctionality(EUSCI_A1_BASE, EUSCI_A_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE);

    //Initialize Master
    EUSCI_A_SPI_initMasterParam spi_param = {
        .selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK,
        .clockSourceFrequency = CS_getSMCLK(),
        .desiredSpiClock = 1000000,
        .msbFirst = EUSCI_A_SPI_MSB_FIRST,
        .clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
        .clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
        .spiMode = EUSCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW,
    };

    EUSCI_A_SPI_initMaster(EUSCI_A1_BASE, &spi_param);

    //Enable SPI module
    EUSCI_A_SPI_enable(EUSCI_A1_BASE);

    EUSCI_A_SPI_clearInterrupt(EUSCI_A1_BASE,
            EUSCI_A_SPI_RECEIVE_INTERRUPT);
    // Enable USCI_A1 RX interrupt
    EUSCI_A_SPI_enableInterrupt(EUSCI_A1_BASE,
        EUSCI_A_SPI_RECEIVE_INTERRUPT);
}


void spi_tx(uint8_t TXData){
    //USCI_A1 TX buffer ready?
    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A1_BASE,
        EUSCI_A_SPI_TRANSMIT_INTERRUPT)) ;

    //Transmit Data to slave
    EUSCI_A_SPI_transmitData(EUSCI_A1_BASE, TXData);

//    UCA1TXBUF = 0xFF;

//    //USCI_A1 TX buffer ready?
//    while (!EUSCI_A_SPI_getInterruptStatus(EUSCI_A1_BASE,
//        EUSCI_A_SPI_TRANSMIT_INTERRUPT)) ;
//
//    //Transmit Data to slave
//    EUSCI_A_SPI_transmitData(EUSCI_A1_BASE, TXData);
}


uint8_t spi_manager(void){
    uint8_t rx_byte = 0;
    if(spi_handler.rx_flag){
        spi_handler.rx_flag = 0;

        rx_byte = spi_handler.rx_data;
//        uart_tx("SPI_RX: %d\r\n", spi_handler.rx_data);
        spi_handler.rx_data = 0;
    }
    return rx_byte;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV, USCI_SPI_UCTXIFG))
    {
        case USCI_SPI_UCRXIFG:{     // UCRXIFG

            spi_handler.rx_data = EUSCI_A_SPI_receiveData(EUSCI_A1_BASE);
            spi_handler.rx_flag++;

            break;
        }

        default:
            break;
    }
}



