#include <iostream>
#include <vector>

#include <pqxx/connection>
#include <pqxx/nontransaction>
#include <pqxx/tablewriter>
#include <pqxx/transaction>

#include <string>
#include <pqxx/notification>

#include "jmsg/pqxxutils.hpp"
#include "jmsg/jmsg_rcver.hpp"
#include "jmsg/jmsg_sender.hpp"

#include "exemodel/poller.hpp"
#include "exemodel/evt_cb.hpp"

#define NOTIFICATION_TEST

#ifndef NOTIFICATION_TEST
template< typename _T >
static bool load_others(pqxx::nontransaction & w,
			rcver & rcv,
			const std::string name,
			_T & dst)
{
	pqxx::result r = w.exec("SELECT cfg FROM others WHERE name = '" + name + "'");
	for (auto row = r.begin(); row != r.end(); ++row) {
		for (auto cell = row->begin(); cell != row->end(); ++cell) {
			rcv.convert(dst, cell->c_str());
			return true;
		}
	}
	return false;
}

bool load_fsparam(pqxx::nontransaction & w,
			int index,
			fs_param_cfg_t & dst)
{
	std::cout << "prepare load fsparam!" << std::endl;
	pqxx::result r = w.prepared("load_fsparam")(index).exec();
	for (const auto & i : r) {
		std::cout << "pqxx2c fsparam!" << std::endl;
		pqxx2c(dst, i.begin());
		std::cout << "pqxx2c fsparam done!" << std::endl;
		return true;
	}
	return false;
}
bool save_fsparam(pqxx::nontransaction & w,
			int index,
			const fs_param_cfg_t & src)
{
	pqxx::result r = w.prepared("save_fsparam")(src)(index).exec();
	for (const auto & i : r) {
		///pqxx2c(dst, i.begin());
		return true;
	}
	return false;
}

static fs_param_cfg_t g_fsparam;
static misc_cfg_t g_misccfg;

static void test_fsparam()
{
	pqxx::connection c("host=127.0.0.1 user=postgres dbname=postgres");
	c.prepare("load_fsparam", "SELECT " PQKL_fs_param_cfg " FROM fs_param WHERE seqn=$1");
	c.prepare("save_fsparam",
		"UPDATE fs_param SET (" PQKL_fs_param_cfg ")=(" PQOL_fs_param_cfg ")"
		" WHERE seqn=$" PQMS_fs_param_cfg "RETURNING " PQKL_fs_param_cfg);
	pqxx::nontransaction w(c, "misc");
	rcver rcv;
	load_others(w, rcv, "misc", g_misccfg);
	std::cout << "fsparamidx: " << g_misccfg.fsParamIdx << std::endl;
	g_misccfg.fsParamIdx = 1;
	load_fsparam(w, g_misccfg.fsParamIdx, g_fsparam);
	std::cout << "fsparamleftfibertype: " << (int)g_fsparam.lfti << std::endl;
	std::cout << "fsparamrightfibertype: " << (int)g_fsparam.rfti << std::endl;
	//save_fsparam(w, g_misccfg.fsParamIdx, g_fsparam);
}

//using namespace PGSTD;
//using namespace pqxx;

// Simple test program for libpqxx.  Write a predetermined data set to a table
// using a tablewriter.  This data will be used by subsequent tests.  Any data
// previously in the table will be deleted.
//
// Usage: test005 [connect-string] [tablename]
//
// Where connect-string is a set of connection options in Postgresql's
// PQconnectdb() format, eg. "dbname=template1" to select from a database
// called template1, or "host=foo.bar.net user=smith" to connect to a
// backend running on host foo.bar.net, logging in as user smith.
//
// The tablename argument determines which table the data will be written to.
// If none is given, it defaults to "pqxxorgevents".
#else

class receiver : public pqxx::notification_receiver {
public:
	receiver(pqxx::connection_base & c, const std::string & channel) :
	pqxx::notification_receiver(c, channel),
	receiver_id(c.backendpid()) {}

	~receiver() {}

	virtual void operator()(const std::string & payload, int backend) {
		std::cout << "payload: " << payload << std::endl;
		std::cout << "backend: " << backend << std::endl;
		std::cout << "backendpid: " << receiver_id << std::endl;
	}

	pqxx::connection_base & conn_base() const { return this->conn(); }

private:
	int receiver_id;
};

class receiver_test : public exemodel::pollee, public exemodel::evt_cb<exemodel::poller&> {
public:
	explicit receiver_test(pqxx::connection_base & c, const std::string & ch) :
	exemodel::pollee(c.sock(), (uint32_t)(::EPOLLIN), "receiver_test"),
	m_rcvr(c, ch) {}

	~receiver_test() {}

	virtual void dispose(exemodel::poller & p, uint32_t) {
		this->exe(p);
	}

	const receiver & rcvr() const { return m_rcvr; }

private:
	receiver m_rcvr;
};

