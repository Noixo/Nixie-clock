/*
 * display.h
 *
 * Created: 28/07/2019 2:00:29 PM
 *  Author: matt
 */ 

#ifndef _DISPLAY_H_
#define _DISPLAY_H_




// Define function names for pins
#define LAMPAMPM 0x20
#define LAMPHH 0x40
#define LAMPMM 0x80

#define TUBEOFF 0x0E

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


extern void cathodeRefresh();
extern void fifthTube(unsigned char byte);
extern void characterTube(unsigned char byte);
extern void clearDisplay();
extern void initDisplay();
extern void display(unsigned char *number);
extern void convertTime(unsigned char *number);

#endif