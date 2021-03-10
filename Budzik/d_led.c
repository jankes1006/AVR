/*
 * d_led.c
 *
 *  Created on: 28 sty 2021
 *      Author: janke
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "d_led.h" //dolaczenie pliku naglowkowego w ktorym znajduja sie potrzebne m.in. definicje preprocka
#include <avr/pgmspace.h>

//definicja zmiennych globalnych przechowujacych cyfry do wyswietlania
//volatile bo beda wykorzystywane do odczytu i zapisu
//w przerwaniu jak i w programie glownym
volatile uint8_t cy1;
volatile uint8_t cy2;
volatile uint8_t cy3;
volatile uint8_t cy4;

//tablica znakow
const uint8_t cyfry[15] PROGMEM= {
		~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F), //0
		~(SEG_B | SEG_C),                                 //1
		~(SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),         //2
		~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),         //3
		~(SEG_B | SEG_C | SEG_F | SEG_G),                 //4
		~(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),         //5
		~(SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), //6
		~(SEG_A | SEG_B | SEG_C | SEG_F),                 //7
		~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), //8
		~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),
};
//definicja funkcji inicjalizujacej prace z wyswietlaczem multipleksera
void d_led_init(void){
	LED_DATA_DIR = 0xFF; //wszystkie piny portu C jako wyjscia
	LED_DATA = 0xFF; //wygaszenie wszystkich katod - stan wysok
	//4 piny portu A jako WYJSCIA
	ANODY_DIR |= CA1 | CA2 | CA3 | CA4;
	//wygaszenie wszystkich wyswietlaczy
	ANODY_PORT |= CA1 | CA2 | CA3 | CA4;

	//ustawienie TIMER0
	TCCR0 |= (1<<WGM01); //ustawienie trybu CTC
	//OCR0 = 8000000/1024(preskaler)/200(To co chcesz dostac) - 1 = 38,0625
	TCCR0 |= (1<<CS02) | (1<<CS00); //preskaler na 1024
	OCR0 = 38; // dodatkowy podzial przez 38
	TIMSK |= (1<<OCIE0); // zezwolenie na Compare Match
}

// PROCEDURA PRZERWANIA COMPARE MATCH
ISR(TIMER0_COMP_vect){
	//zmienna do przelaczania kolejno anod wyswietlacza
	static uint8_t licznik=1;
	//cykliczne przelaczanie w kazdym przerwaniu
	//anod kazdego wyswietlacza (wartosc jest negowana)
	//bo zalaczasz zerem
	ANODY_PORT = ~licznik;

	//gdy zapalony wysw 1 podaj stan zmiennej c1
	if(licznik==1) LED_DATA = pgm_read_byte(&cyfry[cy1]);
	else if(licznik==2) LED_DATA=pgm_read_byte(&cyfry[cy2]) & (~SEG_DP); //dolozenie kropki po godzinach
	else if(licznik==4) LED_DATA=pgm_read_byte(&cyfry[cy3]);
	else if(licznik==8) LED_DATA=pgm_read_byte(&cyfry[cy4]);

	licznik <<= 1; //przesuniecie o 1 w lewo
	if(licznik==16) licznik=1;

}
