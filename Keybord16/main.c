/*
 * main.c
 *
 *  Created on: 14 mar 2021
 *      Author: janke
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DDR_COL DDRC
#define PORT_COL PORTC
#define PIN_COL PINC
#define COL1 (1<<PC0)
#define COL2 (1<<PC1)
#define COL3 (1<<PC2)
#define COL4 (1<<PC3)

#define DDR_ROW DDRB
#define PORT_ROW PORTB
#define ROW1 (1<<PB0)
#define ROW2 (1<<PB1)
#define ROW3 (1<<PB2)
#define ROW4 (1<<PB3)

#define LED (1<<PC4)
#define LED_DDR DDRC
#define LED_PORT PORTC
#define LED_TOG LED_PORT ^= LED

uint8_t whichKey();
volatile uint8_t whichRow = 0;

int main(void){
	LED_DDR |= LED;

	DDR_ROW |= (ROW1 | ROW2 | ROW3 | ROW4); // Ustawienie na wyjscie (bedziesz mulipleksowal)
	PORT_ROW |= (ROW1 | ROW2 | ROW3 | ROW4);

	DDR_COL &= ~(COL1 | COL2 | COL3 | COL4); // ustawienie na wejscie rzedow
	PORT_COL |= (COL1 | COL2 | COL3 | COL4); // podciagniecie pod VCC

	TCCR0 |= (1<<CS00) | (1<<CS02); // preskaler na 1024
	TIMSK |= (1<<TOIE0);

	sei();

	LED_PORT |= LED;
	while(1){
		if(whichKey()==13)
		{
			LED_TOG;
			_delay_ms(1000);
		}
	}
}

ISR(TIMER0_OVF_vect){
	whichRow++;
	if(whichRow>3){
		whichRow=0;
	}

	PORT_ROW = (PORT_ROW & 0xF0) | (0x0F & (~(1<<(whichRow))));
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
