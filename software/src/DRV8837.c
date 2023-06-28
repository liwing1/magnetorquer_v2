/*
 * my_pwm.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: liwka
 */


#include "driverlib.h"
#include "DRV8837.h"
#include "my_uart.h"


#define DRV_PORT        GPIO_PORT_P6
#define DRV_PIN_nSLP    GPIO_PIN4

// PWMfreq = SMCLK / TIMER_PERIOD -> 16MHz/16000 = 1KHz
#define PWM_PERIOD    25000

typedef enum{
    STATE_FIRST_MAG,
    STATE_DEMAG_POS,
    STATE_DEMAG_NEG,
    STATE_MAG_NEG,
    STATE_MAG_POS,
} states_t;

states_t state = STATE_FIRST_MAG;

static const Timer_A_initUpModeParam pwm_param = {
    .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
    .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64,
    .timerPeriod = PWM_PERIOD,
    .timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,
    .captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
    .timerClear = TIMER_A_DO_CLEAR,
    .startTimer = false
};

const DRV_t DRV[HX_MAX] = {
    [H1_IN1] = {    //TA0.1 ; H1_IN1; P7.2
           .baseAddress = TIMER_A0_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1
    },
    [H1_IN2] = {    //TA0.2 ; H1_IN2; P7.3
           .baseAddress = TIMER_A0_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2
    },

    [H2_IN1] = {    //TA3.3 ; H2_IN1; P3.1
           .baseAddress = TIMER_A3_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3
    },
    [H2_IN2] = {    //TA3.2 ; H2_IN2; P3.0
           .baseAddress = TIMER_A3_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2
    },

    [H3_IN1] = {    //TA1.1 ; H3_IN1; P4.6
           .baseAddress = TIMER_A1_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1
    },
    [H3_IN2] = {    //TA1.2 ; H3_IN2; P4.7
           .baseAddress = TIMER_A1_BASE,
           .compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2
    }
};

void pwm_init (void)
{
    GPIO_setAsOutputPin(DRV_PORT, DRV_PIN_nSLP);
//    GPIO_setOutputHighOnPin(DRV_PORT, DRV_PIN_nSLP);
    GPIO_setOutputLowOnPin(DRV_PORT, DRV_PIN_nSLP);

    //H1 P7.2 -> IN1; P7.3 -> IN2;
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P7,
        GPIO_PIN2 + GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    //H2 P3.0 -> IN2; P3.1 -> IN1;
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN0 + GPIO_PIN1,
        GPIO_SECONDARY_MODULE_FUNCTION
        );

    //H3 P4.6 -> IN1; P4.7 -> IN2;
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P4,
        GPIO_PIN6 + GPIO_PIN7,
        GPIO_TERNARY_MODULE_FUNCTION
        );

    //Init Counter H1
    Timer_A_initUpMode(TIMER_A0_BASE, (Timer_A_initUpModeParam*)&pwm_param);
    Timer_A_startCounter(TIMER_A0_BASE,
        TIMER_A_UP_MODE
        );

    //Init Counter H2
    Timer_A_initUpMode(TIMER_A3_BASE, (Timer_A_initUpModeParam*)&pwm_param);
    Timer_A_startCounter(TIMER_A3_BASE,
        TIMER_A_UP_MODE
        );

    //Init Counter H3
    Timer_A_initUpMode(TIMER_A1_BASE, (Timer_A_initUpModeParam*)&pwm_param);
    Timer_A_startCounter(TIMER_A1_BASE,
        TIMER_A_UP_MODE
        );

    //Initialize compare mode to generate PWM
    pwm_setDuty(DRV[H1_IN1], 0);
    pwm_setDuty(DRV[H1_IN2], 0);

    pwm_setDuty(DRV[H2_IN1], 0);
    pwm_setDuty(DRV[H2_IN2], 0);

    pwm_setDuty(DRV[H3_IN1], 0);
    pwm_setDuty(DRV[H3_IN2], 0);
}


void pwm_setDuty(DRV_t timerIndex, uint8_t dutyPercentage){
    Timer_A_initCompareModeParam initCompParam = {
        .compareRegister = timerIndex.compareRegister,
        .compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        .compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET,
        .compareValue = (PWM_PERIOD/100) * dutyPercentage
    };

    Timer_A_initCompareMode(timerIndex.baseAddress, &initCompParam);
}

void mag_states(void){
    static uint8_t pwm_value = 0;
    switch(state){
        case STATE_FIRST_MAG:
            pwm_value = pwm_value == 100 ? 0 : pwm_value + 1;

            pwm_setDuty(DRV[H1_IN2], pwm_value);
            pwm_setDuty(DRV[H2_IN2], pwm_value);
            pwm_setDuty(DRV[H3_IN2], pwm_value);

            if(pwm_value == 100) state = STATE_DEMAG_POS;
        break;

        case STATE_DEMAG_POS:
            pwm_value = pwm_value == 0 ? 100 : pwm_value - 1;

            pwm_setDuty(DRV[H1_IN2], pwm_value);
            pwm_setDuty(DRV[H2_IN2], pwm_value);
            pwm_setDuty(DRV[H3_IN2], pwm_value);

            if(pwm_value == 0) state = STATE_DEMAG_NEG;
        break;

        case STATE_DEMAG_NEG:
            pwm_value = pwm_value == 100 ? 0 : pwm_value + 1;

            pwm_setDuty(DRV[H1_IN1], pwm_value);
            pwm_setDuty(DRV[H2_IN1], pwm_value);
            pwm_setDuty(DRV[H3_IN1], pwm_value);

            if(pwm_value == 100) state = STATE_MAG_NEG;
        break;

        case STATE_MAG_NEG:
            pwm_value = pwm_value == 0 ? 100 : pwm_value - 1;

            pwm_setDuty(DRV[H1_IN1], pwm_value);
            pwm_setDuty(DRV[H2_IN1], pwm_value);
            pwm_setDuty(DRV[H3_IN1], pwm_value);

            if(pwm_value == 0) state = STATE_MAG_POS;
        break;

        case STATE_MAG_POS:
            pwm_value = pwm_value == 100 ? 0 : pwm_value + 1;

            pwm_setDuty(DRV[H1_IN2], pwm_value);
            pwm_setDuty(DRV[H2_IN2], pwm_value);
            pwm_setDuty(DRV[H3_IN2], pwm_value);

            if(pwm_value == 100) state = STATE_DEMAG_POS;
        break;
    }
    uart_tx("PWM: %d, %d\r\n", pwm_value, state);
}
