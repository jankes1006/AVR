/*
 * main.c
 *
 *  Created on: 1 mar 2021
 *      Author: janke
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "d_led.h"
#include "i2c_twi.h"
#include "rs232.h"

//definicje ledow

#define LED_CLOCK_MENU (1<<PD0)
#define LED_ALARM_MENU (1<<PD1)
#define LED_SET_MENU (1<<PD3)
#define LED_1S (1<<PD4)
#define LED_ALARM (1<<PD5)

#define LED_PORT PORTD
#define LED_DDR DDRD

//piny przyciskow
#define KEY1 (1<<PC3)
#define KEY2 (1<<PC4)
#define KEY3 (1<<PC5)
#define KEY4 (1<<PC6)
#define KEY5 (1<<PC7)
//port przyciskow
#define KEY_PORT PORTC
//ddr przyciskow
#define KEY_DDR DDRC
//pin przyciskow
#define KEY_PIN PINC

#define BUZZER (1<<PD6)
#define BUZZER_DDR DDRD
#define BUZZER_PORT PORTD
#define BUZZER_ON BUZZER_PORT |= BUZZER
#define BUZZER_OFF BUZZER_PORT &= ~BUZZER

#define DELAY_TIME 300
uint8_t bufor[4]; // przechowuje aktualnie wyswietlana godzine
volatile uint8_t counterS=0;

uint8_t bcd2dec(uint8_t bcd){
	return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
}

uint8_t dec2bcd(uint8_t dec){
	return ((dec / 10)<<4) | (dec % 10);
}

void writeClock(){
	uint8_t hours = bcd2dec(bufor[3]);
	uint8_t minutes = bcd2dec(bufor[2]);

	if(hours < 10){
		cy1 = 0;
		cy2 = hours;
	}else{
		cy1 = hours/10;
		cy2 = hours%10;
	}

	if(minutes < 10){
		cy3 = 0;
		cy4 = minutes;
	}else{
		cy3 = minutes/10;
		cy4 = minutes%10;
	}
}

uint8_t whichKey(){
	if(!(KEY_PIN & KEY1)) return KEY1;
	if(!(KEY_PIN & KEY2)) return KEY2;
	if(!(KEY_PIN & KEY3)) return KEY3;
	if(!(KEY_PIN & KEY4)) return KEY4;
	if(!(KEY_PIN & KEY5)) return KEY5;
	return 0;
}

void switchLed(uint8_t LED, uint8_t onOff){
	if(onOff==0){
		LED_PORT |= LED;
	}else{
		LED_PORT &= ~LED;
	}
}

//sluzy do wyboru tego czy mamy wyswietlane ustawianie alarmu czy tez zegar
enum menu{alarm, clock, setTime};
enum menu menuClock=clock;

enum modifyThings{hours,minutes};
enum modifyThings mod = hours;

//godzina alarmu
uint8_t hoursAlarm=10, minutesAlarm=0;
uint8_t alarmIsActive=0;

void keyAction(){
	uint8_t choice;
	choice = whichKey();

	if( alarmIsActive==2 && choice!=0){
		BUZZER_OFF;
		alarmIsActive=0;
		_delay_ms(DELAY_TIME);
		return;
	}
	switch(choice){
		case KEY1: // pierwszy klawisz sluzy do zwiekszania godzin/minut alarmu
			if(menuClock==alarm || menuClock==setTime){
				if(mod == hours){
					hoursAlarm++;
					hoursAlarm%=24;
				}
				if(mod == minutes){
					minutesAlarm++;
					minutesAlarm%=60;
				}
				_delay_ms(DELAY_TIME);
			}
		break;

		case KEY2: // drugi klawisz sluzy do zmniejszania godzin/minut alarmu
			if(menuClock==alarm || menuClock==setTime){
				if(mod == hours){
					if(hoursAlarm>0) hoursAlarm--;
					else hoursAlarm=23;
				}

				if(mod == minutes){
					if(minutesAlarm>0) minutesAlarm--;
					else minutesAlarm=59;
				}
				_delay_ms(DELAY_TIME);
			}
		break;

		case KEY3: // trzeci klawisz sluzy do przelaczania godziny/minuty przy alarmie
			if(menuClock==alarm || menuClock==setTime){
				if(mod == hours) mod = minutes;
				else mod = hours;
				_delay_ms(DELAY_TIME);
			}
		break;

		case KEY4: // czwarty klaiwsz przelacza pomiedzy zegarem/alaremem
			switch(menuClock){
				case clock:
					menuClock = alarm;
					break;

				case alarm:
					menuClock = setTime;
					break;

				case setTime:
					menuClock = clock;
			}

			_delay_ms(DELAY_TIME);
		break;

		case KEY5: // aktywuje i dezaktywuje alarm
			if(menuClock!=setTime)
			{
				if(alarmIsActive==0) alarmIsActive=1;
				else alarmIsActive=0;
				_delay_ms(DELAY_TIME);
			}else{
				bufor[0] = 0;			// setne czêœci sekundy
				bufor[1] = dec2bcd(0);	// sekundy
				bufor[2] = dec2bcd(minutesAlarm);	// minuty
				bufor[3] = dec2bcd(hoursAlarm);	// godziny
				// zapis 4 bajtów z bufora pod adres 0x01 w pamiêci RAM naszego RTC
				TWI_write_buf( 0xA2, 0x01, 4, bufor );
				_delay_ms(DELAY_TIME);
			}
		break;

		}
}

void ledSet(){
	if(alarmIsActive>0)
		switchLed(LED_ALARM,1);
	else
		switchLed(LED_ALARM,0);

	if(menuClock==clock)
		switchLed(LED_CLOCK_MENU,1);
	else
		switchLed(LED_CLOCK_MENU,0);

	if(menuClock==alarm)
		switchLed(LED_ALARM_MENU,1);
	else
		switchLed(LED_ALARM_MENU,0);

	if(menuClock==setTime)
		switchLed(LED_SET_MENU,1);
	else
		switchLed(LED_SET_MENU,0);
}

int main(void){
	KEY_DDR &= ~(KEY1 | KEY2 | KEY3 | KEY4 | KEY5);  // ustawienie przyciskow
	KEY_PORT |= (KEY1 | KEY2 | KEY3 | KEY4 | KEY5); // podciagniecie pod VCC przyciskow

	BUZZER_DDR |= BUZZER;
	BUZZER_OFF;

	LED_DDR |= LED_CLOCK_MENU | LED_ALARM_MENU | LED_SET_MENU | LED_1S | LED_ALARM;
	i2cSetBitrate(100); //inicjalizacja i2c na 100 kHz

	//przerwanie na int0
	DDRD &= ~(1<<PD2);     // ustawienie jako wejscie
	PORTD |= (1<<PD2);    // podciagniecie pod vcc
	MCUCR |= (1<<ISC01); // odpalenie na zbocze opadajace
	GICR |= (1<<INT0);  // zezwolenie na przerwanie

	//wyswietlacz inicjalizacja
	d_led_init();

	//zezwolenie na przerwania
	sei();

	TWI_read_buf( 0xA2, 0x01, 4, bufor); //pobranie godziny do bufora
	writeClock(); // zapisanie godziny

	while(1){

		keyAction();

		if(menuClock==clock){
			if(counterS!=0)
			{
				TWI_read_buf( 0xA2, 0x01, 4, bufor); //pobranie godziny do bufora
				writeClock(); // zapisanie godziny
				counterS=0;

				if(alarmIsActive==1 && dec2bcd(hoursAlarm)==bufor[3] && dec2bcd(minutesAlarm)==bufor[2]){
							alarmIsActive = 2; //oznacza ze alarm dziala
							BUZZER_ON;
				}
			}


		}else{
			bufor[3] = dec2bcd(hoursAlarm);
			bufor[2] = dec2bcd(minutesAlarm);
			writeClock();
		}

		ledSet();
	}
}

ISR( INT0_vect ) {
	counterS++;
	LED_PORT ^= LED_1S;
}
