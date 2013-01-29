#include "exemodel/devicee.hpp"
#include "exemodel/poll_tools.hpp"

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace exemodel{
    
devee::devee(char* path)
: pollee(::open(path, O_RDWR), uint32_t(::EPOLLIN|::EPOLLOUT|::EPOLLERR))
, m_path(path)
{    
}

devee::~devee()
{
}

void devee::dispose(poller& mgr, uint32_t evts)
{
    //todo add body
    if ((evts & ::EPOLLERR)) {
            emlog("devee ERROR");
            return;
        }

        devee::args_t args = { mgr, *this, evts };
        this->exe(args);
}

}
