/*
 * keybord16.h
 *
 *  Created on: 24 mar 2021
 *      Author: janke
 */

#ifndef KEYBORD16_H_
#define KEYBORD16_H_

#define DDR_COL DDRA
#define PORT_COL PORTA
#define PIN_COL PINA
#define COL1 (1<<PA0)
#define COL2 (1<<PA1)
#define COL3 (1<<PA2)
#define COL4 (1<<PA3)

#define DDR_ROW DDRB
#define PORT_ROW PORTB
#define ROW1 (1<<PB0)
#define ROW2 (1<<PB1)
#define ROW3 (1<<PB2)
#define ROW4 (1<<PB3)

void keybordInit();
uint8_t whichKey();

#endif /* KEYBORD16_H_ */
