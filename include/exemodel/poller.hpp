#pragma once
/**
 * \file	exemodel/poller.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
namespace exemodel {

class pollee;
/**
 * \class poller
 * \brief it can wait on some \em pollee(s) for events.
 */
class poller {
public:
	explicit poller();
	virtual ~poller();
public:
	/**
	 * \brief the \em add / \em del / \em mod can be used to add/delete
	 * /modify pollee to/from/in this poller.
	 */
	void add(pollee & obj) const;
	void del(pollee & obj) const;
	void mod(pollee & obj) const;
public:
	/**
	 * \brief the \em run will poll all fd(s) in it continuously.
	 * \note  it won't return.
	 */
	void run(void);
private:
	poller(const poller & rhs) = delete;
	poller & operator = (const poller & rhs) = delete;
private:
	int m_fd;
};

}
