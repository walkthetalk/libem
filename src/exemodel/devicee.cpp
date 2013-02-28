#include "exemodel/devicee.hpp"
#include "exemodel/poll_tools.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace exemodel{

devicee::devicee(const char* path)
: pollee(::open(path, O_RDWR), uint32_t(::EPOLLIN|::EPOLLOUT|::EPOLLERR))
, m_path(path)
{
}

devicee::~devicee()
{
}

void devicee::dispose(poller& mgr, uint32_t evts)
{
    //todo add body
    if ((evts & ::EPOLLERR)) {
            emlog("devicee ERROR");
            return;
        }

        devicee::args_t args = { mgr, *this, evts };
        this->exe(args);
}

}
