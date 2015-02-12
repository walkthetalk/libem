#pragma once
/**
 * \file	exemodel/poll_tools.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <errno.h>
#include <system_error>

#include <stdexcept>

namespace exemodel {

/**
 * \brief validate the file descriptor if it is valid.
 * \param fd	the file descriptor need validated.
 */
inline int validate_fd(int fd, const char * info)
{
	if (fd < 0) {
		throw std::system_error(errno, std::system_category(), info);
	}

	return fd;
}

/**
 * \brief validate the value returned by OS call.
 * \param ret	the value returen by OS call.
 * \param ctx	the string used as the arguments of exception.
 */
inline void validate_ret(long ret, const char * ctx)
{
	if (ret == -1) {
		throw std::system_error(errno, std::system_category(), ctx);
	}
}

/**
 * \brief indicate the run loop should stop.
 */
class exec_stop final : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

}
