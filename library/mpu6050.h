#ifndef MPU6050_H_
#define MPU6050_H_

//////////////////////////////////////////////// Defines

// Addresses
#define MPU6050_ADDRESS 0b1101000
#define MPU6050_WRITE 0b11010000
#define MPU6050_READ 0b11010001

// Accelerometer Measurements
#define MPU6050_AX 0x3B
#define MPU6050_AY 0x3D
#define MPU6050_AZ 0x3F

// Temperature Measurement
#define MPU6050_T 0x41

// Gyroscope Measurements
#define MPU6050_GX 0x43
#define MPU6050_GY 0x45
#define MPU6050_GZ 0x47

// Power Management 1
#define MPU6050_PM1 0x6B

// Accelerometer Configuration
#define MPU6050_AC 0x1C

// Who Am I
#define MPU6050_WAI 0x75

//////////////////////////////////////////////// Functions

inline void mpu_write_1byte(uint8_t register_adress, uint8_t data)
{
	I2C_start();
	I2C_send_byte(MPU6050_WRITE);
	I2C_send_byte(register_adress);
	I2C_send_byte(data);
	I2C_stop();
}

inline uint8_t mpu_read_1byte(uint8_t register_adress)
{
	uint8_t data = 0;
	I2C_start();
	I2C_send_byte(MPU6050_WRITE);
	I2C_send_byte(register_adress);
	I2C_start();
	I2C_send_byte(MPU6050_READ);
	data |= I2C_read_byte_no_ack();
	I2C_stop();
	return data;
}

inline int mpu_read_2byte(uint8_t register_adress)
{
	int data = 0;
	I2C_start();
	I2C_send_byte(MPU6050_WRITE);
	I2C_send_byte(register_adress);
	I2C_start();
	I2C_send_byte(MPU6050_READ);
	data |= I2C_read_byte() << 8;
	data |= I2C_read_byte_no_ack();
	I2C_stop();
	return data;
}

void mpu_init()
{
	mpu_write_1byte(MPU6050_PM1, 0x00);
	//mpu_write_1byte(MPU6050_AC, 0x00);
}

bool mpu_check()
{
	I2C_start();
	I2C_send_byte(MPU6050_WRITE);
	I2C_send_byte(MPU6050_WAI);
	I2C_start();
	I2C_send_byte(MPU6050_READ);
	uint8_t status = I2C_read_byte_no_ack();
	I2C_stop();
	if(status == MPU6050_ADDRESS)
		return True;
	else
		return False;
}

float mpu_read_t()
{
	int temperature = mpu_read_2byte(MPU6050_T);
	return (temperature / 340) + 36.53;
}

int mpu_read_ax()
{
	return mpu_read_2byte(MPU6050_AX);
}

int mpu_read_ay()
{
	return mpu_read_2byte(MPU6050_AY);
}

int mpu_read_az()
{
	return mpu_read_2byte(MPU6050_AZ);
}

#endif /* MPU6050_H_ */