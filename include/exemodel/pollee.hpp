#pragma once
/**
 * \file	exemodel/pollee.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <unistd.h>
#include <stropts.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/epoll.h>

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
	explicit pollee(int fd, uint32_t evts, const char * info);
	virtual ~pollee();
public:
	ssize_t pread(void *buf, size_t nbyte, off_t offset)
	{
		return ::pread(m_fd, buf, nbyte, offset);
	}

	ssize_t read(void *buf, size_t nbyte)
	{
		return ::read(m_fd, buf, nbyte);
	}

	ssize_t pwrite(const void *buf, size_t nbyte, off_t offset)
	{
		return ::pwrite(m_fd, buf, nbyte, offset);
	}

	ssize_t write(const void *buf, size_t nbyte)
	{
		return ::write(m_fd, buf, nbyte);
	}

	template< typename _T >
	int io_ctl(int request, _T v)
	{
		return ::ioctl(m_fd, request, v);
	}

	int io_ctl(int request)
	{
		return ::ioctl(m_fd, request);
	}

	template< typename _T >
	int f_cntl(int cmd, _T v)
	{
		return ::fcntl(m_fd, cmd, v);
	}

	int f_cntl(int cmd)
	{
		return ::fcntl(m_fd, cmd);
	}

	int f_stat(struct stat & buf)
	{
		return ::fstat(m_fd, &buf);
	}
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 * \note all subclasses of \em pollee must implement it.
	 */
	virtual void dispose(poller & mgr, uint32_t evts) = 0;
protected:
	/**
	 * \note only used by poller in following situation
	 * \enum add/mod/del pollee in/in/from poller
	 * \enum used by subclass
	 */
	friend class poller;
	int _fd(void) const { return m_fd; }
	uint32_t _evts(void) const { return m_evts; }
private:
	pollee(const pollee & rhs) = delete;
	pollee & operator = (const pollee & rhs) = delete;
protected:
	uint32_t	m_evts;		// the events *this* object caring about.
private:
	const int	m_fd;		// the fd reprented by *this* object.
};

}
