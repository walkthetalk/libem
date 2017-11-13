#include <limits>

#include <exemodel/poll_tools.hpp>
#include "srv_dev_mng.hpp"

#include <iostream>
#include <fstream>
#include <string.h>
#include <openssl/md5.h>

namespace svcDevMng {

svcDeviceManager::svcDeviceManager(uint16_t port)
: serverExt(port)
, m_pkgc(0)
{
	register_callback<bmid_t::sendUpdateData>(
		[this](std::vector<uint8_t> & data) {
			std::cout << "sendUpdateData" << std::endl;
			std::string pos_str;		//data [0-31]
			std::string size_str;		//data [32-63]
			std::vector<uint8_t>::iterator iter;
			for (int i=0; i < 64; i++)
			{
				iter = data.begin();
				if (i < 32)
				{
					pos_str += *iter;
				}
				else
				{
					size_str += *iter;
				}

				data.erase(iter);
			}
			int pos = std::atoi(pos_str.c_str());
			int size = std::atoi(size_str.c_str());
			std::cout << pos << "," << size << std::endl;

			std::ofstream file;
			file.open("/tmp/udisk/updatePackage.tar.gz", std::ios::out|std::ios::in|std::ios::binary);
			if (!file.is_open()) {
				file.open("/tmp/udisk/updatePackage.tar.gz",std::ios::out|std::ios::binary);
			}
			file.seekp(pos, std::ios::beg);
			for (int i=0; i < size; i++)
			{
				file << data[i];
			}

			file.close();

			m_pkgc++;
			std::cout << m_pkgc << std::endl;
		}
	);
}

svcDeviceManager::~svcDeviceManager()
{
}

void svcDeviceManager::fifo_processer(std::vector<uint8_t> &)
{
}

}
