/*
 * LSM6DS3.c
 *
 *  Created on: 27 de jun de 2023
 *      Author: liwka
 */

#include "LSM6DS3.h"
#include "my_spi.h"

#define _X  0
#define _Y  1
#define _Z  2

static int16_t accel[3];
static int16_t gyros[3];


void LSM6DS3_init(){
    //set the gyroscope control register to work at 104 Hz, 2000 dps and in bypass mode
    LSM6DS3_writeRegister(LSM6DS3_CTRL2_G, 0x4C);

    // Set the Accelerometer control register to work at 104 Hz, 4 g,and in bypass mode and enable ODR/4
    // low pass filter (check figure9 of LSM6DS3's datasheet)
    LSM6DS3_writeRegister(LSM6DS3_CTRL1_XL, 0x4A);

    // set gyroscope power mode to high performance and bandwidth to 16 MHz
    LSM6DS3_writeRegister(LSM6DS3_CTRL7_G, 0x00);

    // Set the ODR config register to ODR/4
    LSM6DS3_writeRegister(LSM6DS3_CTRL8_XL, 0x09);

}

uint8_t LSM6DS3_readRegister(uint8_t address){
    spi_tx((1<<7) | address);   // Read bit
    spi_tx(0x00);
    return spi_manager();
}

void LSM6DS3_writeRegister(uint8_t address, uint8_t data){
    spi_tx(address);   // Write bit
    spi_tx(data);
}

uint8_t LSM6DS3_accelerationAvailable(void)
{
  if (LSM6DS3_readRegister(LSM6DS3_STATUS_REG) & 0x01) {
    return 1;
  }

  return 0;
}

void LSM6DS3_readAcceleration(int16_t* ax, int16_t* ay, int16_t* az){
    uint8_t lsbyte = 0, msbyte = 0;

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTX_L_XL);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTX_H_XL);
    *ax = (int16_t)(msbyte << 8) | (lsbyte);

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTY_L_XL);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTY_H_XL);
    *ay = (int16_t)(msbyte << 8) | (lsbyte);

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTZ_L_XL);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTZ_H_XL);
    *az = (int16_t)(msbyte << 8) | (lsbyte);

//    *ax = x * 4.0 / 32768.0;
//    *ay = y * 4.0 / 32768.0;
//    *az = z * 4.0 / 32768.0;
}

uint8_t LSM6DS3_gyroscopeAvailable(void)
{
  if (LSM6DS3_readRegister(LSM6DS3_STATUS_REG) & 0x02) {
    return 1;
  }

  return 0;
}

void LSM6DS3_readGyroscope(int16_t* gx, int16_t* gy, int16_t* gz){
    uint8_t lsbyte = 0, msbyte = 0;

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTX_L_G);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTX_H_G);
    *gx = (int16_t)(msbyte << 8) | (lsbyte);

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTY_L_G);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTY_H_G);
    *gy = (int16_t)(msbyte << 8) | (lsbyte);

    lsbyte = LSM6DS3_readRegister(LSM6DS3_OUTZ_L_G);
    msbyte = LSM6DS3_readRegister(LSM6DS3_OUTZ_H_G);
    *gz = (int16_t)(msbyte) | (lsbyte);

//    *gx = x * 2000.0 / 32768.0;
//    *gy = y * 2000.0 / 32768.0;
//    *gz = z * 2000.0 / 32768.0;
}

void LSM6DS3_update(){
    if(LSM6DS3_accelerationAvailable()){
        LSM6DS3_readAcceleration(&accel[_X], &accel[_Y], &accel[_Z]);
    }

    if(LSM6DS3_gyroscopeAvailable()){
        LSM6DS3_readGyroscope(&gyros[_X], &gyros[_Y], &gyros[_Z]);
    }
}

int16_t LSM6DS3_getAccelData(uint8_t idx){
    return accel[idx];
}

int16_t LSM6DS3_getGyrosData(uint8_t idx){
    return gyros[idx];
}
