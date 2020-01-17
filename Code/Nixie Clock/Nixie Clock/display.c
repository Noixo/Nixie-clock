/*
 * display.c
 *
 * Created: 28/07/2019 2:00:10 PM
 *  Author: Matt
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "display.h"
#include "ShiftReg.h"
#include "ds3231.h"

// array to print 0-9 on tube
const unsigned char reMapBCD[MAPNUM] = {8,0,9,1,14,6,10,2,12,4};
const unsigned char reMapBCDTube[MAPNUM] = {'n', 'k', 'm', 'u', 'c', 'M', '%', 'p', 2, 2};
	
volatile unsigned char hour, minute;

void initDisplay()
{
	PORTC_DIR = FIRSTTUBE | SIXTHTUBEC | CHARTUBEIO;
	PORTD_DIR = SIXTHTUBED | FIFTHTUBEIO;
	PORTA_DIR = FOURTHTUBE | THIRDTUBE;
	PORTB_DIR = SECONDTUBE;
	
	PORTD_DIRSET = PIN4_bm;	// dot point
	
	characterTube(2);
}

//check hours and if minute has passed once
void checkRefesh(unsigned char *time)
{
	static unsigned char last_min, last_hour;
	
	if(*(time + RTCHOUR) != last_hour)
	{
		last_min = *(time + RTCMINUTE);
		
		while (*(rtc_get_time() + RTCMINUTE) == last_min)
		{
			cathodeRefresh();
		}
	}
	
	last_hour = *(time + RTCHOUR);
}

/* Function:	cathodeRefresh

	Flashes all digits of the nixie tube to prevent cathode poisoning.
*/
void cathodeRefresh()
{
	unsigned char val[6] = {0};
	unsigned char i, j;
	
	//flash numbers 0-9 on each digit
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 6; j++)
		{
			val[j] = i;
		}
		display(val);
		characterTube(reMapBCDTube[i]);
		_delay_ms(200);
	}
	
	//turn off all digits
	clearDisplay();
	
	// dot point in each tube
	DOTPOINTHIGH;
	characterTube('c');	//dummy write to protect displays
	_delay_ms(200);
	DOTPOINTLOW;
	//dummy write to protect displays
	display(0);
	characterTube(2);
	
	
}

void fifthTube(unsigned char byte)
{		   
	PORTD.OUTCLR = PIN2_bm;
	
	if(byte & 0x01)	//0001
		PORTD.OUTSET = PIN2_bm;
		
	byte >>= 1;
	
	// clear pins 5-7
	sendData(0x07, 1);
	
	//write data to pins 5-7
	sendData(byte,0);
}

void characterTube(unsigned char byte)
{
	unsigned char i;//, tube;

	for(i = 0; i < MAPNUM; i++)
	{
		if(byte == reMapBCDTube[i])
		{
			byte = i;
			break;
		}
	}
	
	if(byte & 0x02)
		PORTC.OUTSET = PIN5_bm;
	else
		PORTC.OUTCLR = PIN5_bm;
	
	sendData(0x18,1);
	
	if((byte & 0x04) || (byte & 0x08))
	{
		//send to 2 for 0x08, send to pin 4 for 0x04
			sendData((byte & 0x0C) << 1, 0);
	}
		
	if(byte & 0x01)	//pin 3
		PORTC.OUTSET = PIN4_bm;
	else
		PORTC.OUTCLR = PIN4_bm;
}
// SEE IF FIFTHTUBE IS STILL BUGGED (FLASHING 1)
//set displays to display nothing
void clearDisplay()
{
	PORTC.OUT = TUBEOFF | (unsigned char)(TUBEOFF << 6);	//tube 1 and 6
	
	PORTD.OUT &= PORTDPRESERVE;	//0xFC preserve all bits but first 2
	PORTD.OUT |= TUBEOFF >> 2;	//part of tube 6 KILLS TUBE 6
	
	PORTB.OUT = TUBEOFF;	// tube 2	+1
	
	PORTA.OUT = (TUBEOFF << 4) | TUBEOFF; // tube 3 and 4
	
	fifthTube(TUBEOFF);	//tube 5
	
	//character tube
	characterTube(TUBEOFF);
}

/*	Function: display
	
	Takes an UCHAR array/pointer and displays that number on screen
	
	UCHAR* number: An array of numbers to display on screen
	in format of "HHMMSS" Little-endian format
	
	unclean format to prevent flashing of cathode
*/
void display(unsigned char *number)
{
	static unsigned char last_num;
	//unsigned char test = PORTD.OUT & 0xFC;
	
	PORTC.OUT = reMapBCD[*(number + 0)] | (reMapBCD[*(number + 5)] << 6);	//tube 1 and 6
	
	PORTD.OUT &= PORTDPRESERVE;	//0xFC preserve all bits but first 2
	PORTD.OUT |= (reMapBCD[*(number + 5)] >> 2);	//part of tube 6 KILLS TUBE 6
	
	PORTB.OUT = reMapBCD[*(number + 1)];	// tube 2	+1
	
	PORTA.OUT = (reMapBCD[*(number + 2)]) << 4 | reMapBCD[*(number + 3)]; // tube 3 and 4
	
	//prevent flashing of 5th element '1'
	
	if(*(number + 4) != last_num)
		fifthTube(reMapBCD[*(number + 4)]);	//tube 5
		
	last_num = *(number + 4);
}

void convertTime(unsigned char *number)
{
	unsigned char convert[6] = {0};

	convert[4] = *(number) / 10;		//Ss
	convert[5] = *(number) % 10;		//sS
	
	convert[2] = *(number + 1) / 10;	//Mm
	convert[3] = *(number + 1) % 10;	//mM
	
	convert[0] = *(number + 2) / 10;	//Hh
	convert[1] = *(number + 2) % 10;	//hH

	//for cathodeRefresh/checkrefresh	BAD CODE
	minute = *(number + 1);
	hour = *(number + 2);
	
	display(convert);
}