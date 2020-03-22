#include "stdafx.h"
#include "WinSocket.h"

WinSocket::WinSocket()
{
}

WinSocket::~WinSocket()
{
}

HRESULT WinSocket::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa_) != 0)
	{
		printf("! wsa startup fail\n");
		return E_FAIL;
	}

	printf("### WSA 2.2 set complet.\n");

	return S_OK;
}

void WinSocket::release()
{
	WSACleanup();
}
