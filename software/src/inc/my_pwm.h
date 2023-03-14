/*
 * my_pwm.h
 *
 *  Created on: 2 de mar de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_PWM_H_
#define SRC_INC_MY_PWM_H_

enum{
    H1_IN1,
    H1_IN2,
    H2_IN1,
    H2_IN2,
    H3_IN1,
    H3_IN2,
    HX_MAX
};

typedef struct DRV_TAG{
    uint16_t baseAddress;
    uint16_t compareRegister;
} DRV_t;


extern const DRV_t DRV[HX_MAX];


void pwm_init(void);
void pwm_setDuty(DRV_t timerIndex, uint8_t dutyPercentage);


#endif /* SRC_INC_MY_PWM_H_ */
