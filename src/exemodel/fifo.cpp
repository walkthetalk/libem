#include <sys/types.h>
#include <sys/stat.h>

#include "exemodel/fifo.hpp"

namespace exemodel {

int fifo_readee::init(const char *path)
{
	int ret = 0;

	ret = ::mkfifo(path, 0666);
	if (ret == -1 && errno != EEXIST)
		return ret;

	this->enable_autorm();

	ret = this->open(path, (O_RDONLY | O_NONBLOCK));
	if (ret == -1)
		return ret;

	ret = this->setfd_cloexec();
	if (ret == -1)
		return ret;

	saveevts(EPOLLIN | EPOLLET);

	return 0;
}

/**
 * \note because fifo_readee is 'EPOLLET', so we must read until empty.
 */
int fifo_readee::dispose(poller &, uint32_t evts)
{
	if (evts & EPOLLIN) {
		std::string tmpdata;

		char buf;
		do {
			ssize_t ret = this->read(&buf, sizeof(buf));
			if (ret <= 0) {
				break;
			}

			tmpdata.push_back(buf);
		} while (true);

		if (tmpdata.size()) {
			int ret = m_processor(tmpdata);
			if (ret)
				return ret;
		}
	}

	return 0;
}

int fifo_writee::init(const char *path)
{
	int ret = this->open(path, (O_WRONLY | O_NONBLOCK));
	if (ret == -1)
		return ret;

	ret = this->setfd_cloexec();
	if (ret == -1)
		return ret;

	saveevts(EPOLLOUT);

	return 0;
}

int fifo_writee::dispose(poller &, uint32_t evts)
{
	if (evts & EPOLLOUT) {
		std::string tmpdata;
		int ret = m_processor(tmpdata);
		if (ret)
			return ret;

		this->write(tmpdata.data(), tmpdata.size());
	}

	return 0;
}

} /* namespace exemodel */
