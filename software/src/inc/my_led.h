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

void led_init(void);


#endif /* SRC_INC_MY_LED_H_ */
