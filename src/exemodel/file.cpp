#include <cerrno>

#include <unistd.h>	/// for close/unlink

#include "exemodel/file.hpp"

namespace exemodel {

file::file()
: m_fd(-1)
, m_autorm(false)
, m_path("")
{
}

file::~file()
{
	if (m_fd >= 0) {
		::close(m_fd);
	}

	if (m_autorm) {
		::unlink(m_path.c_str());
	}
}

void file::savefd(int fd)
{
	m_fd = fd;
}

int file::open(const char *path, int oflag)
{
	int ret = 0;
	ret = ::open(path, oflag);
	if (ret == -1) {
		return ret;
	}

	m_fd = ret;
	m_path = path;

	return 0;
}

int file::close()
{
	int ret = ::close(m_fd);
	if (ret == -1)
		return ret;

	m_fd = -1;
	return 0;
}

ssize_t file::pread(void *buf, size_t nbyte, off_t offset)
{
	return ::pread(m_fd, buf, nbyte, offset);
}

ssize_t file::read(void *buf, size_t nbyte)
{
	return ::read(m_fd, buf, nbyte);
}

ssize_t file::pwrite(const void *buf, size_t nbyte, off_t offset)
{
	return ::pwrite(m_fd, buf, nbyte, offset);
}

ssize_t file::write(const void *buf, size_t nbyte)
{
	return ::write(m_fd, buf, nbyte);
}

int file::setfl(int arg)
{
	int ret = ::fcntl(m_fd, F_GETFL);
	if (ret == -1) {
		return ret;
	}

	ret = ::fcntl(m_fd, F_SETFL, (ret | arg));
	if (ret == -1) {
		return ret;
	}

	return 0;
}

int file::setfd(int arg)
{
	int ret = ::fcntl(m_fd, F_GETFD);
	if (ret == -1) {
		return ret;
	}

	ret = ::fcntl(m_fd, F_SETFD, (ret | arg));
	if (ret == -1) {
		return ret;
	}

	return 0;
}

}
