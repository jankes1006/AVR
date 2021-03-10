/*
 * d_led.h
 *
 *  Created on: 28 sty 2021
 *      Author: janke
 */
#ifndef _d_led_h //derektywa zabezpieczajaca przed wielokrotnym dolaczaniem
#define _d_led_h //tego samego pliku naglowkowego, jesli bedzie dolaczany
				// w wielu roznych plikach programu
				//definicje portow i pinow wyswietlacza dla
				//ulatwienia w programie jako stale preprocesora
#define LED_DATA PORTB //port z podlaczonymi segmentami
#define LED_DATA_DIR DDRB
#define ANODY_PORT PORTA
#define ANODY_DIR DDRA

#define CA1 (1<<PA0) //CA1 oznacza bit nr 0 portu A
#define CA2 (1<<PA1) //CA2 oznacza bit nr 1 portu A
#define CA3 (1<<PA2) //CA3 oznacza bit nr 2 portu A
#define CA4 (1<<PA3) //CA4 oznacza bit nr 3 portu A

#define SEG_A (1<<0)
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)
#define SEG_DP (1<<7)
//same deklaracje zmiennych globalnych na potrzeby wykorzystania ich
//w innych plikach projektu
//przydomek extern powoduje ze te zmienne beda dostepne we
//wszystkich modulach, ktore dolacza plik d_led.h za pomoca
//derektywy include

extern volatile uint8_t cy1;
extern volatile uint8_t cy2;
extern volatile uint8_t cy3;
extern volatile uint8_t cy4;

//obszar deklaracji funkcji udostêpnionych dla innych modu³ów
void d_led_init(void);
#endif
