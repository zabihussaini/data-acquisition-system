#ifndef MODHEAD_H
#define MODHEAD_H

#include <lpc21xx.h>

// Delay functions
extern void delay_ms(unsigned int ms);
extern void delay(unsigned int s);

// LCD functions
extern void lcd_data(char data);
extern void lcd_cmd(char cmd);
extern void lcd_string(char *str);
extern void lcd_int(int num);
extern void lcd_float(float f);
extern void lcd_init(void);
void lcd_display_time(unsigned char hr, unsigned char min, unsigned char sec);

// UART functions
extern void uart0_init(unsigned int baud);
extern void uart0_tx(unsigned char data);
extern void uart0_tx_string(char *s);
extern unsigned char uart0_rx(void);
extern void uart0_tx_integer(int num);
extern void uart0_float(float f);
extern void uart0_int_hexa(int num);
extern void uart0_rx_string(unsigned char *p, unsigned int len);

// ADC functions
extern void adc_init(void);
extern unsigned int adc_read(unsigned char ch_num);

// SPI functions
extern void spi0_init(void);
extern unsigned char spi0(unsigned char data);

unsigned int mcp3204_read(unsigned char ch_num);


// I2C functions
extern void i2c_init(void);
extern void i2c_write(unsigned char sa, unsigned char mr, unsigned char data);
extern unsigned char i2c_read(unsigned char sa, unsigned char mr);

#endif // MODHEAD_H
