#pragma once
/**
 * \file	exemodel/dev_i2c.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include "exemodel/file.hpp"

namespace exemodel {

class dev_i2c : private file {
public:
	explicit dev_i2c() = default;
	virtual ~dev_i2c() = default;
public:
	int open(int bus_idx);
public:
	int set_slave(int addr, bool force = false);
	int smbus_read8(uint8_t addr);
	int smbus_write8(uint8_t addr, uint8_t val);
	int smbus_read16(uint8_t addr);
	int smbus_write16(uint8_t addr, uint16_t val);
	int read(uint16_t addr, uint16_t reg, uint8_t & val);
	int write(uint16_t addr, uint16_t reg, uint8_t val);
private:
	dev_i2c(const dev_i2c & rhs) = delete;
	dev_i2c & operator = (const dev_i2c & rhs) = delete;
};

}
