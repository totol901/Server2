#include "stdafx.h"
#include "GameIOCPServer.h"

GameIOCPServer::GameIOCPServer(ContentsProcess* contentsProcess)
	:IOCPServer(contentsProcess)
{
	closeSessionFuc_ = [](Session* session)
	{
		PK_C_TANK_OUT packet;
		packet.oid_ = session->id();

		GAMEMANAGER.userMap()->eraseUser(USERMANAGER.at(session->id()));
		USERMANAGER.remove(session->id());

		USERMANAGER.sendMessageAllUser(&packet);
	};
}

GameIOCPServer::~GameIOCPServer()
{
}
