#include <stdlib.h>
#include "srv_dev_mng.hpp"
#include <unistd.h>

int main()
{
	svcDevMng::svcDeviceManager sdm(5903);
	sdm.run();

	return 0;
}
