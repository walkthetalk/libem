#include <thread>
#include <functional>
#include <csignal>

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <exemodel/poll_tools.hpp>
#include <exemodel/poller.hpp>
#include "exemodel/signalee.hpp"
#include "zlog/zlog.hpp"

class thread_test final
: public exemodel::poller
{
public:
	thread_test()
	: m_sig(SIGUSR1)
	{
		m_sig.connect([this](){
			throw exemodel::exec_stop("thread test");
		});
		this->add(m_sig);
	}

	~thread_test()
	{
		this->del(m_sig);
	}
private:
	exemodel::signalee m_sig;
};

static std::function<void()> s_sigterm_hdl;
void signal_handler(int sig)
{
	std::cout << "haha " << sig << std::endl;
	zlog::zlog_base(LOG_INFO, "received sigterm %d");
	s_sigterm_hdl();
}

int main(void)
{
	//exemodel::mask_signal(SIGUSR1);

	/*start fusion splice service*/
	std::unique_ptr<thread_test> fs_svr(new thread_test());
	std::thread th1([&fs_svr](void){
		exemodel::mask_signal(SIGUSR1);
		fs_svr->run();
	});

	/*start heat service*/
	std::unique_ptr<thread_test> heat_svr(new thread_test());
	std::thread th2([&heat_svr](void){
		exemodel::mask_signal(SIGUSR1);
		heat_svr->run();
	});

	/*start device manage service*/
	std::unique_ptr<thread_test> devmng_svr(new thread_test());
	std::thread th3([&devmng_svr](void){
		exemodel::mask_signal(SIGUSR1);
		devmng_svr->run();
	});

	s_sigterm_hdl = [&th1, &th2, &th3](void) {
	std::cout << "start send sigusr1" << std::endl;
		::pthread_kill(th1.native_handle(), SIGUSR1);
		::pthread_kill(th2.native_handle(), SIGUSR1);
		::pthread_kill(th3.native_handle(), SIGUSR1);
	std::cout << "stop send sigusr1" << std::endl;
	};

	//exemodel::mask_signal(SIGTERM);
	std::signal(SIGINT, signal_handler);

	std::cout << "start wait" << std::endl;
	th1.join();
	std::cout << "start wait1" << std::endl;
	th2.join();
	std::cout << "start wait2" << std::endl;
	th3.join();
	std::cout << "start wait3" << std::endl;

	return 0;
}
