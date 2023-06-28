/*
 * my_spi.h
 *
 *  Created on: 27 de fev de 2023
 *      Author: liwka
 */

#ifndef SRC_INC_MY_SPI_H_
#define SRC_INC_MY_SPI_H_

#include <stdint.h>

void spi_init(void);
void spi_tx(uint8_t TXData);
uint8_t spi_manager(void);


#endif /* SRC_INC_MY_SPI_H_ */
