/*
 * main.c
 *
 *  Created on: 6 lut 2021
 *      Author: janke
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hd44780.h"


#define LED (1<<PA0)
#define LED_PORT PORTA
#define LED_TOG LED_PORT ^= LED

#define KEY (1<<PA1)
#define KEY_PIN PINA
#define KEY_PORT PORTA

volatile uint8_t sekundy=0;
volatile uint8_t minuty=0;
volatile uint8_t godziny=0;
volatile uint8_t zmiana=0;

ISR(TIMER1_COMPB_vect){
	LED_TOG;

	sekundy++;
	if(sekundy>=60){
		sekundy=0;
		minuty++;

		if(minuty>=60){
			minuty=0;
			godziny++;
		}
	}


	zmiana++;
}

void wyswietlGodzine(uint8_t godziny, uint8_t minuty, uint8_t sekundy){
	lcd_cls();

	if(godziny<10){
		lcd_int(0);
	}
	lcd_int(godziny);
	lcd_str(":");

	if(minuty<10){
		lcd_int(0);
	}
	lcd_int(minuty);
	lcd_str(":");

	if(sekundy<10){
		lcd_int(0);
	}
	lcd_int(sekundy);
}

uint8_t wcisnietyPrzycisk(void){
	if(!(KEY_PIN & KEY)){
		_delay_ms(200);
		return 1;
	}
	return 0;//nie wcisnieto
}

int main(void){

	DDRA |= LED;
	DDRA &= ~KEY;
	KEY_PORT |= KEY; //podciagniecie VCC do KEY

	lcd_init();

	//TRYB CTC WGM13=0, WGM12=1, WGM11=0, WGM10=0
	TCCR1B |= (1<<WGM12);
	//8000/1024 - 1 = 7812 to musisz dac aby miec 1HZ
	TCCR1B |= (1<<CS12);
	OCR1A = 31249;
	TIMSK |= (1<<OCIE1B);
	sei();

	while(1){
		if(zmiana!=0){
			wyswietlGodzine(godziny,minuty,sekundy);
			zmiana=0;
		}


		if(wcisnietyPrzycisk()==1){
			if(TIMSK & (1<<OCIE1B))
			{
				TIMSK &= ~(1<<OCIE1B);
			}else{
				TIMSK |= (1<<OCIE1B);
				sekundy=0;
				minuty=0;
				godziny=0;
				//TCNT1=0;
			}
		}

	}
}
