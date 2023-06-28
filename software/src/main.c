#include "driverlib.h"
#include "DRV8837.h"
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_spi.h"
#include "my_adc.h"
#include "LSM6DS3.h"


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

    LSM6DS3_init();
    uart_tx("MSP START: %x\r\n", CS_getSMCLK());
    uart_tx("mag_x, mag_y, mag_z, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z\n");


    while (1){
        if(timer_flag_50ms){
            timer_flag_50ms = 0;

            led_manager(&led_grn);
            led_manager(&led_red);
        }
        else if(timer_flag_100ms){
            timer_flag_100ms = 0;

            uart_manager();

            LSM6DS3_update();

            uint16_t b[3];
            int16_t a[3];
            int16_t g[3];
            uint8_t i;
            for(i = 0; i < 3; i++){
                b[i] = adc_read_raw_mean((mag_idx_t)i);
                a[i] = LSM6DS3_getAccelData(i);
                g[i] = LSM6DS3_getGyrosData(i);
            }
            uart_tx("%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                    b[1], b[0], b[2],
                    a[0], a[1], a[2],
                    g[0], g[1], g[2]
                                  );

        }
        else if(timer_flag_500ms){
            timer_flag_500ms = 0;

        }
        else if(timer_flag_1s){
            timer_flag_1s = 0;
            led_blink(&led_red);

        }
        else if(timer_flag_5s){
            timer_flag_5s = 0;
            led_blink(&led_grn);
//            mag_states();
        }
        __low_power_mode_4();
    }
}

