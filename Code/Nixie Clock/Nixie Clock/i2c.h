#ifndef _I2C_H_
#define _I2C_H_

//Data line
#define SDA_HIGH  PORTE.OUTSET = PIN0_bm//PORTB |= (1<<PB0)
#define SDA_LOW   PORTE.OUTCLR = PIN0_bm//PORTB &= ~(1<<PB0)

//Clock line
#define SCL_HIGH  PORTE.OUTSET = PIN1_bm//PORTB |= (1<<PB2)
#define SCL_LOW   PORTE.OUTCLR = PIN1_bm//PORTB &= ~(1<<PB2)

#define TIME 1

#define READ 0x01
#define WRITE 0x00

extern void init_i2c();
extern void i2c_clock();
extern void i2c_start();
extern void i2c_stop();
extern void i2c_write(unsigned char byte);
extern char i2c_device_id(char id, char rw);
extern void i2c_read_id();
extern unsigned char i2c_read(char last_byte);

#endif