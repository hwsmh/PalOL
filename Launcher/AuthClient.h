#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "network.h"
#include "Launcher.h"
#include "../Common/AuthCom.h"

namespace tcpclient
{
	using namespace boost::asio;
	using boost::asio::ip::tcp;

	class auth_client
	{
	public:
		auth_client(io_service& ioservice,tcp::resolver::iterator endpoint_iterator)
			:io_service_(ioservice),
			socket_(ioservice),
			packet_buffer_(buffer_)
		{
			init_packet_callback();
			boost::asio::async_connect(socket_, endpoint_iterator,
				bind(&auth_client::connect_handle,this,_1));
		
		}

		char* get_write_buffer()
		{
			return buffer_;
		}

		void connect_handle(const boost::system::error_code& error)
		{
			if (!error)
			{
				theApp.m_pMainWnd->PostMessage(WM_CONNECT_SUCCESS);
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
		
		void close()
		{
			io_service_.post(boost::bind(&auth_client::do_close,this));
		}

		void send()
		{
			io_service_.post(boost::bind(&auth_client::do_write,this));
		}

		void do_write()
		{
			boost::asio::async_write(socket_, boost::asio::buffer(buffer_),
				boost::bind(&auth_client::write_handle,this,boost::asio::placeholders::error));
		}
		void write_handle(const boost::system::error_code& error)
		{
			if (!error)
			{
				memset(buffer_, 0, sizeof(buffer_));
				async_read(socket_,
					boost::asio::buffer(buffer_),
					boost::bind(&auth_client::read_handle, this, _1, _2)
				);
			}
			else
			{
				do_close();
			}
		}

		void read_handle(const boost::system::error_code& error, size_t bytes_transferred)
		{
		   if (!error)
		   {
			
			   packet_buffer_ = buffer_;
		
			   auto packet = (PPACKET_CMD)(packet_buffer_);
			   auto toktype = packet->dwCmd;
			  
			   if (toktype <= MinTok || toktype >= MaxTok)
			   {
				   socket_.close();
				   return;
			   }
			   auto callback_func = m_handler_packet[(PACKET_CMD_TYPE)toktype];
			   int status = Ã»ÓÐ´íÎó;

			   bool b = callback_func(boost::forward<tcp::socket>(socket_),
				   packet_buffer_ + sizeof(PACKET_CMD), bytes_transferred - sizeof(PACKET_CMD), status);
		   }
		}

	private:
		io_service& io_service_;
		tcp::socket socket_;
		char buffer_[1024];
		char* packet_buffer_;
	};

}