#include "driverlib.h"
#include "my_uart.h"
#include "my_spi.h"
#include <string.h>
#include <stdio.h>

//******************************************************************************
//!
//!   MAGNETORQUER PROJECT
//!
//******************************************************************************
#define LED_PORT        GPIO_PORT_P1
#define LED_PIN_RED     GPIO_PIN0
#define LED_PIN_GRN     GPIO_PIN1

#ifdef XTCLK_ON
#define XTCLK_PORT      GPIO_PORT_PJ
#define XTCLK_PINS      GPIO_PIN4 + GPIO_PIN5
#endif


__inline static void init_msp(void);


void main (void)
{
    uint8_t data = 1;

    init_msp();
    init_uart();
    init_spi();
    __bis_SR_register(/*LPM4_bits +*/ GIE);


    dbg_printf("MSP INICIALIZADO\r\n");
    while(1)
    {
#ifndef __DEBUG__
        WDT_A_resetTimer(WDT_A_BASE);
#endif

        uart_mng();

        spi_send(data);

        __delay_cycles(100000);
    }
}


__inline static void init_msp(void)
{
#ifdef  __DEBUG__
    WDT_A_hold(WDT_A_BASE); //Parar Watchdog
#else
    WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_ACLK, WDT_A_CLOCKDIVIDER_512K);
    WDT_A_start(WDT_A_BASE);
#endif
    PMM_unlockLPM5();       //Desbloquear Pinos

    //Configurar Sistema de Clocks
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_3);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

#ifdef XTCLK_ON
    GPIO_setAsPeripheralModuleFunctionInputPin(XTCLK_PORT, XTCLK_PINS, GPIO_PRIMARY_MODULE_FUNCTION);
    CS_setExternalClockSource(32768, 0);
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);
    CS_initClockSignal(CS_LFXTCLK_SELECT, CS_ACLK, CS_CLOCK_DIVIDER_1);
#else
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
#endif

    //Configurar GPIO
    GPIO_setAsOutputPin(LED_PORT, LED_PIN_RED);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_RED);

    GPIO_setAsOutputPin(LED_PORT, LED_PIN_GRN);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_GRN);
}

#ifndef __DEBUG__
#pragma vector = WDT_VECTOR
__interrupt void isr_wdt(void)
{
    GPIO_setOutputHighOnPin(LED_PORT, LED_PIN_RED);
    _delay_cycles(1000000);
}
#endif
