/*
 * my_gpio.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include "driverlib.h"
#include "my_led.h"



void led_init(led_t* led_obj){
    GPIO_setAsOutputPin(LED_PORT, LED_PIN_RED);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_RED);

    GPIO_setAsOutputPin(LED_PORT, LED_PIN_GRN);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_GRN);
}

void led_blink(led_t* led_obj){
    led_obj->request = 1;
}

void led_manager(led_t* led_obj){
    if(led_obj->request){
        led_obj->on = 1;
        led_obj->request = 0;
        GPIO_setOutputHighOnPin(led_obj->port, led_obj->pin);
    }
    else if(led_obj->on){
        led_obj->on = 0;
        GPIO_setOutputLowOnPin(led_obj->port, led_obj->pin);
    }
}
