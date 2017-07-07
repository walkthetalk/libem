#pragma once

/// error
#include <errno.h>
#include <system_error>
/// open
#include <sys/stat.h>
#include <fcntl.h>
/// close / read
#include <unistd.h>
///
#include <type_traits>
///
#include <string>
#include <sstream>

#include <exemodel/poll_tools.hpp>

namespace exemodel {

/**
 * \brief device attribute
 */
template< oflag_t _oflag, typename _val_t >
class dev_attr final {
	static constexpr int __oflag = static_cast<int>(_oflag);
public:
	dev_attr(const char * path)
	: m_fd(validate_fd(::open(path, __oflag), ""))
	, m_path(path)
	, m_soft_val()
	{
		if (m_fd < 0) {
			throw std::system_error(errno, std::system_category(), path);
		}

		if (_oflag != oflag_t::wo) {
			m_soft_val = this->read();
		}
	}

	~dev_attr()
	{
		::close(m_fd);
	}

	_val_t read()
	{
		char buf[16] = "";
		ssize_t ret = ::pread(m_fd, buf, sizeof(buf), 0);
		if (ret < 0) {
			throw std::system_error(errno, std::system_category(), "read dev attr " + m_path);
		}

		std::stringstream ss(buf);
		_val_t result;
		ss >> result;
		return result;
	}

	_val_t read_soft()
	{
		return m_soft_val;
	}

	void write(const _val_t & val)
	{
		std::stringstream ss;
		ss << val << "\n";
		std::string s = ss.str();

		ssize_t ret = ::pwrite(m_fd, s.c_str(), s.size(), 0);
		if (ret < 0) {
			throw std::system_error(errno, std::system_category(), "write dev attr " + m_path);
		}

		m_soft_val = val;

		return;
	}
private:
	dev_attr(const dev_attr & rhs) = delete;
	dev_attr & operator = (const dev_attr & rhs ) = delete;

private:
	int m_fd;
	const std::string m_path;
	_val_t m_soft_val;	/// \note the hard value may not same as soft value
};

template < typename _val_t >
using dev_attr_ro = dev_attr<oflag_t::ro, _val_t>;
template < typename _val_t >
using dev_attr_wo = dev_attr<oflag_t::wo, _val_t>;
template < typename _val_t >
using dev_attr_rw = dev_attr<oflag_t::rw, _val_t>;

/**
 * \brief device attribute advanced
 */
template< oflag_t _oflag, typename _val_t, unsigned long _factor >
class dev_attr_adv {
public:
	dev_attr_adv(const char * path)
	: m_attr(path)
	{
	}

	~dev_attr_adv()
	{
	}

	_val_t read_raw(void)
	{
		return m_attr.read();
	}

	void write_raw(_val_t raw)
	{
		m_attr.write(raw);
	}

	double read(void)
	{
		return static_cast<double>(m_attr.read()) / static_cast<double>(_factor);
	}

	void write(double val)
	{
		m_attr.write(static_cast<_val_t>(val * static_cast<double>(_factor)));
	}
private:
	dev_attr_adv(const dev_attr_adv & rhs) = delete;
	dev_attr_adv & operator = (const dev_attr_adv & rhs ) = delete;
private:
	dev_attr<_oflag, _val_t> m_attr;
};

template < typename _val_t, unsigned long _factor >
using dev_attr_adv_ro = dev_attr_adv<oflag_t::ro, _val_t, _factor>;
template < typename _val_t, unsigned long _factor >
using dev_attr_adv_wo = dev_attr_adv<oflag_t::wo, _val_t, _factor>;
template < typename _val_t, unsigned long _factor >
using dev_attr_adv_rw = dev_attr_adv<oflag_t::rw, _val_t, _factor>;

/**
 * \brief device attribute normalized
 */
template< oflag_t _oflag, typename _val_t>
class dev_attr_nor {
public:
	dev_attr_nor(const char * path, long maxv, long minv = 0)
	: m_attr(path)
	, m_min(minv)
	, m_max(maxv)
	{
	}

	~dev_attr_nor()
	{
	}

	_val_t read_raw(void)
	{
		return m_attr.read();
	}

	void write_raw(_val_t raw)
	{
		m_attr.write(raw);
	}

	double read(void)
	{
		return (double)(read_raw() - m_min) / (double)(m_max - m_min);
	}

	void write(double val)
	{
		m_attr.write((_val_t)((double)(m_max - m_min) * val + (double)m_min));
	}

	long get_min() const { return m_min; }
	long get_max() const { return m_max; }
private:
	dev_attr_nor(const dev_attr_nor & rhs) = delete;
	dev_attr_nor & operator = (const dev_attr_nor & rhs ) = delete;
private:
	dev_attr<_oflag, _val_t> m_attr;
	const long m_min;
	const long m_max;
};

template < typename _val_t>
using dev_attr_nor_ro = dev_attr_nor<oflag_t::ro, _val_t>;
template < typename _val_t>
using dev_attr_nor_wo = dev_attr_nor<oflag_t::wo, _val_t>;
template < typename _val_t>
using dev_attr_nor_rw = dev_attr_nor<oflag_t::rw, _val_t>;

} /* namespace exemodel */
