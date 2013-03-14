#pragma once
/**
 * \file	exemodel/pollee.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <unistd.h>

#include <cstdint>
#include <cstddef>

namespace exemodel {

class poller;
/**
 * \class pollee
 * \brief \em poller can wait on it for any events it cared about.
 */
class pollee {
public:
	/**
	 * \brief the ctor of pollee
	 * \param fd	the file descriptor representing this pollee.
	 * \param evts	the events it cared about.
	 */
	explicit pollee(int fd, uint32_t evts);
	virtual ~pollee();
public:
	ssize_t read(void *buf, size_t nbyte)
	{
		return ::read(m_fd, buf, nbyte);
	}
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 * \note all subclasses of \em pollee must implement it.
	 */
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
