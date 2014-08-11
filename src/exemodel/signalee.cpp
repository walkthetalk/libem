#include <sys/signalfd.h>

#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/signalee.hpp"

namespace exemodel {

void mask_signal(int signum)
{
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, signum);

	int ret = ::sigprocmask(SIG_BLOCK, &mask, NULL);
	validate_ret(ret, "sigprocmask");
}

static int gen_signalfd(int signum)
{
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, signum);

	int sfd = ::signalfd(-1, &mask, 0);
	validate_fd(sfd, "generate signal fd");

	return sfd;
}

signalee::signalee(int signum)
: pollee(gen_signalfd(signum),
	 (uint32_t)(::EPOLLIN),
	 "signalee")
{
}

signalee::~signalee()
{
	/// \todo need deblock signum
}

void signalee::dispose(poller &, uint32_t evts)
{
	if ((evts & ::EPOLLIN)) {
		struct signalfd_siginfo fdsi;
		ssize_t ret = this->read(&fdsi, sizeof(fdsi));
		if (ret == sizeof(fdsi)) {
			this->exe();
		}
	}
}

}
