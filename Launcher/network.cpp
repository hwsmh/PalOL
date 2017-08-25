#include "stdafx.h"
#include "network.h"
#include <iostream>
#include "Launcher.h"


namespace tcpclient
{
	std::unordered_map<PACKET_CMD_TYPE, packet_handler> m_handler_packet;

	bool on_tok_nothing(tcp::socket&& _socket, void* _packet, size_t _packet_size, int& status)
	{
		return false;
	}

	bool on_tok_error(tcp::socket&& _socket, void* _packet, size_t _packet_size, int& status)
	{
		
		
		const char* errortext = (const char*)((PTOKEN_ERROR)_packet)->error;
		theApp.m_pMainWnd->MessageBox(errortext, "´íÎó", MB_OK);
		
	   return true;
	}

	bool on_tok_loginsuc(tcp::socket&& _socket, void* _packet, size_t _packet_size, int& status)
	{
		std::string cookie((const char*)((PTOKEN_LOGINSUC)_packet)->cookie);
		//theApp.m_pMainWnd->MessageBox(cookie.c_str(), "cookieÄÚÈÝ", MB_OK);
	
		theApp.m_pMainWnd->PostMessage(WM_START_GAME);
		return true;
	}

	void init_packet_callback()
	{
		for (int type = MinTok + 1; type < MaxTok; type++)
		{
			m_handler_packet[(PACKET_CMD_TYPE)type] = on_tok_nothing;
		}
		m_handler_packet[Tok_LoginSuc] = on_tok_loginsuc;
		m_handler_packet[Tok_Error] = on_tok_error;
	}


}