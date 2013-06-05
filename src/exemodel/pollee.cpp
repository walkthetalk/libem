#include "exemodel/poll_tools.hpp"
#include "exemodel/pollee.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

pollee::pollee(int fd, uint32_t evts, const char * info)
: m_evts(evts)
, m_fd(validate_fd(fd, info))
{
	int ret = 0;
	ret = f_cntl(F_SETFL,
		f_cntl(F_GETFL, 0) | O_NONBLOCK);
	validate_ret(ret, "set nonblock");

	ret = f_cntl(F_SETFD,
		f_cntl(F_GETFD, 0) | FD_CLOEXEC);
	validate_ret(ret, "set cloexec");
}

pollee::~pollee()
{
	close(m_fd);
}

}
