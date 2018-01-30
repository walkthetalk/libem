
#include "exemodel/pollee.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

int pollee::setfl_nonblock()
{
	return setfl(O_NONBLOCK);
}

int pollee::setfd_cloexec()
{
	return setfd(FD_CLOEXEC);
}

}
