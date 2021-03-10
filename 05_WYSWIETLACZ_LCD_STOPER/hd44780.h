/*
 * hd44780.h
 *
 *  Created on: 29 sty 2021
 *      Author: janke
 */

#ifndef HD44780_H_
#define HD44780_H_
//**************************************************************************************************************************************
//************************************************LCD_MODYFIKACJE_USTAWIEN**************************************************************
//**************************************************************************************************************************************

//ustawia kursor na wybranej pozycj Y,X (Y = 0-3, X=0-a)
#define USE_LCD_LOCATE 1

#define USE_LCD_STR_P 1 // wysyla string umieszony w pamieci FLASH
#define USE_LCD_STR_E 1 // wysyla string umieszczony w pamieci EEPROM

#define USE_LCD_INT 1 // wyswietla liczbe dziesietna na LCD
#define USE_LCD_HEX 1 // wyswietla liczbe szesnastkowa na LCD

#define USE_LCD_DEFCHAR 1 // wysyla zdefiniowany znak z pamieci RAM
#define USE_LCD_DEFCHAR_P 1 // wysyla zdefinowany znak z pmaieci FLASH
#define USE_LCD_DEFCHAR_E 1 // wysyla zdefiniowany znak z pamieci EEPROM

#define USE_LCD_CURSOR_ON 1 //obsluga wl/wyl kursora
#define USE_LCD_CURSOR_BLINK 1 //obsluga wl/wyl migania kursora
#define USE_LCD_CURSOR_HOME 1 //ustawia kursor na pozycji poczatkowej
//**************************************************************************************************************************************
//************************************************rozdzielczosc wyswietlacza LCD (wiersze/kolumny)**************************************
//**************************************************************************************************************************************
#define LCD_Y 2    //liczba wierszy wyswietlacza LCD
#define LCD_X 16   //liczba kolumn wyswietlacza LCD

//**************************************************************************************************************************************
//************************************************************UZYWANE RW****************************************************************
//**************************************************************************************************************************************
#define USE_RW 1   // 0 - pin RW podlaczony do GND, 1 - uzywamy portu

//**************************************************************************************************************************************
//*******************************************************KONFIGURACJA PINOW I PORTOW****************************************************
//**************************************************************************************************************************************
#define LCD_D7PORT C
#define LCD_D7 5
#define LCD_D6PORT C
#define LCD_D6 4
#define LCD_D5PORT C
#define LCD_D5 3
#define LCD_D4PORT C
#define LCD_D4 2

///*************************************************************************************************************************************
//*******************************************************KONFIGURACJA RS,RW,E***********************************************************
//**************************************************************************************************************************************
#define LCD_RSPORT B
#define LCD_RS 2
#define LCD_RWPORT B
#define LCD_RW 1
#define LCD_EPORT B
#define LCD_E 0

///*************************************************************************************************************************************
//*******************************************************ADRESY W DDRAM*****************************************************************
//**************************************************************************************************************************************
#if ( (LCD_Y == 4) && (LCD_X == 20) )
#define LCD_LINE1 0x00 //adres 1 znaku 1 wiersza
#define LCD_LINE2 0x28 //adres 1 znaku 2 wiersza
#define LCD_LINE3 0x14 //adres 1 znaku 3 wiersza
#define LCD_LINE4 0x54 //adres 1 znaku 4 wiersza
#else
#define LCD_LINE1 0x00 //adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40 //adres 1 znaku 2 wiersza
#define LCD_LINE3 0x10 //adres 1 znaku 3 wiersza
#define LCD_LINE4 0x50 //adres 1 znaku 4 wiersza
#endif

///*************************************************************************************************************************************
//***********************************************MAKRA DOSTEPU PIN/PORT/DDR*************************************************************
//**************************************************************************************************************************************
#define PORT(X) SPORT(X)
#define SPORT(X) (PORT##X)

#define PIN(X) SPIN(X)
#define SPIN(X) (PIN##X)

#define DDR(X) SDDR(X)
#define SDDR(X) (DDR##X)

///*************************************************************************************************************************************
//***********************************************MAKRA USTAWIAJACE RS/RW/E**************************************************************
//**************************************************************************************************************************************
#define SET_RS PORT(LCD_RSPORT) |= (1<<LCD_RS)
#define CLR_RS PORT(LCD_RSPORT) &= ~(1<<LCD_RS)

#define SET_RW PORT(LCD_RWPORT) |= (1<<LCD_RW)  //odczyt
#define CLR_RW PORT(LCD_RWPORT) &= ~(1<<LCD_RW) //zapis

#define SET_E PORT(LCD_EPORT) |= (1<<LCD_E)
#define CLR_E PORT(LCD_EPORT) &= ~(1<<LCD_E)

///*************************************************************************************************************************************
//***********************************************KOMENDY STERUJACE**************************************************************
//**************************************************************************************************************************************
#define LCDC_CLS					0x01
#define LCDC_HOME					0x02
#define LCDC_ENTRY					0x04
	#define LCDC_ENTRYR					0x02
	#define LCDC_ENTRYL					0
	#define LCDC_MOVE					0x01
#define LCDC_ONOFF					0x08
	#define LCDC_DISPLAYON				0x04
	#define LCDC_CURSORON				0x02
	#define LCDC_CURSOROFF				0
	#define LCDC_BLINKON				0x01
#define LCDC_SHIFT					0x10
	#define LCDC_SHIFTDISP				0x08
	#define LCDC_SHIFTR					0x04
	#define LCDC_SHIFTL					0
#define LCDC_FUNC					0x20
	#define LCDC_FUNC8B					0x10
	#define LCDC_FUNC4B					0
	#define LCDC_FUNC2L					0x08
	#define LCDC_FUNC1L					0
	#define LCDC_FUNC5x10				0x04
	#define LCDC_FUNC5x7				0
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80

///*************************************************************************************************************************************
//***********************************************UDOSTEPNIONE FUNKCJE*******************************************************************
//**************************************************************************************************************************************
//strona 221
void lcd_init(void);
void lcd_home(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_blink_on(void);
void lcd_blink_off(void);
void lcd_str(char *str);
void lcd_str_P(const char *str);
void lcd_str_E(char *str);
void lcd_int(int val);
void lcd_hex(int val);
void lcd_defchar(uint8_t nr, uint8_t *def_znak);
void lcd_defchar_P(uint8_t nr, const uint8_t *def_znak);
void lcd_defchar_E(uint8_t nr, const uint8_t *def_znak);
void lcd_locate(uint8_t y, uint8_t x);
#endif /* HD44780_H_ */
