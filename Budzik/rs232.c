/*
 * rs232.c
 *
 *  Created on: 9 lut 2021
 *      Author: janke
 */
#include "rs232.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

void USART_Transmit(char data){
	//czekaj a¿ bufor nadawczy bedzie pusty
	while(!(UCSRA & (1<<UDRE)));
	//wrzuc dane do bufora nadawczego, start transmisji
	UDR = data;
}

unsigned char USART_Receive(void){
	//czekaj az dane zostana odebrane do bufora
	while(! (UCSRA & (1<<RXC)));
	//zwruc zawartosc bufora jako rezultat funkcji odebrany bajt
	return UDR;
}

void USART_Init(uint16_t baud )
{
	/* Set baud rate */
	UBRRH = (uint8_t)(baud>>8);
	UBRRL = (uint8_t)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	UCSRB |= (1<<RXEN) | (1<TXEN) | (1<<RXCIE);

	//pobieranie komend
#if GET_CMD==1
	flag_cmd=0;
#endif
}


char uart_getc(void){
	//sprawdzamy czy indeksy sa rowne
	if(UART_RxHead == UART_RxTail) return 0;

	//obliczamy i zapamietujemy nowy indeks ogona, moze sie zrownac z glowa
	UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
	return UART_RxBuf[UART_RxTail];
}

ISR(USART_RXC_vect){
	uint8_t tmp_head;
	char data;

	data = UDR; //pobieramy natychmiast bajt danych z bufora sprzetowago obliczamy nowy indeks glowy
	tmp_head = (UART_RxHead+1) & UART_RX_BUF_MASK; //sprawdzamy czy waz nie zacznie zjadac wlasnego ogona

	if(tmp_head == UART_RxTail){
		//tutaj ogarnij cos jak waz zacznie szamac sam siebie
	}else{
		UART_RxHead = tmp_head; // zapamietujesz indeks glowy
		UART_RxBuf[tmp_head]=data; //

	//jezeli wystapilo enter, uzytkownik wyslal komende
#if GET_CMD==1
		if(data==0x0d){
			flag_cmd=1;
		}
#endif
	}
}

void uart_putc(char data){
	uint8_t tmp_head;

	tmp_head = (UART_TxHead + 1) & UART_TX_BUF_MASK;

	while(tmp_head == UART_TxTail){} // tutaj sprawdzasz czy nie zaczniesz nadpisywac bufora

	UART_TxBuf[tmp_head] = data;
	UART_TxHead = tmp_head;

	//inicjalizujemy przerwanie wystepujace gdy bufor jest pusty, dzieki czemu
	//w dlaszej czesci wysylaniem danych zajmie sie juz procedura obslug przerwania

	UCSRB |=(1<<UDRIE);
}

ISR(USART_UDRE_vect){
	if(UART_TxHead != UART_TxTail){
		UART_TxTail = (UART_TxTail+1) & UART_TX_BUF_MASK;
		UDR = UART_TxBuf[UART_TxTail];
	}else{
		UCSRB &= ~(1<<UDRIE);
	}
}

void uart_puts(char *s){
	register char c;
	while ((c = *s++)) uart_putc(c);
}

void uart_putint(int value, int radix)	// wysy³a na port szeregowy tekst
{
	char string[17];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts(string);			// wyœlij string na port szeregowy
}

//******************************************************************WLASNE FUNKCJE*******************************************************
#if GET_CMD==1

void take_cmd(void){
	char c;
	size_cmd=0;

	while(c=uart_getc()){
		if(c!=0x0d){
		CMD_BUFOR[size_cmd]=c;
		size_cmd++;
		}
	}
	flag_cmd=0;
}

uint8_t equals_cmd(char *cmd){

	char *copy = cmd;
	uint8_t size=0;
	while(*copy++) size++;

	if(size!=size_cmd) return 0;
	for(uint8_t i=0; i<size_cmd; i++){
		if(!cmd) return 0;
		if(CMD_BUFOR[i]!=*cmd) return 0;
		cmd++;
		//USART_Transmit(*cmd++);
	}

	return 1;
}

#endif
