/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
 *
 * Modfications Copyright (C) 2006 Michał:
 *   - Prepared to compile with atmega328p
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.6 2006/09/17 16:45:53 wolti Exp $
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#define BAUD 9600
#include <util/setbaud.h>

//#define UART_UCSRB  UCSR0B

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
#ifdef RTS_ENABLE
	UCSR1B |= _BV( TXEN1 ) | _BV(TXCIE1);
#else
	UCSR1B |= _BV(TXEN1);
#endif

	if (xRxEnable) {
		UCSR1B |= _BV( RXEN1 ) | _BV(RXCIE1);
	} else {
		UCSR1B &= ~( _BV( RXEN1 ) | _BV(RXCIE1));
	}

	if (xTxEnable) {
		UCSR1B |= _BV( TXEN1 ) | _BV(UDRIE1);
#ifdef RTS_ENABLE
		RTS_HIGH;
#endif
	} else {
		UCSR1B &= ~(_BV(UDRIE1));
	}
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {
	UCHAR ucUCSRC = 0;

	/* prevent compiler warning. */
	(void) ucPORT;

	// set computed value by util/setbaud.h
	UBRR1H = UBRRH_VALUE;
	UBRR1L = UBRRL_VALUE;

#if USE_2X
	UCSR1A |= (1<<U2X1);
#else
	UCSR1A &= ~(1 << U2X1);
#endif

	switch (eParity) {
	case MB_PAR_EVEN:
		ucUCSRC |= _BV(UPM11);
		break;
	case MB_PAR_ODD:
		ucUCSRC |= _BV( UPM11 ) | _BV(UPM10);
		break;
	case MB_PAR_NONE:
		break;
	}

	switch (ucDataBits) {
	case 8:
		ucUCSRC |= _BV( UCSZ10 ) | _BV(UCSZ11);
		break;
	case 7:
		ucUCSRC |= _BV(UCSZ11);
		break;
	}

	UCSR1C |= ucUCSRC;

	vMBPortSerialEnable( FALSE, FALSE);

#ifdef RTS_ENABLE
	RTS_INIT;
#endif
	return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte) {
	UDR1 = ucByte;
	return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte) {
	*pucByte = UDR1;
	return TRUE;
}

//ISR( UART_RX_vect )
/**
 * interrupt generated when output buffer is empty
 * USART, UDR Empty Handle
 */
ISR(USART1_UDRE_vect) {
	pxMBFrameCBTransmitterEmpty();
}

//ISR( UART_UDRE_vect )
/**
 * interrupt generated after byte received.
 * received data stored in UDR
 * USART, RX Complete Handler
 */
ISR(USART1_RX_vect) {
	pxMBFrameCBByteReceived();
}

#ifdef RTS_ENABLE
SIGNAL( SIG_UART_TRANS )
{
	RTS_LOW;
}
#endif

