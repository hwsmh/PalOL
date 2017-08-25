#ifndef _AUTHSESSION_H_
#define _AUTHSESSION_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../Common/AuthCom.h"
#include "network.h"

namespace tcpserver
{
	using namespace boost::asio;
	using boost::asio::ip::tcp;

	class auth_session :public boost::enable_shared_from_this<auth_session>
	{
	public:
		auth_session(io_service& io_service)
			:socket_(io_service),
			packet_buffer_(buffer_)
		{
			
		}
		~auth_session()
		{
			std::cout << "客户端被析构了" << std::endl;
		}
	public:
		tcp::socket& socket()
		{
			return socket_;
		}

		void start()
		{
			memset(buffer_, 0, sizeof(buffer_));

			socket_.async_read_some(boost::asio::buffer(buffer_, sizeof(buffer_)),
				boost::bind(&auth_session::read_handle, shared_from_this(), 
					placeholders::error,
					placeholders::bytes_transferred));
		}

		const char* ErrorText(int status)
		{
			switch (status)
			{
			case  账户或密码不正确: return "账户或密码不正确";
			default:return "未知";
			}
		}

		void read_handle(const boost::system::error_code& error,size_t bytes_transferred)
		{
			if (!error)
			{
				packet_buffer_ = buffer_;
				auto packet = (PPACKET_CMD)(packet_buffer_);

				auto cmdtype = packet->dwCmd;
				if (cmdtype <= MinCmd || cmdtype >= MaxCmd)
				{
					socket_.close();
					return;
				}

				
				auto callback_func = m_handler_packet[(PACKET_CMD_TYPE)cmdtype];
				int status = 账户或密码不正确;

				bool b = callback_func(boost::forward<tcp::socket>(socket_),
					packet_buffer_ + sizeof(PACKET_CMD),bytes_transferred - sizeof(PACKET_CMD),status);

				memset(buffer_, 0, sizeof(buffer_));
				packet_buffer_ = buffer_;

				if (b)
				{
					((PPACKET_CMD)packet_buffer_)->dwCmd = Tok_LoginSuc;
					int size = ((PPACKET_CMD)packet_buffer_)->dwSize = sizeof(TOKEN_LOGINSUC);
					packet_buffer_ += sizeof(PACKET_CMD);
					strcpy((char*)((PTOKEN_LOGINSUC)packet_buffer_)->cookie,"11111");
				}
				else
				{
					const char* errortext = ErrorText(status);
					
					((PPACKET_CMD)packet_buffer_)->dwCmd = Tok_Error;
					((PPACKET_CMD)packet_buffer_)->dwSize = sizeof(TOKEN_ERROR);

					packet_buffer_ += sizeof(PACKET_CMD);

					strcpy((char*)((PTOKEN_ERROR)packet_buffer_)->error, errortext);
				}
				async_write(socket_, boost::asio::buffer(buffer_),
					boost::bind(&auth_session::write_handle, shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}


		void write_handle(const boost::system::error_code& error,size_t bytes_transferred)
		{
			if (!error)
			{
				memset(buffer_, 0, sizeof(buffer_));

				socket_.async_read_some(boost::asio::buffer(buffer_, sizeof(buffer_)),
					boost::bind(&auth_session::read_handle, shared_from_this(),
						placeholders::error,
						placeholders::bytes_transferred));
			}
		
		}
	private:
		tcp::socket socket_;
		char buffer_[1024];
		char* packet_buffer_;
	};

	typedef boost::shared_ptr<auth_session> auth_session_ptr;
}


#endif