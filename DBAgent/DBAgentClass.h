#pragma once

class DBAgentClass
{
private:
	void serverProcess();

public:
	DBAgentClass();
	~DBAgentClass();

	HRESULT init();
	void release();

	void run();
};
