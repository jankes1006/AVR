/*
 * game.h
 *
 *  Created on: 24 mar 2021
 *      Author: janke
 */

#ifndef GAME_H_
#define GAME_H_

#define TIME_LED 250
#define TIME_START 200
#define SEED 10
extern uint8_t randomTab[100];
extern uint8_t howManyNumbers;


void startGame(void);
void drawNumbers(void);

#endif /* GAME_H_ */
