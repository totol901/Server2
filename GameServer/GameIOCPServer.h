#pragma once

class GameIOCPServer : public IOCPServer
{
public:
	GameIOCPServer(ContentsProcess* contentsProcess);
	~GameIOCPServer();
};
