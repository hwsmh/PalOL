#ifndef _MYSQL_CONNECTION_POOL_H
#define _MYSQL_CONNECTION_POOL_H

#include <iostream>
#include <boost/unordered_map.hpp>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn//exception.h>
#include <boost/thread.hpp>

typedef struct  
{
	sql::Connection* conn;
	bool locked;
}connection_;



class MysqlConnectionPool
{
public:
	static MysqlConnectionPool &getInstance()
	{
		static MysqlConnectionPool pool;
		return pool;
	}
public:
	void initializePool(std::string host,std::string username,
		std::string passward,std::string keep_alive);

	void startConnections(int number);

	connection_ createConnection();
	void releaseConnection(sql::Connection* conn);
	bool pingConnection(sql::Connection *conn);
	void pingPool();
	sql::Connection* getConnection();
private:
	MysqlConnectionPool()
	{
	}
private:
	std::string username_;
	std::string password_;
	std::string host_;
	std::string keep_alive_;
	bool loop_;
	sql::Driver* driver_;
	int connection_number_;
	boost::unordered_map<short, connection_> pool_;
	boost::mutex lock_;
	boost::shared_ptr<boost::thread> pingThread_;
};

#endif // !_MYSQL_CONNECTION_POOL_H
