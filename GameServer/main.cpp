#include <iostream>
#include "GameServer.h"


int main(int argc, char** argv)
{
	try
	{
		boost::asio::io_service io_service;
		tcp::endpoint endpoint(tcp::v4(), 6666);
		game_server_ptr server(new game_server(io_service, endpoint));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

	return 0;
}