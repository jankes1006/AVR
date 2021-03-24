/*
 * matrycaLed.c
 *
 *  Created on: 23 mar 2021
 *      Author: janke
 */
#include <avr/io.h>
#include "matrycaLed.h"

void matrixInit(void){
	ROW_DDR |= (ROW1 | ROW2 | ROW3 | ROW4); //ustawienie kierunkow na wyjscia
	COL_DDR |= (COL1 | COL2 | COL3 | COL4);

	ROW_PORT |= (ROW1 | ROW2 | ROW3 | ROW4); //podciagniecie wszystkiego pod +
	COL_PORT |= (COL1 | COL2 | COL3 | COL4);
}

void matrixLED_ON(uint8_t numberLED){
	if(numberLED>=1 && numberLED<=16){
		ROW_PORT |= (ROW1 | ROW2 | ROW3 | ROW4);
		COL_PORT |= (COL1 | COL2 | COL3 | COL4);

		if(numberLED<=4) ROW_PORT &= ~(ROW1);
		else if(numberLED<=8) ROW_PORT &= ~(ROW2);
		else if(numberLED<=12) ROW_PORT &= ~(ROW3);
		else ROW_PORT &= ~(ROW4);

		if(numberLED%4 == 1) ROW_PORT &= ~(COL2 | COL3 | COL4);
		else if(numberLED%4 == 2) ROW_PORT &= ~(COL1 | COL3 | COL4);
		else if(numberLED%4 == 3) ROW_PORT &= ~(COL1 | COL2 | COL4);
		else ROW_PORT &= ~(COL1 | COL2 | COL3);
	}
}

void matrixOff(void){
	ROW_PORT |= (ROW1 | ROW2 | ROW3 | ROW4);
	COL_PORT |= (COL1 | COL2 | COL3 | COL4);
}
