#pragma once
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using  boost::asio::ip::tcp;

#include "../Common/GameMessage.h"

class game_client
{
public:
    game_client(io_service& io_service, tcp::resolver::iterator endpoint_iterator)
        : io_service_(io_service)
        , socket_(io_service)
    {
        async_connect(socket_, endpoint_iterator,
            std::bind(&game_client::handle_connect, this,
            std::placeholders::_1));
    }

    void write(const game_message& msg)
    {
        io_service_.post(std::bind(&game_client::do_write, this, msg));
    }

    void close()
    {
        io_service_.post(std::bind(&game_client::do_close, this));
    }

private:

    void handle_connect(const boost::system::error_code& error)
    {
        if (!error)
        {
            async_read(socket_,
				boost::asio::buffer(read_msg_.data(), game_message::header_length),
                std::bind(&game_client::handle_read_header, this,
                std::placeholders::_1));
        }
    }

    void handle_read_header(const boost::system::error_code& error)
    {
        if (!error && read_msg_.decode_header())
        {
            async_read(socket_,
                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                std::bind(&game_client::handle_read_body, this,
                std::placeholders::_1));
        }
        else
        {
            do_close();
        }
    }

    void handle_read_body(const boost::system::error_code& error)
    {
        if (!error)
        {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            async_read(socket_,
                boost::asio::buffer(read_msg_.data(), game_message::header_length),
                std::bind(&game_client::handle_read_header, this,
                std::placeholders::_1));
        }
        else
        {
            do_close();
        }
    }

    void do_write(game_message msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            async_write(socket_,
                boost::asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
                std::bind(&game_client::handle_write, this,
                std::placeholders::_1));
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
                    boost::asio::buffer(write_msgs_.front().data(),
                    write_msgs_.front().length()),
                    std::bind(&game_client::handle_write, this,
                    std::placeholders::_1));
            }
        }
        else
        {
            do_close();
        }
    }

    void do_close()
    {
        socket_.close();
    }

private:
    io_service&       io_service_;
    tcp::socket             socket_;
    game_message            read_msg_;
    game_message_queue      write_msgs_;
};
