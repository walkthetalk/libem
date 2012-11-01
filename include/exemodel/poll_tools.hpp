#pragma once
/**
 * \file	exemodel/poll_tools.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <errno.h>
#include <system_error>
#include <iostream>

namespace exemodel {

/**
 * \brief logger procedure.
 * \param ctx	user can specify a string to log.
 */
inline void emlog(const char * ctx)
{
#if defined(NDEBUG)
	std::cout << ctx << std::endl;
#else
#endif
}

/**
 * \brief validate the file descriptor if it is valid.
 * \param fd	the file descriptor need validated.
 */
inline int validate_fd(int fd)
{
	if (fd < 0) {
		throw std::system_error(errno, std::system_category(), "validate fd");
	}

	return fd;
}

/**
 * \brief validate the value returned by OS call.
 * \param ret	the value returen by OS call.
 * \param ctx	the string used as the arguments of exception.
 */
inline void validate_ret(int ret, const char * ctx)
{
	if (ret == -1) {
		throw std::system_error(errno, std::system_category(), ctx);
	}
}

}
