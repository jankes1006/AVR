/*
 * rs232.h
 *
 *  Created on: 9 lut 2021
 *      Author: janke
 */

#ifndef RS232_H_
#define RS232_H_
#include<avr/io.h>

//*************************************KALIBRACJA************************************************
//DO KALIBRACJI UZYWAMY OSCCAL, ALE UZYJ GO TYLKO WTEDY KIEDY JESTES PEWNY ZE WSZYSTKO INNE JEST
//OK
//***********************************************************************************************
//******************************************RS232************************************************
#define UART_BAUD 9600
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

#define UART_RX_BUF_SIZE 32 //definiujemy bufor o rozmarze 32 bajtow
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE - 1) //definiujemy maske dla bufora

#define UART_TX_BUF_SIZE 16
#define UART_TX_BUF_MASK (UART_TX_BUF_SIZE-1)

volatile char UART_RxBuf[UART_RX_BUF_SIZE]; //definujemy bufor
volatile uint8_t UART_RxHead; // indeks oznaczaj¹cy g³owe weza
volatile uint8_t UART_RxTail; //indeks oznaczajacy ogon weza

volatile char UART_TxBuf[UART_TX_BUF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;

//***********************************************************************************************
//****************************************POBIERANIE KOMEND**************************************
#define GET_CMD 1

#if GET_CMD==1
#define MAX_SIZE_CMD 20
volatile char CMD_BUFOR[MAX_SIZE_CMD];
volatile uint8_t flag_cmd; // tutaj jezeli ktos wysle enter(sprawdzasz w przerwaniu) bedzie 1 i wtedy wiesz ze masz bufor do sprawdzenia.
volatile uint8_t size_cmd;
#endif
//***********************************************************************************************
//***********************************************************************************************
void USART_Init(uint16_t baud);
char uart_getc(void);
void uart_puts(char *s);
void uart_putint(int value, int radix);

#if GET_CMD==1
uint8_t equals_cmd(char *cmd);
void take_cmd(void);
#endif

#endif /* RS232_H_ */
