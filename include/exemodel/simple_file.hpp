#pragma once
/**
 * \file	exemodel/simple_file.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <cstdint>
#include <cstddef>

#include <exemodel/poll_tools.hpp>

namespace exemodel {

/**
 * \brief simple_file
 */
template< oflag_t _oflag>
class simple_file {
	static constexpr int __oflag = static_cast<int>(_oflag);
public:
	simple_file(const char * path)
	: m_fd(validate_fd(::open(path, __oflag), ""))
	, m_path(path)
	{
	}

	virtual ~simple_file()
	{
		::close(m_fd);
	}

	ssize_t pread(void *buf, size_t nbyte, off_t offset)
	{
		return ::pread(m_fd, buf, nbyte, offset);
	}

	ssize_t read(void *buf, size_t nbyte)
	{
		return ::read(m_fd, buf, nbyte);
	}

	ssize_t pwrite(const void *buf, size_t nbyte, off_t offset)
	{
		return ::pwrite(m_fd, buf, nbyte, offset);
	}

	ssize_t write(const void *buf, size_t nbyte)
	{
		return ::write(m_fd, buf, nbyte);
	}

	template< typename _T >
	int io_ctl(unsigned long int request, _T v)
	{
		return ::ioctl(m_fd, request, v);
	}

	int io_ctl(unsigned long int request)
	{
		return ::ioctl(m_fd, request);
	}

	template< typename _T >
	int f_cntl(int cmd, _T v)
	{
		return ::fcntl(m_fd, cmd, v);
	}

	int f_cntl(int cmd)
	{
		return ::fcntl(m_fd, cmd);
	}

	int f_stat(struct stat & buf)
	{
		return ::fstat(m_fd, &buf);
	}

	const char * path(void)
	{
		return m_path.c_str();
	}
private:
	simple_file(const simple_file & rhs) = delete;
	simple_file & operator = (const simple_file & rhs ) = delete;

private:
	int m_fd;
	std::string m_path;
};

using simple_file_ro = simple_file<oflag_t::ro>;
using simple_file_wo = simple_file<oflag_t::wo>;
using simple_file_rw = simple_file<oflag_t::rw>;

}
