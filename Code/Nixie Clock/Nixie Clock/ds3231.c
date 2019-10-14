#include <avr/io.h>

#include "ds3231.h"
#include "i2c.h"

void rtc_init()
{
	PORTD.DIRCLR = PULSE;	//set to input
	PORTD.PIN5CTRL = PORT_OPC_PULLUP_gc;
}

/*
unsigned char decToBcd(unsigned char convert)
{
	return ((convert / 10 * 16) + (convert % 10));
}
*/

unsigned char bcdToDec(unsigned char convert)
{
	return ((convert / 16 * 10) + (convert % 16));
}

unsigned char* rtc_get_time()
{
	static unsigned char time[8];
	char i;
	
	i2c_start();
	(void) i2c_device_id(ds3231,0);	//send address + write bit
	
	i2c_write(0);	//point to location i of DS3231
	
	i2c_start();
	
	(void) i2c_device_id(ds3231, 1);	//send address + read bit
	
	for(i = 0; i < 7; i++)
	{
			time[i] = bcdToDec(i2c_read(0));
	}
	time[i] = bcdToDec(i2c_read(1));	//set to not ack when 6th loop
	
	i2c_stop();
	return time;
}

void rtc_ctrl(unsigned char mask)
{	
	i2c_start();
	
	i2c_device_id(ds3231, 0);
	
	i2c_write(CONTROL);
	
	//i2c_start();
	//i2c_device_id(ds3231, 1);
	i2c_write(mask);
	
	i2c_stop();
}

void rtc_set_time(unsigned char* setTime)
{
	char i;
	//multi-byte set time
	
	i2c_start();
	(void) i2c_device_id(ds3231, 0);	//1
	i2c_write(0);	//point to seconds
	
	//i2c_start();
	
	//7 is where the time registers end
	for(i = 0; i < 7; i++)//setTime[i] != '\0'; i++)
	{
		i2c_write(setTime[i]);
	}
	i2c_stop();
}

//convert time from base 10 to DS3231 format and send it to rtc_set_time
void rtc_convert_time(unsigned char *time)
{
	unsigned char rtcDecode[14] = {0};
	//input protection
	*(time) %= 60;	//seconds
	*(time + 1) %= 60;	//minutes
	*(time + 2) %= 24;	//hours
	
	*(time + 3) %= 7;	//week days
	*(time + 4) %= 31;	//day
	*(time + 5) %= 12;	//month
	*(time + 6) %= 99;	//year
	
	rtcDecode[0] = (*(time) / 10) << 4;	//grab 10's value of a number and shifts it up for DS3231
	rtcDecode[0] |= (*(time) % 10);	//grab 1's value of a number
	
	rtcDecode[1] = (*(time + 1) / 10) << 4;	//grab 10's value of a number and shifts it up for DS3231
	rtcDecode[1] |= (*(time + 1) % 10);	//grab 1's value of a number
	
	rtcDecode[2] = (*(time + 2) / 10) << 3;
	rtcDecode[2] |= (*(time + 2) % 10);
	
	//*(time + 3) = *(time + 3);
	
	rtcDecode[4] = (*(time + 4) / 10) << 4;	//grab 10's value of a number and shifts it up for DS3231
	rtcDecode[4] |= (*(time + 4) % 10);	//grab 1's value of a number
	
	
	//FIX MONTH AND YEAR
	rtcDecode[5] = (*(time + 5) / 10) << 4;	//grab 10's value of a number and shifts it up for DS3231
	rtcDecode[5] |= (*(time + 5) % 10);	//grab 1's value of a number
	
	rtcDecode[6] = (*(time + 6) / 10) << 4;	//grab 10's value of a number and shifts it up for DS3231
	rtcDecode[6] |= (*(time + 6) % 10);	//grab 1's value of a number
	
	rtc_set_time(rtcDecode);
}

void rtc_daylight_savings(unsigned char *time)
{
	//SYDNEY
	//if month is october
	if(*(time + 5) == 10)
	{
		//if first sunday
		if(*(time + 4))
		{
			if(*(time + 3) == 7)	//7 is sunday
			{
				
			}
		}
	}
}

//void rtc_convert_