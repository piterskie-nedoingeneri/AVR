#ifndef UART_H_
#define UART_H_

void UART_init(const uint8_t speed)
{
	UBRR0H = speed >> 8;
	UBRR0L = speed;
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0A |= (1 << U2X0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit_char(const unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_transmit_string(const unsigned char *str)
{
	for(int i=0; str[i] != '\0'; i++)
	{
		UART_transmit_char(str[i]);
	}
}

unsigned int num_count(long int num)
{
	unsigned int count = 0;
	while(num != 0)
	{
		count++;
		num /= 10;
	}
	return count;
}

void UART_transmit_int(long int data_in)
{
	if(data_in < 0)
		UART_transmit_char('-');
	
	data_in = labs(data_in);
	long int data_in_buff = data_in;
	long int count = 1;
	
	while(data_in_buff >= 10)
	{
		count *= 10;
		data_in_buff /= 10;
	}
	
	for(long int i=1; i <= count; i *= 10)
	{
		UART_transmit_char(((unsigned char)(data_in / (count / i))) + 0x30);
		data_in %= count / i;
	}
}

#endif /* UART_H_ */
