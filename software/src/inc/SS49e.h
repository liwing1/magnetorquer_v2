/*
 * SS49e.h
 *
 *  Created on: 6 de mai de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_SS49E_H_
#define SRC_INC_SS49E_H_


#include "driverlib.h"
#include "my_adc.h"

typedef struct{
    mag_idx_t magIdx;
    uint32_t currentVoltage;
    uint32_t calibrationVoltage;
    float sensitivity;
    float currentField;
} SS49e_t;

void SS49e_init(SS49e_t* SS49e_configs);
float SS49e_getField(SS49e_t* SS49e_obj);

#endif /* SRC_INC_SS49E_H_ */
