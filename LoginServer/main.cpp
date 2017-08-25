#include <iostream>
#include "TcpServer.h"
#include "MysqlConnectionPool.h"

int main(int argc,char** argv)
{
	using namespace tcpserver;

	io_service io_service_;
	tcp::endpoint endpoint(tcp::v4(), 8888);

	MysqlConnectionPool::getInstance().initializePool("tcp://127.0.0.1:3306", "root", "root", "select 0 from dual");
	std::shared_ptr<auth_server> server(std::make_shared<auth_server>(io_service_, endpoint));

	server->start();
	
	return 0;
}