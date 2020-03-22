#include "stdafx.h"
#include "Session.h"

Session::Session()
	:type_(SESSION_TYPE_CLIENT)
{
	this->updateHeartBeat();
}

Session::~Session()
{
	this->onClose();
}

bool Session::setSocketOpt()
{
#ifdef linux
	int keepAlive = 1;
	int keepAliveIdle = 1;
	int keepAliveCnt = 3;
	int keepAliveInterval = 3;

	int ret;
	ret = ::setsockopt(socketData_.socket_, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPIDLE, &keepAliveIdle, sizeof(keepAliveIdle));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPCNT, &keepAliveCnt, sizeof(keepAliveCnt));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPINTVL, &keepAliveInterval, sizeof(keepAliveInterval));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
#else
	//좀비 소켓 채크를 위함
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;					// KEEPALIVE ON
	keepAliveSet.keepalivetime = 3000;		// Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;	// Resend if No-Reply (10회 반복)

	DWORD dwBytes;
	if (WSAIoctl(
		socketData_.socket_,	//입력 소켓
		SIO_KEEPALIVE_VALS,		//소켓이 살아있나 채크하는 소켓 옵션
		&keepAliveSet,			//소켓 채크 세팅 (인버퍼)
		sizeof(keepAliveSet),	//인버퍼 크기
		&returned,				//아웃 버퍼
		sizeof(returned),		//아웃 버퍼 크기
		&dwBytes,				//IOCP에서 리턴된 바이트크기
		NULL, NULL
	) != 0)
	{
		return false;
	}
#endif
	return true;
}

bool Session::onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo)
{
	socketData_.socket_ = socket;
	int addrLen;
	getpeername(socketData_.socket_, (struct sockaddr *)&socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->setSocketOpt())
	{
		return false;
	}
	return true;
}

void Session::onClose(bool force)
{
	if (force)
	{
		SESSIONMANAGER.forceCloseSession(this);
	}
	else
	{
		SESSIONMANAGER.closeSession(this);
	}
}

SOCKET& Session::socket()
{
	return socketData_.socket_;
}

wstr_t Session::clientAddress()
{
	std::array<char, SIZE_64> ip;
	::inet_ntop(AF_INET, &(socketData_.addrInfo_.sin_addr), ip.data(), ip.size());

	std::array<WCHAR, SIZE_64> wip;
	StrConvA2W(ip.data(), wip.data(), wip.max_size());
	wstr_t stringData = wip.data();

	return stringData;
}

oid_t Session::id()
{
	return id_;
}

void Session::setId(oid_t id)
{
	id_ = id;
}

int8_t Session::type()
{
	return type_;
}

void Session::setType(int8_t type)
{
	type_ = type;
}

tick_t Session::heartBeat()
{
	return lastHeartBeat_;
}

void Session::updateHeartBeat()
{
	lastHeartBeat_ = CLOCK.systemTick();
}
