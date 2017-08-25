#ifndef _DATABASE_QUERY_H
#define _DATABASE_QUERY_H

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "MysqlConnectionPool.h"


class DatabaseQuery
{
public:
	DatabaseQuery();
	void releaseConnection();
	sql::ResultSet* loginAccount(std::string username, std::string password);
private:
	sql::Connection *conn_;
};



#endif
