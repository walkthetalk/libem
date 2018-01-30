#pragma once

#include <fcntl.h>
#include <sys/stat.h>

#include <cstdint>
#include <cstddef>

#include <string>

namespace exemodel {

class file {
public:
	explicit file();
	virtual ~file();
public:
	/// NOTE: checking fd is users responsibility.
	void savefd(int fd);
	int open(const char *path, int oflag);

	int close(void);

	void enable_autorm() { m_autorm = true; }
	void disable_autorm() { m_autorm = false; }
public:
	ssize_t pread(void *buf, size_t nbyte, off_t offset);
	ssize_t read(void *buf, size_t nbyte);
	ssize_t pwrite(const void *buf, size_t nbyte, off_t offset);
	ssize_t write(const void *buf, size_t nbyte);

	int setfl(int arg);
	int setfd(int arg);

	int f_stat(struct stat & buf)
	{
		return ::fstat(m_fd, &buf);
	}

	int fd(void) const { return m_fd; }
private:
	file(const file & rhs) = delete;
	file & operator = (const file & rhs) = delete;
private:
	int m_fd;
	bool m_autorm;
	std::string m_path;
};

}
