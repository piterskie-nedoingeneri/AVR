/*
 * zond.c
 *
 * Created: 14.04.2019 11:31:54
 * Author : НИКОЛАЙ
 */
#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
///#include "lib_for_zond.h"
#include "mySerial.h"
#include "i2c.h"
#include "mpu6050.h"
#include "avr/interrupt.h"
char word[16];
uint8_t ind=0;
int kolvo=0;
uint8_t adc0isInit=0;
void getsample();//прототип 
void adc_init()
{
	adc0isInit=1;
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADMUX |= (1<<REFS1)|(1<<REFS0);
	ADMUX &=~(((1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX0)));//ADC0
	while((ADCSRA&(1<<ADSC)));
}

int adcRead()
{
	
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	return (int)ADC;
}
void adc3_init()
{
	adc0isInit=0;
	ADMUX=(1<<MUX1)|(1<<MUX0)|(1<<REFS0)|(1<<REFS1);//ADC3 юзаем встроенный делитель напряжения
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS2);//включаем ацп
	ADCSRA&=~(1<<ADPS0);//предделитель на 64
	while((ADCSRA&(1<<ADSC)));//ждём старта
}
void timerinit()
{
	TCCR0A|=(1<<WGM01);//сброс по совподению с OCR0A
	TCCR0B|=(1<<CS00);//делитель на 8
	//10 Кгц
	OCR0A=119;//120 иттерациий при 1.2 мгц  -> 10 кгц
	TIMSK0|=(1<<OCIE0A);//прерывание по совподению
}
void timerOFF()
{
	TIMSK0&=~(1<<OCIE0A);//отключаем прерывания по таймеру
}
void checkword()
{
	if(word[0]=='S')
	{
		kolvo=0;
		for(int i=1;i<ind;i++)
		{
			kolvo=kolvo*10 + (uint8_t)(word[i]-'0');
		}
		getsample();
	}
}
int time=0;
ISR(TIMER0_COMPA_vect)
{	
	send_byte(adcRead());
	time++;
	if(time%512==0)
	{
		print("\nSample ");
		writeint(time/512);
		send_byte('\n');
	}
	if(time>=512*kolvo)
	{
		timerOFF();//отключаем таймер
		adc_init();//переключаем ацп на фоторезистор
	}
}
ISR(USART_RX_vect)
{
	char buff = UDR0;
	point:
	if(((buff!='\r')||(buff!='\n'))||(ind<16))
	{//читаем слово
		word[ind]+=buff;
		ind++;
	}
	else
	{//начинаем читать новое слово
		checkword();
		if(ind<16) word[ind+1]='\0';
		print("\nline:");
		print(word);
		ind=0;
		if((buff!='\r')||(buff!='\n')) 
			goto point;
	}
}
void getsample()
{
	adc3_init();
	timerinit();
}
int main(void)
{
	adc_init();
	uartInit();
	I2C_init();
	mpu_init();	
	print("HELLO FROM ATMEGA328\n");
	if(mpu_check())
		print("MPU OK\n");
	else
		print("ERROR MPU!");
	sei();
	while(1)
    {
		if(adc0isInit)
		{
			print("\nPHOTOREZISTOR:");
			writeint(adcRead());
		}
		print("\nT=");
		writeint(mpu_read_t());
		print("\n y=");
		writeint(mpu_read_ay());
		_delay_ms(1000);
    }
}