class rcvr_test : public exemodel::poller {
public:
	explicit rcvr_test(pqxx::connection_base & c, const std::string & ch) :
	exemodel::poller(),
	m_rcvr_test(c, ch) {
		m_rcvr_test.connect([this](exemodel::poller&) {
			int backend_id = m_rcvr_test.rcvr().conn_base().get_notifs();
			std::cout << "get notifs return value is: " << backend_id << std::endl;
		});
		this->add(m_rcvr_test);
	}
	~rcvr_test() {
		this->del(m_rcvr_test);
	}
private:
	receiver_test m_rcvr_test;
};

void receiver_test_func(const std::string & ch) {
	pqxx::connection c("host=127.0.0.1 user=postgres dbname=postgres");
	rcvr_test rtest(c, ch);
	rtest.run();
}

#endif

int main(int argc, char *argv[])
{
#ifndef NOTIFICATION_TEST
	test_fsparam();
  try
  {
    // Set up a connection to the backend
	  static char opt[] = "host=127.0.0.1 user=postgres dbname=fsdb";
    pqxx::connection C(opt);

    std::string TableName((argc > 2) ? argv[2] : "tbl_test2");

    // First create a separate transaction to drop old table, if any.  This may
    // fail if the table didn't previously exist.
    std::cout << "Dropping old " << TableName << std::endl;
    try
    {
      pqxx::work Drop(C, "drop_" + TableName);
      Drop.exec("DROP TABLE " + TableName);
      Drop.commit();
    }
    catch (const pqxx::undefined_table &e)
    {
      std::cout << "(Expected) Couldn't drop table: " << e.what() << std::endl
	   << "Query was: " << e.query() << std::endl;
    }
    catch (const pqxx::sql_error &e)
    {
      std::cerr << "Couldn't drop table: " << e.what() << std::endl
	   << "Query was: " << e.query() << std::endl;
    }

    // Now begin new transaction to create new table & write data
    pqxx::work T(C, "test5");

    T.exec("CREATE TABLE " +
	    TableName +
	    "(year INTEGER, event VARCHAR)");

    pqxx::tablewriter W(T, TableName);

    // TODO: Move this stuff to a file!
    const char *const CData[][2] =
    {
      {   "71", "jtv" },
      {   "38", "time_t overflow" },
      {    "1", "'911' WTC\"\" attack" },
      {   "81", "C:\\>" },
      { "1978", "bloody\t\tcold" },
      {   "99", "" },
      { "2002", "libpqxx" },
      { "1989", "Ode an die Freiheit" },
      { "2001", "New millennium" },
      { "1974", "fdsafdsa" },
      {   "97", "Asian crisis" },
      { "2001", "A Space Odyssey" },
      { nullptr, nullptr}
    };

    std::cout << "Writing data to " << TableName << std::endl;

    // Insert tuple of data using "begin" and "end" abstraction
    for (int i=0; CData[i][0]; ++i)
      W.insert(&CData[i][0], &CData[i][2]);

    // Insert tuple of data held in container
    std::vector<std::string> MoreData;
    MoreData.push_back("10");
    MoreData.push_back("Odyssey Two");
    W.insert(MoreData);

    // Now that MoreData has been inserted, we can get rid of the original
    // and use it for something else.  And this time, we use the insertion
    // operator.
    MoreData[0] = "3001";
    MoreData[1] = "Final Odyssey";
    W << MoreData;

    W.complete();

    // Now that our tablewriter is done, it's safe to commit T.
    T.commit();


	// Query
	/// @note: the work or transaction are all disposable, you can't reuse it after commit.
	C.prepare("queryXX", "SELECT $1 FROM ");
	pqxx::nontransaction NS(C);
	/**
	 * @note: if you use SELECT (year, event) ..., the result will contain only single column.
	 */
	pqxx::result r = NS.exec("SELECT year, event FROM " + TableName + " ");
	NS.commit();
	for (auto row = r.begin(); row != r.end(); ++row) {
		for (auto tbl_filed = row->begin();
			tbl_filed != row->end();
			++tbl_filed) {
			std::cout << tbl_filed->c_str() << ": " << '\t';
		}

		std::cout << std::endl;
	}
  }
  catch (const pqxx::sql_error &e)
  {
    // If we're interested in the text of a failed query, we can write separate
    // exception handling code for this type of exception
    std::cerr << "SQL error: " << e.what() << std::endl
         << "Query was: '" << e.query() << "'" << std::endl;
    return 1;
  }
  catch (const std::exception &e)
  {
    // All exceptions thrown by libpqxx are derived from std::exception
    std::cerr << "Exception: " << e.what() << std::endl;
    return 2;
  }
  catch (...)
  {
    // This is really unexpected (see above)
    std::cerr << "Unhandled exception" << std::endl;
    return 100;
  }
#else
	(void)argc;
	(void )argv[0][0];
	receiver_test_func("changed");
#endif

  return 0;
}
