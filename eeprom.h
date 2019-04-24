#ifndef EEPROM_H_
#define EEPROM_H_

void EEPROM_write_1byte(unsigned int adress, uint8_t data)
{
	while(EECR & (1 << EEPE));
	EEAR = adress;
	EEDR = data;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

uint8_t EEPROM_read_1byte(unsigned int adress)
{
	while(EECR & (1 << EEPE));
	EEAR = adress;
	EECR |= (1 << EERE);
	return EEDR;
}

void EEPROM_write_2byte(unsigned int adress, uint16_t data)
{
	EEPROM_write_1byte(adress, (uint8_t) data);
	EEPROM_write_1byte(adress + 1, (uint8_t) (data >> 8));
}

uint16_t EEPROM_read_2byte(unsigned int adress)
{
	uint16_t data = EEPROM_read_1byte(adress + 1) << 8;
	asm("nop");
	data += EEPROM_read_1byte(adress);
	return data;
}

void EEPROM_write_4byte(unsigned int adress, uint32_t data)
{
	EEPROM_write_2byte(adress, data);
	EEPROM_write_2byte(adress + 2, data >> 16);
}

uint32_t EEPROM_read_4byte(unsigned int adress)
{
	uint32_t data = EEPROM_read_2byte(adress + 2) << 16;
	asm("nop");
	data += EEPROM_read_2byte(adress);
	return data;
}

void EEPROM_write_string(unsigned int adress, char str[])
{
	for(int i=0; str[i] != '\0'; i++)
		EEPROM_write_1byte(adress + i, (unsigned char) str[i]);
}

unsigned char* EEPROM_read_string(unsigned int adress, unsigned int length)
{
	unsigned char *str = (unsigned char *) malloc(length);
	for(int i=0; i < length; i++)
		str[i] = EEPROM_read_1byte(adress + i);
	return str;
}

#endif /* EEPROM_H_ */