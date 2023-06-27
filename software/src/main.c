#include "driverlib.h"
#include "DRV8837.h"
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_spi.h"
#include "my_adc.h"


led_t led_red = {
                 .port = LED_PORT,
                 .pin = LED_PIN_RED,
                 .on = 0,
                 .request = 0
};

led_t led_grn = {
                 .port = LED_PORT,
                 .pin = LED_PIN_GRN,
                 .on = 0,
                 .request = 0
};

void main(void){

    // stop watchdog
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    clock_init();
    uart_init();
    pwm_init();
    adc_init();
    spi_init();
    led_init(&led_red);
    led_init(&led_grn);

    __enable_interrupt();

    uart_tx("MSP START: %x\r\n", CS_getSMCLK());

    uint8_t spi_data = 0x8F;

    while (1){
        if(timer_flag_50ms){
            timer_flag_50ms = 0;

            led_manager(&led_grn);
            led_manager(&led_red);
        }
        else if(timer_flag_100ms){
            timer_flag_100ms = 0;

            uart_manager();
            spi_manager();

        }
        else if(timer_flag_500ms){
            timer_flag_500ms = 0;

            led_blink(&led_red);
        }
        else if(timer_flag_1s){
            timer_flag_1s = 0;

            spi_tx(spi_data);
//            uint8_t i;
//            uint16_t b[3];
//            for(i = 0; i < 3; i++){
//                b[i] = adc_read_mag(i);
//            }
//            uart_tx("%d,%d,%d\n", b[0], b[1], b[2]);

        }
        else if(timer_flag_5s){
            timer_flag_5s = 0;

            led_blink(&led_grn);
            mag_states();
        }

//        spi_data |= BIT7;
//        spi_tx(spi_data++);
//        uart_tx("sent = %x\r\n", spi_data);
    }
}

