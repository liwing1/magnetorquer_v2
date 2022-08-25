#include <driverlib.h>
#include "my_pwm.h"


void init_pwm(void)
{
    Timer_A_initContinuousModeParam initContParam = {0};

    initContParam.clockSource =                 TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider =          TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParam.timerInterruptEnable_TAIE =   TIMER_A_TAIE_INTERRUPT_ENABLE;
    initContParam.timerClear =                  TIMER_A_DO_CLEAR;
    initContParam.startTimer =                  false;


    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_ISR(void)
{
    switch(__even_in_range(TA1IV, TA1IV_TAIFG)){
    case TA1IV_NONE: break;
    case TA1IV_TACCR1:
        __no_operation();
        break;
    case TA1IV_TACCR2:
        __no_operation();
        break;
    case TA1IV_3: break;
    case TA1IV_4: break;
    case TA1IV_5: break;
    case TA1IV_6: break;
    case TA1IV_TAIFG:
        GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
        break;
    default: _never_executed();
    }
}
