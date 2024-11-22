#include<lpc21xx.h>
#include"modhead.h"

void delay_ms(unsigned int ms){
unsigned int pclk;
int a[]={15,60,30,15,15};
pclk=a[VPBDIV]*1000;
T0PC=0;
T0PR=pclk-1;
T0TC=0;
T0TCR=1;
while(T0TC<ms);
T0TCR=0;
}

void delay(unsigned int s){
unsigned int pclk;
int a[]={15,60,30,15,15};
pclk=a[VPBDIV]*1000000;
T0PC=0;
T0PR=pclk-1;
T0TC=0;
T0TCR=1;
while(T0TC<s);
T0TCR=0;
}

void lcd_data(char data){
	unsigned int temp;
	IOCLR1=0xfe<<16;
	temp=(data&0xf0)<<16;
	
	IOSET1=temp;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(5);
	IOCLR1=1<<19;
	
	
	IOCLR1=0xfe<<16;
	temp=(data&0x0F)<<20;
	
	IOSET1=temp;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(5);
	IOCLR1=1<<19;	
	
}
	
	
void lcd_cmd(char cmd){
	unsigned int temp;
	IOCLR1=0xfe<<16;
	temp=(cmd&0xf0)<<16;
	
	IOSET1=temp;
	
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(5);
	IOCLR1=1<<19;
	
	
	IOCLR1=0xfe<<16;
	temp=(cmd&0x0F)<<20;
	IOSET1=temp;
	
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(5);
	IOCLR1=1<<19;	
	
}

void lcd_string(char *str){
	while(*str!='\0'){
		lcd_data(*str);
		str++;
	}
}

void lcd_int(int num)
{unsigned int a[10];
int i;
if(num==0)
{
	lcd_data(num+48);
return;
}
 if(num<0)
   {
   num=-num;
   //lcd_data('-');
   }
   i=0;
   while(num>0)
   {
   	  a[i]=(num%10)+48;
	   num=(num/10);
	   i++;
   }
   for(i=i-1;i>=0;i--)
   {
   lcd_data(a[i]);
   }
 }

 
void lcd_float(float f)
 {
 unsigned int num;
   if(f<0)
   {
     //uart0_tx_string("-");
	 f=-f;
   }
   else if(f==0)
   {
   lcd_string("0.000");
   }
   else if(f>0)
   {
  num=f;
  lcd_int(num);
  lcd_data('.');
  num=(f-num)*1000;
  lcd_int(num);
}
 }
 
 
 
 
 void lcd_display_time(unsigned char hr, unsigned char min, unsigned char sec) {
     // Display hour
        lcd_data((hr / 0x10) + 48); // Display tens place of the hour
        lcd_data((hr % 0x10) + 48); // Display ones place of the hour
        lcd_data(':');
        // Display minute
        lcd_data((min / 0x10) + 48); // Display tens place of the minute
        lcd_data((min % 0x10) + 48); // Display ones place of the minute
        lcd_data(':');
        // Display second
        lcd_data((sec / 0x10) + 48); // Display tens place of the second
        lcd_data((sec % 0x10) + 48); // Display ones place of the second
        lcd_data(' ');
}

 
 
 

