#include "DatabaseQuery.h"
#include <boost/format.hpp>

DatabaseQuery::DatabaseQuery()
{
	conn_ = MysqlConnectionPool::getInstance().getConnection();
}

void DatabaseQuery::releaseConnection()
{
	MysqlConnectionPool::getInstance().releaseConnection(conn_);
}

sql::ResultSet* DatabaseQuery::loginAccount(std::string username,std::string password)
{
	sql::Statement* stmt = nullptr;
	sql::ResultSet* res = nullptr;

	stmt = conn_->createStatement();

	static boost::format fmt_query("SELECT * FROM auth where username='%s' and password = '%s'");
	res = stmt->executeQuery(boost::str(fmt_query % username % password).c_str());
	return res;
}