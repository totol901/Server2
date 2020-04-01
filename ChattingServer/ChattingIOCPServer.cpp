#include "pch.h"
#include "ChattingIOCPServer.h"

ChattingIOCPServer::ChattingIOCPServer(ContentsProcess* contentsProcess)
	:IOCPServer(contentsProcess)
{
	closeSessionFuc_ = [](Session* session) 
	{
		USERMANAGER.remove(session->id());
	};
}

ChattingIOCPServer::~ChattingIOCPServer()
{
}
