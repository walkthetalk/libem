
#include "test/client.hpp"

int main(void)
{
	//signal(SIGPIPE,SIG_IGN);

	CClient client(0x7F000001, 65533);
	client.run();

	return 0;
}
