#include <unistd.h>	// read/write
#include <fcntl.h>	// fcntl

#include "exemodel/pollee.h"
#include "exemodel/poller.h"
#include "exemodel/poll_tools.h"

namespace exemodel {

pollee::pollee(int fd, uint32_t evts)
: m_fd(validate_fd(fd))
, m_evts(evts)
{
	int ret = 0;
	ret = ::fcntl(_fd_(), F_SETFL,
		::fcntl(_fd_(), F_GETFL, 0) | O_NONBLOCK);
	validate_ret(ret, "set nonblock");
	ret = ::fcntl(_fd_(), F_SETFD,
		::fcntl(_fd_(), F_GETFD, 0) | FD_CLOEXEC);
	validate_ret(ret, "set cloexec");
}

pollee::~pollee()
{
	close(m_fd);
}

size_t pollee::read(void * buf, size_t nbyte)
{
	ssize_t ret = ::read(_fd_(), buf, nbyte);
	if (ret < 0) {
		// TODO: log
		return 0;
	}

	return (size_t)ret;
}

size_t pollee::write(const void * buf, size_t nbyte)
{
	ssize_t ret = ::write(_fd_(), buf, nbyte);
	if (ret < 0) {
		// TODO: log
		return 0;
	}

	return (size_t)ret;
}

}
