#include <sys/signalfd.h>

#include "exemodel/signalee.hpp"

namespace exemodel {

int mask_signal(int signum)
{
	sigset_t mask;
	int ret = sigemptyset(&mask);
	if (ret == -1)
		return ret;
	ret = sigaddset(&mask, signum);
	if (ret == -1)
		return ret;

	ret = ::sigprocmask(SIG_BLOCK, &mask, NULL);
	if (ret == -1)
		return ret;

	return 0;
}

int signalee::init(int signum)
{
	int ret = 0;

	ret = mask_signal(signum);
	if (ret == -1)
		return ret;

	sigset_t mask;
	ret = sigemptyset(&mask);
	if (ret == -1)
		return ret;
	ret = sigaddset(&mask, signum);
	if (ret == -1)
		return ret;

	ret = ::signalfd(-1, &mask, (SFD_NONBLOCK | SFD_CLOEXEC));
	if (ret == -1)
		return ret;

	this->savefd(ret);
	this->saveevts(EPOLLIN);

	return 0;
}

int signalee::dispose(poller &, uint32_t evts)
{
	if ((evts & EPOLLIN)) {
		struct signalfd_siginfo fdsi;
		ssize_t ret = this->read(&fdsi, sizeof(fdsi));
		if (ret == sizeof(fdsi)) {
			return m_processor();
		}
	}

	return 0;
}

}
