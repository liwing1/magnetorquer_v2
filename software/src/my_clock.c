/*
 * my_clock.c
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#include "driverlib.h"
#include "my_clock.h"


//#define _EXT_OSC_


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
}


