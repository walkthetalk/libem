#pragma once
#include <exemodel/signalee.hpp>
#include <exemodel/wsserveree.hpp>
#include <exemodel/binary_rcver.hpp>
#include <exemodel/timeree.hpp>

#include "jmsg/jmsg_sender.hpp"
#include "jmsg/jmsg_rcver.hpp"

class serverExt : public exemodel::wsserveree {
public:
	serverExt(uint16_t port);
	virtual ~serverExt();
public:
	template<mid_t mid>
	void register_callback(std::function<void(typename msg_helper<mid>::value_type &)> cb)
	{
		m_rcver.register_callback<mid>(cb);
	}

	template<bmid_t bmid>
	void register_callback(std::function<void(std::vector<uint8_t> &)> cb)
	{
		m_brcver.register_callback<bmid>(cb);
	}

	template< mid_t mid >
	void send(typename msg_helper<mid>::value_type const & msg)
	{
		m_sender.fill_to<mid>(msg, [this](void *buf, size_t len) {
			this->sendTextMessage(m_cid, buf, len);
		});
	}
	
	virtual void fifo_processer(std::vector<uint8_t> & buf) = 0;
private:
	cid m_cid;
	rcver m_rcver;
	sender m_sender;
	brcver m_brcver;
	exemodel::signalee m_sigterm;
	exemodel::monotonic_timeree m_tmr;
};
