#pragma once

//#include <unistd.h>	//close
//#include <stdio.h>	//perror
#include <stdlib.h>	//exit
#include <assert.h>
#include <string.h>	//strerror
#include <errno.h>

#include <iostream>
#include <system_error>

namespace exemodel {

inline int validate_fd(int fd)
{
	if (fd < 0) {
		throw std::system_error(errno, std::system_category(), "validate fd");
	}

	return fd;
}

inline void validate_ret(int ret, const char * ctx)
{
	if (ret == -1) {
		throw std::system_error(errno, std::system_category(), ctx);
	}
}

}
