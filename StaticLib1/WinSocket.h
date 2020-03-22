#pragma once

class WinSocket : public Singleton<WinSocket>
{
	friend Singleton;
private:
	WSADATA            wsa_;

public:
	WinSocket();
	~WinSocket();

	HRESULT init();
	void release();
};
