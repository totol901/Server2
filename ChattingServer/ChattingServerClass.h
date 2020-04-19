#pragma once

class ChattingServerClass
{
private:
	void serverProcess();

public:
	ChattingServerClass();
	~ChattingServerClass();

	HRESULT init();
	void release();

	void run();
};

