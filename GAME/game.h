#pragma once
#include <boost/asio.hpp>
#include <thread>
#include "game_client.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

class game
{
public:
	game(io_service& ioservice,tcp::resolver::iterator endpoint_iterator)
		:io_service_(ioservice)
	{
		client_ = std::make_shared<game_client>(ioservice, endpoint_iterator);
	}

	void connect_server()
	{
		std::thread t([&]
		{
			io_service_.run();
		});

		t.detach();
	}

	void disconnect()
	{
		client_->close();
	}
private:
	io_service& io_service_;

	std::shared_ptr<game_client> client_;
};