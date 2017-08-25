#include <iostream>
#include "network.h"
#include "DatabaseQuery.h"

namespace tcpserver
{
	std::unordered_map<PACKET_CMD_TYPE, packet_handler> m_handler_packet;

	bool on_cmd_nothing(tcp::socket&& _socket, void* _packet, size_t _packet_size, int& status)
	{
		return false;
	}


	bool on_cmd_register(tcp::socket&& _socket, void* _packet, size_t _packet_size, int& status)
	{
		return false;
	}


	bool on_cmd_login(tcp::socket&& _socket, void* _packet, size_t _packet_size,int& status)
	{
		std::string usename((const char*)((CMD_LOGIN*)_packet)->username);
		std::string password((const char*)((CMD_LOGIN*)_packet)->password);
		std::cout << "��¼�˻�" << usename 
			      << "\n��¼����" << password << std::endl;

		DatabaseQuery data;
		sql::ResultSet* result = data.loginAccount(usename, password);

		if (result->rowsCount() == 0)
		{
			status = �˻������벻��ȷ;
			return false;
		}

		status = �˻����Ե�¼;
		return true;
	}

	void init_packet_callback()
	{
		for (int type = MinCmd + 1; type < MaxCmd; type++)
		{
			m_handler_packet[(PACKET_CMD_TYPE)type] = on_cmd_nothing;
		}

		m_handler_packet[Cmd_Register] = on_cmd_register;
		m_handler_packet[Cmd_Login] = on_cmd_login;
	}

	
}