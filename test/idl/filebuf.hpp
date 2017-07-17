#pragma once
#include <fstream>

static const char * const s_prefix[] = {
	"",
	"\t",
	"\t\t",
	"\t\t\t",
	"\t\t\t\t",
	"\t\t\t\t\t",
	"\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t\t\t\t\t",
	"\t\t\t\t\t\t\t\t\t\t\t\t\t",
};

class filebuf {
public:
	filebuf(std::size_t N, const char * file_name, const char * header = nullptr)
	: m_cur(new char[N])
	, m_remain(N)
	, m_bp(m_cur)
	, m_N(m_remain)
	, m_file(file_name)
	, m_header(header)
	{
		m_cur[0] = '\0';
	}
	~filebuf()
	{
		delete [] m_bp;
	}
public:
	template<typename ... Types>
	void pf(std::size_t lvl, const char * format, Types ... rest)
	{
		__pf_prefix(lvl);
		int ret = std::snprintf(m_cur, m_remain, format, rest...);
		if (ret < 0) {
			throw "no enough space";
		}
		m_cur += ret;
		m_remain -= ret;
	}

	void output()
	{
		std::ofstream ofs(m_file);
		if (m_header) ofs << m_header;
		ofs << m_bp;
		//std::printf("%s\n", m_bp);
	}
private:
	void __pf_prefix(std::size_t lvl)
	{
		if (sizeof(s_prefix) / sizeof(s_prefix[0]) <= lvl) {
			throw "too deep indent!!!!";
		}

		int ret = std::snprintf(m_cur, m_remain, "%s", s_prefix[lvl]);
		if (ret < 0) {
			throw "no enough space";
		}
		m_cur += ret;
		m_remain -= ret;
	}
private:
	char * m_cur;
	std::size_t m_remain;
	char * const m_bp;
	std::size_t const m_N;

	const char * m_file;
	const char * m_header;
};

extern filebuf s_outf_types;
extern filebuf s_outf_id;
extern filebuf s_outf_converter;
extern filebuf s_outf_sender;
extern filebuf s_outf_rcver;
