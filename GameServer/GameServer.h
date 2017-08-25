#pragma once
#include <boost/asio.hpp>
#include "GameRoom.h"
#include "GameSession.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

class game_server
{
public:
	game_server(io_service& ioservice,
		const tcp::endpoint& endpoint)
		: io_service_(ioservice),
		acceptor_(ioservice, endpoint)
	{
		start_accept();
	}

	void start_accept()
	{
		game_session_ptr new_session(new game_session(io_service_, room_));
		acceptor_.async_accept(new_session->socket(),
			std::bind(&game_server::handle_accept, this, new_session,
				std::placeholders::_1));
	}

	void handle_accept(game_session_ptr session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			session->start();
		}

		start_accept();
	}

private:
	io_service& io_service_;
	tcp::acceptor acceptor_;
	game_room room_;

};

typedef std::shared_ptr<game_server> game_server_ptr;