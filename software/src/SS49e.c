/*
 * SS49e.c
 *
 *  Created on: 6 de mai de 2023
 *      Author: liwka
 */


#include "SS49e.h"
#include "my_uart.h"

#define N_SAMPLES_CALIB 50
#define N_SAMPLES_FIELD 10

void SS49e_init(SS49e_t* SS49e_obj){
    uint32_t meanVoltage = 0;

    // Average of Voltage w/o Field applied
    uint8_t i;
    for(i = 0; i < N_SAMPLES_CALIB; i++){
        meanVoltage += adc_read_mag(SS49e_obj->magIdx);
    }
    meanVoltage /= N_SAMPLES_CALIB;
    meanVoltage = (meanVoltage*3300)/0x0FFF;

    SS49e_obj->calibrationVoltage = meanVoltage;
    uart_tx("##CALIBRATION_VOLTAGE: %d\r\n", SS49e_obj->calibrationVoltage);
}

float SS49e_getField(SS49e_t* SS49e_obj){
    uint32_t meanVoltage = 0;

    uint8_t i;
    for(i = 0; i < N_SAMPLES_FIELD; i++){
        meanVoltage += adc_read_mag(SS49e_obj->magIdx);
    }
    meanVoltage /= N_SAMPLES_FIELD;
    uart_tx("%d\r\n", meanVoltage);
    meanVoltage = (meanVoltage*3300)/0x0FFF;

    int actualVoltage = meanVoltage - SS49e_obj->calibrationVoltage;

    SS49e_obj->currentField = (actualVoltage)/SS49e_obj->sensitivity;
    return SS49e_obj->currentField;
}

