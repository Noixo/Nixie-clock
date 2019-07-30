/*
 * ShiftReg.h
 *
 * Created: 22/07/2019 3:46:02 PM
 *  Author: Matt
 */ 

#ifndef _SHIFTREG_H_
#define _SHIFTREG_H_

#define LATCH PIN2_bm

#define SHIFTCLK PIN3_bm
#define SHIFTDATA PIN3_bm
//#define SHIFTCLKHIGH PORTD.OUT = PIN3_bm;
//#define SHIFTCLKLOW PORTD.OUTCLR = PIN3_bm;

extern void initShiftReg();
extern void sendData(unsigned char byte, unsigned char bit);

#endif