/*
 * my_gpio.h
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_LED_H_
#define SRC_INC_MY_LED_H_


#define LED_PORT        GPIO_PORT_P1
#define LED_PIN_RED     GPIO_PIN0
#define LED_PIN_GRN     GPIO_PIN1

typedef struct{
    uint8_t port;
    uint8_t pin;
    uint8_t request;
    uint8_t on;
} led_t;


void led_init(led_t* led_obj);
void led_blink(led_t* led_obj);
void led_manager(led_t* led_obj);

#endif /* SRC_INC_MY_LED_H_ */
