/*
 * my_pwm.c
 *
 *  Created on: 2 de mar de 2023
 *      Author: liwka
 */


#include "driverlib.h"
#include "my_pwm.h"


// PWMfreq = SMCLK / TIMER_PERIOD -> 16MHz/1600 = 10KHz
#define TIMER_PERIOD    1600

static const Timer_A_initUpModeParam pwm_param = {
    .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
    .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
    .timerPeriod = TIMER_PERIOD,
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
    pwm_setDuty(DRV[H1_IN1], 25);
    pwm_setDuty(DRV[H1_IN2], 75);

    pwm_setDuty(DRV[H2_IN1], 12);
    pwm_setDuty(DRV[H2_IN2], 66);

    pwm_setDuty(DRV[H3_IN1], 99);
    pwm_setDuty(DRV[H3_IN2], 1);
}


void pwm_setDuty(DRV_t timerIndex, uint8_t dutyPercentage){
    Timer_A_initCompareModeParam initCompParam = {
        .compareRegister = timerIndex.compareRegister,
        .compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        .compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET,
        .compareValue = (TIMER_PERIOD/100) * dutyPercentage
    };

    Timer_A_initCompareMode(timerIndex.baseAddress, &initCompParam);
}
