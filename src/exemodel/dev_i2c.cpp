
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

#include "exemodel/dev_i2c.hpp"

namespace exemodel {

int dev_i2c::open(int bus_idx)
{
	char filename[20];
	snprintf(filename, sizeof(filename)-1, "/dev/i2c-%d", bus_idx);
	return file::open(filename, O_RDWR);
}

int dev_i2c::set_slave(int addr, bool force)
{
	return ::ioctl(fd(), force ? I2C_SLAVE_FORCE : I2C_SLAVE, addr);
}

static int i2c_smbus_access(int file, char read_write, uint8_t command,
		       int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	return ::ioctl(file, I2C_SMBUS, &args);
}

int i2c_smbus_write_quick(int file, uint8_t value)
{
	return i2c_smbus_access(file, value, 0, I2C_SMBUS_QUICK, nullptr);
}

int i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	int err = i2c_smbus_access(file, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data);
	if (err < 0)
		return err;

	return data.byte;
}

int i2c_smbus_write_byte(int file, uint8_t value)
{
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, value,
				I2C_SMBUS_BYTE, NULL);
}

int i2c_smbus_read_byte_data(int file, uint8_t command)
{
	union i2c_smbus_data data;
	int err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_BYTE_DATA, &data);
	if (err < 0)
		return err;

	return data.byte;
}

int i2c_smbus_write_byte_data(int file, uint8_t command, uint8_t value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_BYTE_DATA, &data);
}

int i2c_smbus_read_word_data(int file, uint8_t command)
{
	union i2c_smbus_data data;
	int err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_WORD_DATA, &data);
	if (err < 0)
		return err;

	return data.word;
}

int i2c_smbus_write_word_data(int file, uint8_t command, uint16_t value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_WORD_DATA, &data);
}

int i2c_smbus_process_call(int file, uint8_t command, uint16_t value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
			     I2C_SMBUS_PROC_CALL, &data))
		return -1;
	else
		return data.word;
}

/* Returns the number of read bytes */
int i2c_smbus_read_block_data(int file, uint8_t command, uint8_t *values)
{
	union i2c_smbus_data data;
	int err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_BLOCK_DATA, &data);
	if (err < 0)
		return err;

	const uint8_t   num = data.block[0];
	const uint8_t * src = &data.block[1];
	for (int i = 0; i < num; ++i)
		values[i] = src[i];
	return num;
}

int i2c_smbus_write_block_data(int file, uint8_t command, uint8_t length,
				 const uint8_t *values)
{
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;

	union i2c_smbus_data data;
	uint8_t * dst = &data.block[1];
	for (int i = 0; i < length; ++i)
		dst[i] = values[i];
	data.block[0] = length;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
/* Until kernel 2.6.22, the length is hardcoded to 32 bytes. If you
   ask for less than 32 bytes, your code will only work with kernels
   2.6.23 and later. */
int i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length,
				    uint8_t *values)
{
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;

	union i2c_smbus_data data;
	data.block[0] = length;

	int err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN :
				I2C_SMBUS_I2C_BLOCK_DATA, &data);
	if (err < 0)
		return err;

	const uint8_t   num = data.block[0];
	const uint8_t * src = &data.block[1];
	for (int i = 0; i <= num; i++)
		values[i] = src[i];
	return num;
}

int i2c_smbus_write_i2c_block_data(int file, uint8_t command, uint8_t length,
				     const uint8_t *values)
{
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;

	union i2c_smbus_data data;
	uint8_t * dst = &data.block[1];
	for (int i = 0; i < length; ++i)
		dst[i] = values[i];
	data.block[0] = length;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_I2C_BLOCK_BROKEN, &data);
}

/* Returns the number of read bytes */
int i2c_smbus_block_process_call(int file, uint8_t command, uint8_t length,
				   uint8_t *values)
{
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;

	union i2c_smbus_data data;
	for (int i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;

	int err = i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
			       I2C_SMBUS_BLOCK_PROC_CALL, &data);
	if (err < 0)
		return err;

	for (int i = 1; i <= data.block[0]; i++)
		values[i-1] = data.block[i];
	return data.block[0];
}

int dev_i2c::smbus_read8(uint8_t addr)
{
	return i2c_smbus_read_byte_data(fd(), addr);
}

int dev_i2c::smbus_write8(uint8_t addr, uint8_t val)
{
	return i2c_smbus_write_byte_data(fd(), addr, val);
}

int dev_i2c::smbus_read16(uint8_t addr)
{
	return i2c_smbus_read_word_data(fd(), addr);
}

int dev_i2c::smbus_write16(uint8_t addr, uint16_t val)
{
	return i2c_smbus_write_word_data(fd(), addr, val);
}

int dev_i2c::read(uint16_t addr, uint16_t reg, uint8_t & val)
{
	uint8_t buf[2] = {
		(uint8_t)(reg >> 8),
		(uint8_t)(reg & 0xFF),
	};

	const uint16_t flags = 0x0;
	struct i2c_msg msg[2] = {
		{ addr, flags, sizeof(buf), buf },
		{ addr, (flags | I2C_M_RD), 1, buf },
	};

	struct i2c_rdwr_ioctl_data rdwr = { &msg[0], 2 };
	const int ret = ::ioctl(fd(), I2C_RDWR, &rdwr);
	if (ret < 0) {
		return ret;
	}

	val = buf[0];
	return 0;
}

int dev_i2c::write(uint16_t addr, uint16_t reg, uint8_t val)
{
	uint8_t buf[3] = {
		(uint8_t)(reg >> 8),
		(uint8_t)(reg & 0xFF),
		val,
	};

	struct i2c_msg msg = {
		addr,
		0,	// flags
		3,	// buffer length
		buf,
	};

	struct i2c_rdwr_ioctl_data rdwr = { &msg, 1 };
	const int ret = ::ioctl(fd(), I2C_RDWR, &rdwr);
	if (ret < 0) {
		return ret;
	}

	if (ret != 1) {
		return -1;
	}
	return 0;
}

}
