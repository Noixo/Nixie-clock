/*
 * Nixie Clock.c
 *
 * Created: 19/07/2019 8:47:54 PM
 * Author : Matt
 */ 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "ShiftReg.h"
#include "i2c.h"
#include "ds3231.h"
#include "display.h"


volatile unsigned char *p_time;

//define for setting display to off
//define for setting display to 0000?

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
0 = fuzz
1 = k
2 = blank
3 = micro
4 = *C
5 = M
6 = %
7 = weird blue p?
8 = fuzz
9 = fuzz
10 = fuzz
11 = fuzz
12 = fuzz
13 = weird fuzz dot
14 = fuzz

NO N
*/
unsigned char test[8] = {0};

int main(void)
{
	
	const unsigned char time[7] = {00,0x12,0x20,1,1,1,1};
	_delay_ms(500);	//delay to let voltage stabalise
	
	initDisplay();
	
	initShiftReg();
	init_i2c();
	rtc_init();
	
	// set square wave to 1Hz, enable squarewave output
	rtc_ctrl(0x00);
	
	sendData(LAMPHH | LAMPMM, 0);

	//rtc_set_time(time);
	unsigned char i = 0;
    while (1) 
    {
		//clearDisplay();
	//	sendData((a << 3) & 0x20, 1);
		//sendData((a << 3) & ~0x20, 0);
		//PORTC_OUT = (a << 4) & 0x3F;
		//a++;
		//_delay_ms(1000);
		
		//if( a > 20000 && a < 30000)
	//	{
		//	cathodeRefresh();
	//	}
		if(PORTD.IN & PULSE)
		{
			
			characterTube(i);
			i = (i + 1) % 0x10;
			_delay_ms(400);
			//test[4] = test[4] + 1;
			//test = rtc_get_time();
			//convertTime(test);
			//display(test);

			//*test = rtc_get_time();
			
			//if(*(test + 1) == 1)	//
			
				
			//else
			//	convertTime(test);
			
			//display(test);
			
		}
    }
	
}

/*TODO

- Display time on display
- add anti-cathode poisoning
- interrupt for DS3231 pulse
- char tube
- temperature/humidity
- am/pm


- USB uart
	- change time
	- 12/24hour
	- 
-set time with USB

*/
