#pragma once

#include <cstdint>
#include <cstddef>

namespace exemodel {

class poller;

class pollee {
public:
	explicit pollee(int fd, uint32_t evts);
	virtual ~pollee();
public:
	virtual void dispose(poller & mgr, uint32_t evts) = 0;
public:
	int _fd_(void) const { return m_fd; }
	uint32_t _evts_(void) const { return m_evts; }
private:
	pollee(const pollee & rhs) = delete;
	pollee & operator = (const pollee & rhs) = delete;
protected:
	uint32_t	m_evts;		// the events *this* object caring about.
private:
	const int	m_fd;		// the fd reprented by *this* object.
};

}
