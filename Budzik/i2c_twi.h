/*
 * i2c_twi.h
 *
 *  Created on: 1 mar 2021
 *      Author: janke
 */

#ifndef I2C_TWI_H
#define I2C_TWI_H

#define NACK 0
#define ACK 1

void TWI_read_buf(uint8_t SLA, uint8_t adr, uint8_t len, uint8_t *buf);
void TWI_write_buf(uint8_t SLA, uint8_t adr, uint8_t len, uint8_t *buf);
void i2cSetBitrate(uint16_t bitrateKHz);

#endif
