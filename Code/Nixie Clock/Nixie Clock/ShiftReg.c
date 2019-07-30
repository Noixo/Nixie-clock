/*
 * ShiftReg.c
 *
 * Created: 22/07/2019 3:45:50 PM
 *  Author: Matt
 */ 

#include "ShiftReg.h"

#include <avr/io.h>
#include <util/delay.h>

void initShiftReg()
{
	PORTD.DIRSET = SHIFTCLK;	//set SHIFTCLK pin to output
	PORTE.DIRSET = LATCH | SHIFTDATA;	//set SHIFTCLK pin to output
	sendData(0xFF, 1);	// ensure shift register is in known state
}

/* Function:	sendData

	Write a byte of information to the 595 shift register.
	
	UCHAR byte:	The data to send to the shift register. Big-endian.
	UCHAR bit:	
			0:	OR's the byte you send with what's currently in the shift
				register.
			1:	Sets the set bits from byte to 0 in the 
				shift register.
				
	E.G.	sendData(0x01, 0) turns ON the 8th bit of the shift register
			sendData(0x01, 1) clears the 8th bit in the shift register
*/
void sendData(unsigned char byte, unsigned char bit)
{
	unsigned char i;
	static unsigned char last_command;	// keeps track of set bits in shift register
	
	if(bit)
	{
		/* invert byte then AND with the last_command, preserving contents of 
		last_command so only set pins become cleared*/
		last_command &= ~byte;
		byte = last_command;	//byte is destructive, so copy
	}
	else
	{
		//copy byte data into last_command
		last_command |= byte;
	
		//copy last_commands data back into byte
		byte |= last_command;
	}
	
	// send byte to shift register, Big-endian
	for (i = 0; i < 8; i++)
	{
		if((byte & 0x01))	//read first bit
			PORTE.OUTSET = SHIFTDATA;	//Set DATA pin to HIGH
			
		PORTD.OUTSET = SHIFTCLK;	//set CLK pin to HIGH
		_delay_us(10);
		
		PORTE.OUTCLR = SHIFTDATA;	//Set DATA pin to LOW
		PORTD.OUTCLR = SHIFTCLK;	//set CLK pin to LOW
		_delay_us(10);
		
		byte >>= 1;	// shift all bits down, removing first bit
	}
	// tell shift register to activate
	PORTE.OUTSET = LATCH;
	_delay_us(10);
	PORTE.OUTCLR = LATCH;
}