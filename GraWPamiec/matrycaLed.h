/*
 * matrycaLed.h
 *
 *  Created on: 23 mar 2021
 *      Author: janke
 */

#ifndef MATRYCALED_H_
#define MATRYCALED_H_

//----------------------------------------------------------------------------------------------------------
//*************************************DEFINICJE RZEDOW I KOLUMN********************************************
#define ROW_DDR DDRC
#define ROW_PORT PORTC
#define ROW_PIN PINC
#define ROW1 (1<<PC0)
#define ROW2 (1<<PC1)
#define ROW3 (1<<PC2)
#define ROW4 (1<<PC3)

#define COL_DDR DDRC
#define COL_PORT PORTC
#define COL_PIN PINC
#define COL1 (1<<PC4)
#define COL2 (1<<PC5)
#define COL3 (1<<PC6)
#define COL4 (1<<PC7)
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

void matrixInit(void);
void matrixLED_ON(uint8_t numberLED);
void matrixOff(void);

#endif /* MATRYCALED_H_ */
