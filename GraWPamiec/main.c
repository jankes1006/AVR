/*
 * main.c
 *
 *  Created on: 23 mar 2021
 *      Author: janke
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "matrycaLED.h"
#include "keybord16.h"
#include "game.h"


int main(void){
	sei();
	matrixInit();
	keybordInit();
	startGame();
	_delay_ms(1000);
	drawNumbers();
	uint8_t key=10;

	uint8_t userAnswer=0;
	while(1){

		if(key=whichKey()){
			if(key==randomTab[userAnswer]){
				matrixLED_ON(key);
				userAnswer++;
				_delay_ms(200);

				if(userAnswer>=howManyNumbers){
					matrixOff();
					_delay_ms(500);
					drawNumbers();
					userAnswer=0;
				}
			}else{
				startGame();
				howManyNumbers=2;
				userAnswer=0;
				drawNumbers();
			}
		}
	}
}
