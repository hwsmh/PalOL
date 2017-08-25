#pragma once
#include "GamePlayer.h"
#include "GameRoom.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

class game_session
	: public game_player
	, public std::enable_shared_from_this<game_session>
{
public:
	game_session(io_service& ioservice, game_room& room)
		: socket_(ioservice)
		, room_(room)
	{
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		room_.join(shared_from_this());
		async_read(socket_,
			buffer(read_msg_.data(), game_message::header_length),
			std::bind(&game_session::handle_read_header,
				shared_from_this(),
				std::placeholders::_1));
	}

	void deliver(const game_message& msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			async_write(socket_,
				buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
				std::bind(&game_session::handle_write, shared_from_this(),
					std::placeholders::_1));
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.decode_header())
		{
			async_read(socket_,
			buffer(read_msg_.body(), read_msg_.body_length()),
				std::bind(&game_session::handle_read_body, shared_from_this(),
					std::placeholders::_1));
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

	void handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			room_.deliver(read_msg_);
		async_read(socket_,
				buffer(read_msg_.data(), game_message::header_length),
				std::bind(&game_session::handle_read_header, shared_from_this(),
					std::placeholders::_1));
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				async_write(socket_,
					buffer(write_msgs_.front().data(),
						write_msgs_.front().length()),
					std::bind(&game_session::handle_write, shared_from_this(),
						std::placeholders::_1));
			}
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

private:
	tcp::socket             socket_;
	game_room&              room_;
	game_message            read_msg_;
	game_message_queue      write_msgs_;
};

typedef std::shared_ptr<game_session> game_session_ptr;
