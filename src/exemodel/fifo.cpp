#include <sys/stat.h>

#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/fifo.hpp"

namespace exemodel {

/**
 * \note we use 'EPOLLET' for fifo_readee, because if remote end closed it, we
 * will receive EPOLLHUP always.
 */
fifo_readee::fifo_readee(const char* path)
: pollee(::open(path, (O_RDONLY | O_NONBLOCK)),
	 (uint32_t)(::EPOLLIN | ::EPOLLET),
	 "fofo_readee")
, m_path(path)
{
}

fifo_readee::~fifo_readee()
{
}

/**
 * \note because fifo_readee is 'EPOLLET', so we must read until empty.
 */
void fifo_readee::dispose(poller &, uint32_t evts)
{
	if (evts & ::EPOLLIN) {
		m_data.clear();

		uint8_t buf;
		do {
			ssize_t ret = this->read(&buf, sizeof(buf));
			if (ret <= 0) {
				break;
			}

			m_data.push_back(buf);
		} while (true);

		if (m_data.size()) {
			this->exe(m_data);
		}
	}
#if 0
	else {
		zlog::zlog_debug("exemodel: unkown fifo_readee event, %u", evts);
	}
#endif
}

fifo_writee::fifo_writee(const char* path)
: pollee(::open(path, (O_WRONLY | O_NONBLOCK)),
	 (uint32_t)(::EPOLLOUT),
	 "fifo_writee")
, m_path(path)
{
}

fifo_writee::~fifo_writee()
{
}

void fifo_writee::dispose(poller &, uint32_t evts)
{
	if (evts & ::EPOLLOUT) {
		m_data.clear();
		this->exe(m_data);

		ssize_t ret = this->write(m_data.data(), m_data.size());
		if (ret < 0
			|| (size_t)ret != m_data.size()) {
			return;
		}
	}
	else {
		zlog::zlog_debug("exemodel: unkown fifo_writee event, %u", evts);
	}
}

} /* namespace exemodel */
