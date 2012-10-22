#include <sys/epoll.h>

#include "exemodel/poll_tools.h"
#include "exemodel/connectee.h"

namespace exemodel {

connectee::connectee(const destroy_cb_t & destroycb, int idx, int fd)
: pollee(fd, (uint32_t)(::EPOLLIN | ::EPOLLET))
, m_destroycb(destroycb)
, m_idx(idx)
{
}

void connectee::dispose(poller & mgr, uint32_t evts)
{
	if (evts & ::EPOLLRDHUP) {
		m_destroycb(mgr, m_idx);
		return;
	} else if (evts & ::EPOLLIN) {
		connectee::args_t args = { mgr, *this, evts };
		this->exe(args);
	}
}

}
