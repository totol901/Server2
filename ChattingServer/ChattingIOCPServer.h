#pragma once

class ChattingIOCPServer : public IOCPServer
{
public:
	ChattingIOCPServer(ContentsProcess* contentsProcess);
	~ChattingIOCPServer();
};
