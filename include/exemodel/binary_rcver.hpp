#pragma once
#include <map>
#include <string>
#include <functional>

typedef unsigned char uchar;

enum class bmid_t : uint16_t {
	sendUpdateData = 0,
};

static std::string const s_bmid_to_str[] = {
	"sendUpdateData",
};

class brcver final {
public:
	brcver() {}
	~brcver() {}
public:
	void process(void * buf, size_t len) 
	{
		std::string msgid;
		char * msg = (char *)buf;
		for (int i=0; i < 32; i++)
		{
			if (*(char *)msg != ' ')
			{ 
				msgid += *(char *)msg; 
			}
			msg++;
		}
		std::vector<uint8_t> data((uchar *)msg, (uchar *)msg + (len - 32));
		m_data = data;
		auto & cb = m_cbs[msgid];
		if (cb) { cb(); }
	}

	template<bmid_t bmid>
	void register_callback(std::function<void(std::vector<uint8_t> &)> cb)
	{
		m_cbs[__bmid_to_str(bmid)] = [this, cb](void) -> void {
			std::vector<uint8_t> data = m_data;
			cb(data);
			m_data.clear();
		};
	}

private:
	const std::string __bmid_to_str(bmid_t bmid)
	{
		return s_bmid_to_str[(uint16_t)bmid];
	}

private:
	std::vector<uint8_t> m_data;
	std::map<std::string, std::function<void(void)>> m_cbs;
};
