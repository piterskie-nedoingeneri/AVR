#ifndef BUFFER_H_
#define BUFFER_H_

#define BUFFER_SIZE 8

struct BUFFER {
	unsigned char data[BUFFER_SIZE];
	unsigned int read_index;
	unsigned int write_index;
};

bool buffer_is_empty(struct BUFFER *buffer)
{
	if(buffer->read_index == buffer->write_index)
		return True;
	else
		return False;
}

bool buffer_is_full(struct BUFFER *buffer)
{
	if((buffer->write_index == buffer->read_index - 1) || (buffer->read_index == 0 && buffer->write_index == BUFFER_SIZE - 1))
		return True;
	else
		return False;
}

void buffer_clear(struct BUFFER *buffer)
{
	buffer->read_index = 0;
	buffer->write_index = 0;
	for(unsigned int i=0; i < BUFFER_SIZE; i++)
	{
		buffer->data[i] = '0';
	}
}

// количество байт доступных для чтения
unsigned int buffer_read_size(struct BUFFER *buffer)
{
	if(!buffer_is_empty(buffer))
	{
		if(buffer->read_index < buffer->write_index)
			return buffer->write_index - buffer->read_index;
		else
			return ((BUFFER_SIZE - buffer->read_index) + (buffer->write_index - 1));
	}
	else
		return 0;
}

// добавить 1 элемент в буфер
void buffer_add_one(struct BUFFER *buffer, unsigned char elem)
{
	if(!buffer_is_full(buffer))
	{
		buffer->data[buffer->write_index] = elem;
		if(buffer->write_index == BUFFER_SIZE - 1)
			buffer->write_index = 0;
		else
			buffer->write_index++;
	}
}

// прочитать один элемент из буфера
unsigned char buffer_read_one(struct BUFFER *buffer)
{
	if(buffer->read_index == BUFFER_SIZE - 1)
	{
		buffer->read_index = 0;
		return buffer->data[BUFFER_SIZE - 1];
	}
	else
	{
		buffer->read_index++;
		return buffer->data[buffer->read_index - 1];
	}
}

// прочитать строку из буфера
unsigned char *buffer_read_string(struct BUFFER *buffer, unsigned int length)
{
	unsigned char *str = NULL;
	if(buffer_read_size(buffer) >= length)
	{
		str = (unsigned char *) malloc(length);
		for(unsigned int i=0; i < length; i++)
		{
			str[i] = buffer_read_one(buffer);
		}
	}
	return str;
}

#endif /* BUFFER_H_ */