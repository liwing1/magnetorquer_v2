/*
 * my_clock.h
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_CLOCK_H_
#define SRC_INC_MY_CLOCK_H_


void clock_init(void);

extern uint8_t timer_flag_50ms;
extern uint8_t timer_flag_100ms;
extern uint8_t timer_flag_500ms;
extern uint8_t timer_flag_1s;
extern uint8_t timer_flag_3s;

#endif /* SRC_INC_MY_CLOCK_H_ */
