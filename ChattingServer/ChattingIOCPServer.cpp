#include "pch.h"
#include "ChattingIOCPServer.h"

ChattingIOCPServer::ChattingIOCPServer(ContentsProcess* contentsProcess)
	:IOCPServer(contentsProcess)
{
	closeSessionFuc_ = [](Session* session) 
	{
		User* user = USERMANAGER.at(session->id());
		if (user)
		{
			PK_I_DB_REQ_LOGOUT packet;
			packet.name_ = user->name();

			Terminal* terminal = TERMINALMANAGER.get(L"DBAgent");
			terminal->sendPacket(&packet);
		}
		
		USERMANAGER.remove(session->id());
	};
}

ChattingIOCPServer::~ChattingIOCPServer()
{
}
