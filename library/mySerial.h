/*
 * mySerial.h
 *
 * Created: 16.04.2019 18:49:49
 *  Author: НИКОЛАЙ
 */ 


#ifndef MYSERIAL_H_
#define MYSERIAL_H_
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
void uartInit()
{
	//9600 бод
	UBRR0L=108;//устнанавливаем скорость 9600
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0A |= (1 << U2X0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}
void send_byte(uint8_t byte)
{
	while(!(UCSR0A&(1<<UDRE0)));//ждем готовности к передаче
	UDR0=byte;
}
void print(char str[])
{
	uint8_t i=0;
	while(str[i]!='\0')//идём до конца строки
	{
		send_byte((uint8_t)str[i]);
		i++;
	}
}

void writeint(long int d)
{
	if(d<0) send_byte('-');
	else if(d==0) send_byte('0');
	unsigned int buff=abs(d);
	uint8_t lenin=0;
	while(buff!=0)
	{
		buff/=10;
		lenin++;
	}
	buff=abs(d);
	for(uint8_t i=1; i <= lenin; i++)
	{
		send_byte(((unsigned char)(buff / pow(10, lenin - i))) + 0x30);
		buff %= (long int)pow(10, lenin - i);
	}
}
#endif /* MYSERIAL_H_ */