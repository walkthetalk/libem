#include <sys/epoll.h>

#include "exemodel/poll_tools.h"
#include "exemodel/connectee.h"

namespace exemodel {

connectee::connectee(IConnMgr & master, int idx, int fd)
: pollee(fd, (uint32_t)(::EPOLLIN | ::EPOLLET))
, m_master(master)
, m_idx(idx)
{
}

void connectee::dispose(poller & mgr, uint32_t evts)
{
	if (evts & ::EPOLLRDHUP) {
		m_master.destroy(mgr, m_idx);
		return;
	} else if (evts & ::EPOLLIN) {
		this->exe(mgr, *this, evts);
	}
}

}
