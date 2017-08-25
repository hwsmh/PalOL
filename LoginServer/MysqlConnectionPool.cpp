#include "MysqlConnectionPool.h"



void MysqlConnectionPool::initializePool(std::string host,std::string username,std::string password,std::string keep_alive)
{
	username_ = username;
	password_ = password;
	host_ = host;
	keep_alive_ = keep_alive;
	loop_ = true;

	try 
	{
		driver_ = get_driver_instance();
	}
	catch (sql::SQLException &e) 
	{
		std::cout << "SQL error " << e.getErrorCode() 
			      << " at " << __FUNCTION__
			      << "\nDescription :" << e.what() 
			      << std::endl;
	}

	startConnections(3); 

	pingThread_ = boost::make_shared<boost::thread>(boost::bind(&MysqlConnectionPool::pingPool, this));
}

void MysqlConnectionPool::startConnections(int number) 
{
	connection_number_ = number;
	for (int i = 0; i < number; i++) 
	{
		pool_[i] = createConnection();
	}
}

connection_ MysqlConnectionPool::createConnection() 
{
	connection_ connection_struct;
	try 
	{
		connection_struct.locked = false;
		connection_struct.conn = driver_->connect(host_, username_, password_);
		connection_struct.conn->setAutoCommit(false);
		connection_struct.conn->setSchema("palol");
		pingConnection(connection_struct.conn);
	}
	catch (sql::SQLException &e) 
	{
		std::cout << "SQL error " << e.getErrorCode()
			<< " at " << __FUNCTION__
			<< "\nDescription :" << e.what()
			<< std::endl;
	}

	return connection_struct;
}

void MysqlConnectionPool::releaseConnection(sql::Connection* conn)
{
	boost::lock_guard<boost::mutex> lock(lock_);

	for (int i = 0;i < connection_number_;i++)
	{
		if (pool_[i].conn == conn)
		{
			pool_[i].locked = false;
		}
	}
}

bool MysqlConnectionPool::pingConnection(sql::Connection *conn)
{
	try
	{
		sql::Statement* stmt = conn->createStatement();
		stmt->executeQuery(keep_alive_);
		delete stmt;
		return true;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL error " << e.getErrorCode()
			<< " at " << __FUNCTION__
			<< "\nDescription :" << e.what()
			<< std::endl;
		return false;
	}

}


void MysqlConnectionPool::pingPool()
{
	while(loop_)
	{
		boost::this_thread::sleep(boost::posix_time::minutes(5));

		boost::lock_guard<boost::mutex> lock(lock_);

		for (int i = 0;i < connection_number_;i++)
		{
			if (!pool_[i].locked)
			{
				pingConnection(pool_[i].conn);
			}
		}
	}
}

sql::Connection* MysqlConnectionPool::getConnection()
{
	lock_.lock();
	int i;
	for (i = 0;i < connection_number_;i++)
	{
		if (!pool_[i].locked)
		{
			pool_[i].locked = true;
			lock_.unlock();
			return pool_[i].conn;
		}
	}

	pool_[i] = createConnection();
	pool_[i].locked = true;
	connection_number_++;

	lock_.unlock();

	return pool_[i].conn;
}