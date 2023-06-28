/*
 * LSM6DS3.h
 *
 *  Created on: 27 de jun de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_LSM6DS3_H_
#define SRC_INC_LSM6DS3_H_

#include <stdint.h>

#define LSM6DS3_ADDRESS            0x6A

#define LSM6DS3_WHO_AM_I_REG       0X0F
#define LSM6DS3_CTRL1_XL           0X10
#define LSM6DS3_CTRL2_G            0X11

#define LSM6DS3_STATUS_REG         0X1E

#define LSM6DS3_CTRL6_C            0X15
#define LSM6DS3_CTRL7_G            0X16
#define LSM6DS3_CTRL8_XL           0X17

#define LSM6DS3_OUTX_L_G           0X22
#define LSM6DS3_OUTX_H_G           0X23
#define LSM6DS3_OUTY_L_G           0X24
#define LSM6DS3_OUTY_H_G           0X25
#define LSM6DS3_OUTZ_L_G           0X26
#define LSM6DS3_OUTZ_H_G           0X27

#define LSM6DS3_OUTX_L_XL          0X28
#define LSM6DS3_OUTX_H_XL          0X29
#define LSM6DS3_OUTY_L_XL          0X2A
#define LSM6DS3_OUTY_H_XL          0X2B
#define LSM6DS3_OUTZ_L_XL          0X2C
#define LSM6DS3_OUTZ_H_XL          0X2D

void LSM6DS3_init(void);
uint8_t LSM6DS3_readRegister(uint8_t address);
void LSM6DS3_writeRegister(uint8_t address, uint8_t data);
void LSM6DS3_update(void);
int16_t LSM6DS3_getAccelData(uint8_t idx);
int16_t LSM6DS3_getGyrosData(uint8_t idx);


#endif /* SRC_INC_LSM6DS3_H_ */
