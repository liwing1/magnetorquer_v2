#include "driverlib.h"
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_spi.h"
#include "my_pwm.h"
#include "my_adc.h"


void main(void){
    // stop watchdog
    WDT_A_hold(WDT_A_BASE);
//    FRAMCtl_configureWaitStateControl(FRAMCTL_ACCESS_TIME_CYCLES_1);
    PMM_unlockLPM5();

    clock_init();
    led_init();
    uart_init();
    pwm_init();
    adc_init();
//    spi_init();

    __enable_interrupt();

    uart_tx("MSP START: %x\r\n", CS_getSMCLK());

//    uint8_t spi_data = 0;
    while (1)
    {
        uart_manager();
        spi_manager();
        adc_manager();

//        spi_data |= BIT7;
//        spi_tx(spi_data++);
//        uart_tx("sent = %x\r\n", spi_data);

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
        __delay_cycles(5000000/2);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
        __delay_cycles(50000000/2);

    }
}

