#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <unordered_map>
#include "../Common/AuthCom.h"


namespace tcpclient
{
	using namespace boost::asio;
	using boost::asio::ip::tcp;

	using packet_handler = boost::function<bool(tcp::socket&&, void*, size_t, int& status)>;

	extern std::unordered_map<PACKET_CMD_TYPE, packet_handler> m_handler_packet;
	void init_packet_callback();
}
#endif