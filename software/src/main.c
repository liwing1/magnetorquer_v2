#include "driverlib.h"
#include "DRV8837.h"
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_spi.h"
#include "my_adc.h"
#include "SS49e.h"


void main(void){
    SS49e_t hall_sens = {
                         .magIdx = MAG_IDX_2,
                         .sensitivity = 2.0,
    };

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

    SS49e_init(&hall_sens);

    uart_tx("MSP START: %x\r\n", CS_getSMCLK());

//    uint8_t spi_data = 0;
    uint8_t pwm_value = 0;
    float field;
    while (1)
    {
        if(timer_flag_50ms){
            timer_flag_50ms = 0;
        }
        else if(timer_flag_100ms){
            timer_flag_100ms = 0;

            uart_manager();
            spi_manager();

        }
        else if(timer_flag_500ms){
            timer_flag_500ms = 0;

            GPIO_toggleOutputOnPin(LED_PORT, LED_PIN_RED);
        }
        else if(timer_flag_1s){
            timer_flag_1s = 0;

//            adc_manager();
            field =  SS49e_getField(&hall_sens) * 1000;
            uart_tx("FIELD[%d]: %d\r\n", hall_sens.magIdx, (int32_t)field);

        }
        else if(timer_flag_3s){
            timer_flag_3s = 0;

            pwm_value = (pwm_value == 100 ? 0 : pwm_value + 5);
            pwm_setDuty(DRV[H1_IN1], pwm_value);
            pwm_setDuty(DRV[H2_IN1], pwm_value);
            pwm_setDuty(DRV[H3_IN1], pwm_value);
//            uart_tx("PWM: %d\r\n", pwm_value);

            GPIO_toggleOutputOnPin(LED_PORT, LED_PIN_GRN);
        }

//        spi_data |= BIT7;
//        spi_tx(spi_data++);
//        uart_tx("sent = %x\r\n", spi_data);
    }
}

