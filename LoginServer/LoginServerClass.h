#pragma once

class LoginServerClass
{
private:
	void serverProcess();

public:
	LoginServerClass();
	~LoginServerClass();

	HRESULT init();
	void release();

	void run();
};
