#pragma once

#include <cstddef>
#include <string>

class in_wrapper final {
public:
	typedef char Ch;
public:
	in_wrapper(Ch * p, std::size_t s)
	: m_cur(p)
	, m_p(p)
	, m_e(p + s)
	{
	}
	Ch Peek() const {
		if (m_cur == m_e) {
			return '\0';
		}
		else {
			return *m_cur;
		}
	}
	Ch Take() {
		if (m_cur == m_e) {
			return '\0';
		}
		else {
			Ch ret = *m_cur;
			++m_cur;
			return ret;
		}
	}
	size_t Tell() const {
		return (m_cur - m_p);
	}
private:
	in_wrapper(const in_wrapper&);
	in_wrapper& operator=(const in_wrapper&);
private:
	Ch * m_cur;
	Ch * const m_p;
	Ch * const m_e;
};

struct out_wrapper final {
public:
	typedef char Ch;
public:
	out_wrapper(Ch * p)
	: pcur(p)
	{
	}
	void Put(Ch c) { *pcur = c; ++pcur; }
	void Flush() { }
private:
	out_wrapper(const out_wrapper&);
	out_wrapper& operator=(const out_wrapper&);
public:
	Ch * pcur;
};

struct out_string_wrapper final {
public:
	typedef char Ch;
public:
	out_string_wrapper(std::string & out)
	: m_str(out)
	{
	}

	void Put(Ch c) { m_str.push_back(c); }
	void Flush() {}
private:
	std::string & m_str;
};
