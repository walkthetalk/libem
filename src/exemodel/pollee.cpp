#include <fcntl.h>	// fcntl

#include "exemodel/poll_tools.hpp"
#include "exemodel/pollee.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

pollee::pollee(int fd, uint32_t evts)
: m_evts(evts)
, m_fd(validate_fd(fd))
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

}
