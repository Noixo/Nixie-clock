/*
 * display.h
 *
 * Created: 28/07/2019 2:00:29 PM
 *  Author: matt
 */ 

/*
0: 1
1: 3
2: 7
3: blank	011 1
4: 9
5: blank	010 1
6: 5
7: blank	011 1
8: 0
9: 2
10: 6
11: blank	101 1
12: 8
13: blank	110 1
14: 4
15: blank	111 1
*/

/*

	0 fuzz
	1 k
	2 blank
	3 u
	4 *C
	5 M
	6 %
	7 p
	8 blank
	9 blank
	10 blank
	11 blank
	12 blank
	13 fuzz
	14 blank
	15 blank
	
	missing m & n
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_




// Define function names for pins
#define LAMPAMPM 0x20
#define LAMPHH 0x40
#define LAMPMM 0x80

#define TUBEOFF 0x0F

#define PORTDPRESERVE 0xFC

#define DOTPOINTHIGH PORTD.OUTSET = PIN4_bm
#define DOTPOINTLOW PORTD.OUTCLR = PIN4_bm

//PORTA.OUT = (reMapBCD[i] << 4) | reMapBCD[i];	//3 & 4

#define FIRSTTUBE 0x0F	//PORTC
#define SECONDTUBE 0x0F	//PORTB
#define THIRDTUBE 0xF0	//PORTA		//PORTA.OUT = reMapBCD[i] << 4
#define FOURTHTUBE 0x0F //PORTA	//PORTA.OUT |= reMapBCD[i];

//use fifthTube();
#define FIFTHTUBESHIFT 0x07
#define FIFTHTUBEIO 0x04	//PORTD


#define SIXTHTUBEC 0xC0	//PORTC
#define SIXTHTUBED 0x03	//PORTD

//usecharTube() instead
#define CHARTUBESHIFT 0x18
#define CHARTUBEIO	0x030//0x04	//PORTC

#define RTCMINUTE 1
#define RTCHOUR 2

#define MAPNUM 10

extern void checkRefesh(unsigned char *time);
extern void fifthTube(unsigned char byte);
extern void characterTube(unsigned char byte);
extern void clearDisplay();
extern void initDisplay();
extern void display(unsigned char *number);
extern void convertTime(unsigned char *number);
extern void cathodeRefresh();

#endif