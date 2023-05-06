/*
 * my_gpio.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include "driverlib.h"
#include "my_led.h"


void led_init(void){
    GPIO_setAsOutputPin(LED_PORT, LED_PIN_RED);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_RED);

    GPIO_setAsOutputPin(LED_PORT, LED_PIN_GRN);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_GRN);
}


void led_manager(void){

}
