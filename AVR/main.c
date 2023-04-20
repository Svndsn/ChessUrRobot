/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- AVR includes -------------------------------------*/
#include "avr/io.h"
#include "avr/interrupt.h"
#include <util/delay.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4


#define PWM_PIN PD5
#define DIR_PIN PC2

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/* ----------------------- Start implementation -----------------------------*/
void setup_dir() {
	// Configure PC2 as output
	
	DDRC |= (1 << DIR_PIN);
}

void set_direction(int dir) {
	if (dir) {
		// Set DIR_PIN high for forward direction
		PORTC |= (1 << DIR_PIN);
		} else {
		// Set DIR_PIN low for reverse direction
		PORTC &= ~(1 << DIR_PIN);
	}
}

void setup_pwm() {
	// Configure PD5 as output
	DDRD |= (1 << PWM_PIN);
	
	// Configure Timer 0 for Fast PWM mode
	
	
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 0xFFFF;
}



int
main( void )
{
	setup_pwm();
	setup_dir();

    //setup_pwm();
	//setup_dir();
    eMBErrorCode    eStatus;

    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, MB_PAR_NONE );
    sei(  );

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        (void) eMBPoll(  );
        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

uint16_t holding[100];

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode) {
	uint16_t i;
	if (usAddress < 1000 || usAddress + (usNRegs - 1) > 1099) return MB_ENOREG;
	usAddress -= 1000;
     
	for (i = 0; i < usNRegs; i++) {

		if (eMode == MB_REG_READ) {

			pucRegBuffer[i * 2 + 0] = (unsigned char) (holding[i + usAddress] >> 8);
			pucRegBuffer[i * 2 + 1] = (unsigned char) (holding[i + usAddress] & 0xFF);

		} else if (eMode == MB_REG_WRITE) {

			holding[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];
            
			if(holding[1]==1){
				
                grip();
                holding[1]=0;

			}else if (holding[1]==2)
			{
				ungrip();
                holding[1]=0;

			}
			

		}

	}
    
    return MB_ENOERR;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
void grip(){
	 
    // Move the motor forward for 4 seconds
	set_direction(1);
	OCR1A = 0xFFFF; //25% Hastighed
	_delay_ms(500);// Hvor lang tid den kører
	OCR1A = 0x00; //0% Hastighed
	
}

void ungrip(){
	
    set_direction(0);
	OCR1A = 0xFFFF;
	_delay_ms(500);
    OCR1A = 0x000F;

	
}

