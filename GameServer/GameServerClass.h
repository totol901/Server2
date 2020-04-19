#pragma once

class GameServerClass
{
private:
	void serverProcess();

public:
	GameServerClass();
	~GameServerClass();

	HRESULT init();
	void release();

	void run();
};