void lcd_init(void){
	IODIR1=0XFE<<16;
	PINSEL2=0X0;
	IOCLR1=1<<19;
	
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uart
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uart0_init(unsigned int baud) {
    unsigned int pclk, result;
    int ar[5] = {15, 60, 30, 15, 15};
    pclk = ar[VPBDIV] * 1000000;
    PINSEL0 |= 5;
    result = pclk / (16 * baud);
    U0LCR = 0x83;
    U0DLL = result & 0xFF;
    U0DLM = (result >> 8) & 0xFF;
    U0LCR = 0x03;
}

void uart0_tx(unsigned char data) {
    U0THR = data;
    while (((U0LSR >> 5) & 1) == 0);
}

void uart0_tx_string(char *s) {
    while (1) {
        uart0_tx(*s);
        s++;
        if (*s == '\0')
            break;
    }
}

unsigned char uart0_rx() {
    while ((U0LSR & 1) == 0);
    return U0RBR;
}

void uart0_tx_integer(int num) {
    unsigned int a[10];
    int i;
    if (num == 0) {
        uart0_tx(num + 48);
        return;
    }
    if (num < 0) {
        num = -num;
        uart0_tx('-');
    }
    i = 0;
    while (num > 0) {
        a[i] = (num % 10) + 48;
        num = (num / 10);
        i++;
    }
    for (i = i - 1; i >= 0; i--) {
        uart0_tx(a[i]);
    }
}

void uart0_float(float f) {
    unsigned int num;
    if (f < 0) {
        uart0_tx_string("-");
        f = -f;
    } else if (f == 0) {
        uart0_tx_integer(f);
    } else if (f > 0) {
        num = f;
        uart0_tx_integer(num);
        uart0_tx('.');
        num = (f - num) * 10000000;
        uart0_tx_integer(num);
    }
}

void uart0_int_hexa(int num) {
    int x, shift = 28, i;
    uart0_tx_string("0x");
    for (i = 0; i < 8; i++) {
        x = (0x0000000F) & (num >> shift);
        if (x <= 9)
            uart0_tx_integer(x);
        else {
            x = x - 9;
            uart0_tx('A' + x - 1);
        }
        shift = shift - 4;
    }
}

void uart0_rx_string(unsigned char *p, unsigned int len) {
    unsigned int i = 0;
    while (i < len) {
        while (((U0LSR & 1) == 0));
        p[i] = U0RBR;
        if (p[i] == '\r')
            break;
        i++;
    }
    p[i] = '\0';
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//adc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void adc_init()
{
PINSEL1=0x15400000;
ADCR=0x00200400;
}

unsigned int adc_read(unsigned char ch_num)
{
unsigned int result=0;
	ADCR|=(1<<ch_num);
  ADCR|=(1<<24);	
	while(((ADDR>>31)&1)==0);
	ADCR^=(1<<24);
	ADCR^=(1<<ch_num);
	result=(ADDR>>6)&0x3FF;
	return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//spi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void spi0_init()
{
  PINSEL1|=0x1500;
	IODIR0|=(1<<7);
	IOSET0=(1<<7);
	S0SPCR=0x20;
	S0SPCCR=150;
}

unsigned char spi0(unsigned char data)
{
  S0SPDR=data;
	while(((S0SPSR>>7)&1)==0);
	return S0SPDR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i2c
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void i2c_init()
{
PINSEL0|=0x50;
I2SCLH=I2SCLL=75;
I2CONSET|=1<<6;
}



void i2c_write(unsigned char sa,unsigned char mr,unsigned char data)
{
I2CONSET|=1<<5;
I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	I2CONCLR=1<<5;
	//slave address
	I2DAT=sa;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("err :sa+w\r\n");
		goto exit;
	}
	I2DAT=mr;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("err :memory\r\n");
		goto exit;
	}
	I2DAT=data;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("err :data\r\n");
		goto exit;
	}
	exit:
	I2CONCLR=1<<3;
	I2CONCLR=1<<4;
}




unsigned char i2c_read(unsigned char sa,unsigned char mr)
{
	unsigned char temp;
  I2CONSET|=1<<5;
  I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	I2CONCLR=1<<5;
	//slave address
	I2DAT=sa;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x20)
	{
		uart0_tx_string("err :sa+w\r\n");
		goto exit;
	}
	I2DAT=mr;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x30)
	{
		uart0_tx_string("err :memory\r\n");
		goto exit;
	}
	I2CONSET|=1<<5;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	I2CONCLR=1<<5;
	I2DAT=sa|1;
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	if(I2STAT==0x48)
	{
		uart0_tx_string("err :sa+r\r\n");
		goto exit;
	}
	I2CONCLR=1<<3;
	while(((I2CONSET>>3)&1)==0);
	temp=I2DAT;
	exit:
	I2CONCLR=1<<3;
	I2CONCLR=1<<4;
	return temp;
}


unsigned int mcp3204_read(unsigned char ch_num)

{

  unsigned int result=0;

  unsigned char byteh=0,bytel=0;

  ch_num<<=6;

  IOCLR0=1<<7;

	spi0(0x06);

	byteh=spi0(ch_num);

	bytel=spi0(0x00);

	IOSET0=1<<7;

	byteh&=0x0F;

	result=(byteh<<8)|bytel;

	return result;

}
