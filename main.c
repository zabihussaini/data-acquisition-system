#include<lpc21xx.h>
#include"modhead.h"
//unsigned char ar[12];
int main()
{
	unsigned char hour,min,sec;
	unsigned int ldr,temp,pot,light;
	float voult,temperature,temper;
	uart0_init(9600);
    lcd_init();
	adc_init();
    spi0_init();
    i2c_init();
    i2c_write(0xD0,0x02,0x11);
	i2c_write(0xD0,0x01,0x59);
	i2c_write(0xD0,0x00,0x59);
	//lcd_cmd(0x01);
while(1)
{   
    hour=i2c_read(0xD0,0x2);
	min=i2c_read(0xD0,0x1);
	sec=i2c_read(0xD0,0x0);
	lcd_cmd(0x80);
	lcd_data((hour/0x10)+48);
	lcd_data((hour%0x10)+48);
	lcd_data(':');
	lcd_data((min/0x10)+48);
	lcd_data((min%0x10)+48);
	lcd_data(':');
	lcd_data((sec/0x10)+48);
	lcd_data((sec%0x10)+48);
	lcd_data(' ');
	if(((hour>>5)&1)==0)
	{
		lcd_data('A');
		lcd_data('M');
	}
	else
	{
	lcd_data('P');
	lcd_data('M');
	}

ldr=mcp3204_read(2);
light=100-((ldr*100)/4095);
lcd_cmd(0xc0);
lcd_int(light);
lcd_data(' ');
temp=adc_read(1);
temperature=(temp*3.3)/1023;
temper=(temperature-0.5)/0.01;
lcd_int(temper);
lcd_data('c');
lcd_data(' ');
////////potentio meter 
pot=adc_read(2);
voult=(pot*3.3)/1023;
lcd_float(voult);
lcd_data('v');
//lcd_string("hello")

}
}
