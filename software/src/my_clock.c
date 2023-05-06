/*
 * my_clock.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include "driverlib.h"
#include "my_clock.h"


//#define _EXT_OSC_

// TIMERfreq = SMCLK / TIMER_PERIOD -> 16MHz/16000 = 1kHz
#define TIMER_PERIOD    16000

static const Timer_B_initUpModeParam timer_param = {
    .clockSource = TIMER_B_CLOCKSOURCE_SMCLK,
    .clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1,
    .timerPeriod = TIMER_PERIOD,
    .timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE,
    .captureCompareInterruptEnable_CCR0_CCIE = TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE,
    .timerClear = TIMER_B_DO_CLEAR,
    .startTimer = false
};

uint16_t timer_flag = 0;
uint8_t timer_flag_50ms = 0;
uint8_t timer_flag_100ms = 0;
uint8_t timer_flag_500ms = 0;
uint8_t timer_flag_1s = 0;
uint8_t timer_flag_3s = 0;

void clock_init(void){
    //Set DCO frequency to 16 MHz
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_4);
    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Set MCLK = DCO with frequency divider of 2
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_2);

#ifdef _EXT_OSC_
    // LFXT Setup
    //Set PJ.4 and PJ.5 as Primary Module Function Input.
    /*

    * Select Port J
    * Set Pin 4, 5 to input Primary Module Function, LFXT.
    */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
    );
    //Set external clock frequency to 32.768 KHz
    CS_setExternalClockSource(32768,0);
    //Start XT1 with no time out
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);

    //Set ACLK=LFXT
    CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
#else
    //Set ACLK= DCO
    CS_initClockSignal(CS_ACLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_16);
#endif

    //Init timer to set the flags
    Timer_B_initUpMode(TIMER_B0_BASE, (Timer_B_initUpModeParam*)&timer_param);
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_UP_MODE);

}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void timer1_ISR(void){
    switch(__even_in_range(TBIV, TB0IV_TBIFG)){
    case TB0IV_NONE: break;             /* No Interrupt pending */
    case TB0IV_TBCCR1:                  /* TB0CCR1_CCIFG */
//        timer_flag = 1;
        break;
    case TB0IV_TBCCR2:                  /* TB0CCR2_CCIFG */
//        timer_flag = 1;
        break;
    case TB0IV_TBCCR3: break;           /* TB0CCR3_CCIFG */
    case TB0IV_TBCCR4: break;           /* TB0CCR4_CCIFG */
    case TB0IV_TBCCR5: break;           /* TB0CCR5_CCIFG */
    case TB0IV_TBCCR6: break;           /* TB0CCR6_CCIFG */
    case TB0IV_TBIFG:                   /* TB0IFG */
        timer_flag++;
        if(timer_flag % 50 == 0){
            timer_flag_50ms = 1;
        }
        if(timer_flag % 100 == 0){
            timer_flag_100ms = 1;
        }
        if(timer_flag % 500 == 0){
            timer_flag_500ms = 1;
        }
        if(timer_flag % 1000 == 0){
            timer_flag_1s = 1;
        }
        if(timer_flag % 3000 == 0){
            timer_flag_3s = 1;
            timer_flag = 0;
        }

        break;
    default: _never_executed();
    }
}
