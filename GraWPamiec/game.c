/*
 * game.c
 *
 *  Created on: 24 mar 2021
 *      Author: janke
 */
#include<avr/io.h>
#include<util/delay.h>
#include"matrycaLed.h"
#include "game.h"

uint8_t randomTab[100];
uint8_t howManyNumbers=2;


void startGame(void){
	//srand(SEED);
	for(int i=1; i<=16; i++){
		matrixLED_ON(i);
		_delay_ms(TIME_START);
	}
	matrixOff();
}

void drawNumbers(void){
	howManyNumbers++;

	for(int i=0; i<howManyNumbers; i++){
		_delay_ms(TIME_LED);
		randomTab[i] = (rand()%16) + 1;
		matrixLED_ON(randomTab[i]);
		_delay_ms(TIME_LED);
		matrixOff();
	}
}
