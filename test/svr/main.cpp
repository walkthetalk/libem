#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <iomanip>

#include "exemodel/wsserveree.hpp"
#include "jmsg/jmsg_sender.hpp"
#include "jmsg/jmsg_rcver.hpp"

int main(void)
{
	//signal(SIGPIPE,SIG_IGN);

	exemodel::wsserveree svr(5678);
	sender s(LWS_PRE);
	rcver r;
	svr.setMessageCallback([&r, &s, &svr](exemodel::wsserveree::cid id, void* buf, size_t length) {
		r.process(buf, length);
		struct count_down smsg = { 978 };
		s.fill_to<mid_t::countDown>(smsg, [&svr, id](void* b, size_t l){
			svr.sendTextMessage(id, b, l);
		});
	}, nullptr);
	svr.run();

	return 0;
}
