#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "AuthSession.h"



namespace tcpserver
{
	using namespace boost::asio;
	using boost::asio::ip::tcp;

	class  auth_server
	{
	public:
		auth_server(io_service& io_service,const tcp::endpoint& endpoint)
			:io_service_(io_service),
			acceptor_(io_service_, endpoint)
		{
		}
	public:
		void start()
		{
			init_packet_callback();
			start_accept();
			io_service_.run();
		}

		void start_accept()
		{
			auth_session_ptr session(new auth_session(io_service_));
			acceptor_.async_accept(session->socket(),
				boost::bind(&auth_server::accept_handle,
				this, session,placeholders::error));
		}

		void accept_handle(auth_session_ptr session,const boost::system::error_code& error)
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
	};

}


#endif 


