/*
 * my_gpio.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include "driverlib.h"


#define LED_PORT        GPIO_PORT_P1
#define LED_PIN_RED     GPIO_PIN0
#define LED_PIN_GRN     GPIO_PIN1


void led_init(void){
    GPIO_setAsOutputPin(LED_PORT, LED_PIN_RED);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_RED);

    GPIO_setAsOutputPin(LED_PORT, LED_PIN_GRN);
    GPIO_setOutputLowOnPin(LED_PORT, LED_PIN_GRN);
}

