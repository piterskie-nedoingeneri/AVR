#ifndef I2C_H_
#define I2C_H_

void I2C_init()
{
	TWBR = 0x20; // 10kHz
}

void I2C_start()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
}

void I2C_stop()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void I2C_send_byte(unsigned char data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
}

uint8_t I2C_read_byte()
{
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t I2C_read_byte_no_ack()
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

#endif /* I2C_H_ */