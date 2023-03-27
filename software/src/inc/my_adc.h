/*
 * my_adc.h
 *
 *  Created on: 3 de mar de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_ADC_H_
#define SRC_INC_MY_ADC_H_

typedef enum{
    MAG_IDX_1,
    MAG_IDX_2,
    MAG_IDX_3,
    MAG_IDX_MAX
} mag_idx_t;

void adc_init(void);
void adc_manager(void);
uint8_t adc_read_mag(mag_idx_t mag_idx);


#endif /* SRC_INC_MY_ADC_H_ */
