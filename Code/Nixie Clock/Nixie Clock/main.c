/*
 * Nixie Clock.c
 *
 * Created: 19/07/2019 8:47:54 PM
 * Author : Matt
 */ 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "ShiftReg.h"
#include "i2c.h"
#include "ds3231.h"
#include "display.h"
#include "sht20.h"

//#include "conf_usb.h"


volatile unsigned char *p_time;

unsigned char test[8] = {0};
	
void print_float(float value)
{
	char arr[5], i;
		
		
	dtostrf(value, 4, 2, arr);
	//arr[5] = '/0';	//only 2 decimal points
	for(i = 0; i < 5; i++)
	{
		arr[i] -= 48;
	}
	display(arr);//convertTime(arr);	
	//oled_write_string(arr);
}

int main(void)
{
	
	const unsigned char time[7] = {0,42,1,1,14,10,19}; //{seconds,minutes,hour,weekday,day,month,year};
	unsigned char *test;
	_delay_ms(500);	//delay to let voltage stabalise
	
	initDisplay();
	
	initShiftReg();
	init_i2c();
	rtc_init();
	//sht_init();
	
	// set square wave to 1Hz, enable squarewave output
	rtc_ctrl(0x00);
	
	sendData(LAMPHH | LAMPMM | LAMPAMPM, 0);
	
	clearDisplay();

	//rtc_set_time(time);
	//rtc_convert_time(time);
	//unsigned char i = 0;
    while (1) 
    {	

		if(PORTD.IN & PULSE)	//check if 1 second has passed
		{
			//print_float(sht_temp());
			//print_float(sht_humidity());
			_delay_ms(100);
			
			test = rtc_get_time();
			
			//if(*(test + 2) >= 11 || *(test + 2) < 6)	//if time is > 10pm
			//{
				//clearDisplay();
		//	}
			//else
			//{
				//cathodeRefresh();
				convertTime(test);
				
				checkRefesh(test);	// flash screen every hour
			//}
			
		}
    }
	
}

/*TODO

- temperature/humidity
- am/pm
- self test (cycle between each numbers slowly in UART mainly)

- USB uart
	- change time
	- 12/24hour
	- 
-set time with USB


BUGS:
-	5th digit doesn't turn on unless a dummy write is writen on first start up (only issue if clear display run before)
*/
