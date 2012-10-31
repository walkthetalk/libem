#pragma once

namespace exemodel {

class pollee;

class poller {
public:
	explicit poller();
	virtual ~poller();
public:
	void add(pollee & obj) const;
	void del(pollee & obj) const;
	void mod(pollee & obj) const;
public:
	void run(void);
private:
	poller(const poller & rhs) = delete;
	poller & operator = (const poller & rhs) = delete;
private:
	int m_fd;
};

}
