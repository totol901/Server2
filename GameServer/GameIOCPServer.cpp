#include "stdafx.h"
#include "GameIOCPServer.h"

GameIOCPServer::GameIOCPServer(ContentsProcess* contentsProcess)
	:IOCPServer(contentsProcess)
{
	closeSessionFuc_ = [](Session* session)
	{
		USERMANAGER.remove(session->id());
	};
}

GameIOCPServer::~GameIOCPServer()
{
}
