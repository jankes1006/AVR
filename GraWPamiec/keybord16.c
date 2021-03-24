/*
 * keybord16.c
 *
 *  Created on: 24 mar 2021
 *      Author: janke
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include "keybord16.h"

volatile uint8_t whichRow = 0;

void keybordInit(){
	DDR_ROW |= (ROW1 | ROW2 | ROW3 | ROW4); // Ustawienie na wyjscie (bedziesz mulipleksowal)
	PORT_ROW |= (ROW1 | ROW2 | ROW3 | ROW4);

	DDR_COL &= ~(COL1 | COL2 | COL3 | COL4); // ustawienie na wejscie rzedow
	PORT_COL |= (COL1 | COL2 | COL3 | COL4); // podciagniecie pod VCC

	TCCR0 |= (1<<CS00) | (1<<CS02); // preskaler na 1024
	TIMSK |= (1<<TOIE0);
}

uint8_t whichKey(){
	uint8_t tempCol = PIN_COL;
	uint8_t tempRow = whichRow;

	if(!(tempCol & COL1)) return 1+tempRow*4;
	if(!(tempCol & COL2)) return 2+tempRow*4;
	if(!(tempCol & COL3)) return 3+tempRow*4;
	if(!(tempCol & COL4)) return 4+tempRow*4;

	return 0;
}

ISR(TIMER0_OVF_vect){
	whichRow++;
	if(whichRow>3){
		whichRow=0;
	}

	PORT_ROW = (PORT_ROW & 0xF0) | (0x0F & (~(1<<(whichRow))));
}
