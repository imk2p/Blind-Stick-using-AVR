#include <avr/io.h>
#include<util/delay.h>
#include<stdlib.h>

#include "sbit.h"

#define TRIG 		SBIT(PORTD,2)
#define DIR_TRIG	SBIT(DDRD,2)

#define IN 			SBIT(PIND,3)
#define DIR_IN	 	SBIT(DDRD,3)

#define BUZ 		SBIT(PORTD,4)
#define DIR_BUZ	 	SBIT(DDRD,4)

#define LDR 		SBIT(PINC,1)
#define DIR_LDR	 	SBIT(DDRC,1)

void initADC()
{
	ADMUX=(1<<REFS0);//|(1<<REFS1);
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
int readADC(int ch)
{
	ADMUX&=0xf0;
	ch&=0x07;
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	return(ADC);
}

int main(void)
{
	DDRC = 0x00;
	DDRD = 0x00;
	DIR_TRIG = 1;
	DIR_IN = 0;
	DIR_BUZ = 1;
	DIR_LDR = 0;

	BUZ = 1;
	int b=0;
	int b0=0;
	_delay_ms(2000);
	long int count = 0;
	int CTRL_c = 0;
	int adc_value = 0;
	initADC();//
	int dev_on = 0;
	while(1)
	{
		if(dev_on)
		{
			if((PINC&(0x3C))==0x00)
			{
				dev_on = 0;
				for(int i=0;i<10;i++)
				{
					BUZ = 0;
					_delay_ms(700);
					BUZ = 1;
					_delay_ms(300);
				}
			}
		}
		else
		{
			if((PINC&(0x3C))==0x20)
			{
				dev_on = 1;
			}
		}
		adc_value = readADC(0);
		if(adc_value >= 100)
		{
			for(int i=0;i<10;i++)
			{
				BUZ = 0;
				_delay_ms(300);
				BUZ = 1;
				_delay_ms(50);
			}
		}
		TRIG=1;//PORTD|=0x04;
		_delay_us(20);
		TRIG=0;//PORTD&= ~(0x04);
		while (!(IN))
		{
			b++;
			_delay_us(100);
			if(b==100)
			{
				b=0;
				break;
			}
		}
		while(IN)
		{
			_delay_us(1);
			count++;
			if(count>=50000)
			{
				count=0;
				break;
			}
		}

		if(count>1)
		{
			CTRL_c = count;
			CTRL_c/=5;
			count = 0;
			if(CTRL_c<5)
			{
				for(int i=0;i<10;i++)
				{
					BUZ = 0;
					_delay_ms(50);
					BUZ = 1;
					_delay_ms(20);
				}
			}
		}
		if(!(LDR))
		{
			for(int i=0;i<10;i++)
			{
				BUZ = 0;
				_delay_ms(150);
				BUZ = 1;
				_delay_ms(50);
			}
		}
		_delay_ms(200);
	}
}
