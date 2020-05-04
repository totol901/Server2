#include "stdafx.h"
#include "Session.h"


IoData::IoData()
	:ioType_(IO_ERROR)
{
	ZeroMemory(&overlapped_, sizeof(overlapped_));

	this->clear();
}

void IoData::clear()
{
	buffer_.fill(0);
	totalBytes_ = 0;
	currentBytes_ = 0;
}

bool IoData::needMoreIO(size_t transferSize)
{
	currentBytes_ += transferSize;
	if (currentBytes_ < totalBytes_)
	{
		return true;
	}
	return false;
}

int32_t IoData::setupTotalBytes()
{
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };

	if (totalBytes_ == 0)
	{
		::memcpy_s((void*)packetLen, sizeof(packetLen), (void*)buffer_.data(), sizeof(packetLen));
		//PacketObfuscation::getInstance().decodingHeader((Byte*)&packetLen, sizeof(packetLen));

		totalBytes_ = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}

const size_t& IoData::totalByte()
{
	return totalBytes_;
}

const IO_OPERATION& IoData::type()
{
	return ioType_;
}

void IoData::setType(IO_OPERATION type)
{
	ioType_ = type;
}

char* IoData::data()
{
	return buffer_.data();
}

bool IoData::setData(Stream& stream)
{
	this->clear();

	if (buffer_.max_size() <= stream.size())
	{
		SLog(L"! packet size too big [%d]byte", stream.size());
		return false;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t offset = 0;

	char* buf = buffer_.data();
	//									 head size  + real data size
	packet_size_t packetLen[1] = { (packet_size_t)packetHeaderSize + (packet_size_t)stream.size(), };
	// insert packet len
	::memcpy_s(buf + offset, buffer_.max_size(), (void*)packetLen, packetHeaderSize);
	offset += packetHeaderSize;

	// packet obfuscation
	//PacketObfuscation::getInstance().encodingHeader((Byte*)buf, packetHeaderSize);
	//PacketObfuscation::getInstance().encodingData((Byte*)stream.data(), stream.size());

	// insert packet data
	::memcpy_s(buf + offset, buffer_.max_size(), stream.data(), (int32_t)stream.size());
	offset += (packet_size_t)stream.size();

	totalBytes_ = offset;
	return true;
}

LPWSAOVERLAPPED IoData::overlapped()
{
	return &overlapped_;
}

WSABUF IoData::wsabuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = buffer_.data() + currentBytes_;
	wsaBuf.len = (ULONG)(totalBytes_ - currentBytes_);

	return wsaBuf;
}

//-----------------------------------------------------------------//
void AcceptData::setAcceptSocket(SOCKET acceptSocket)
{
	acceptSocket_ = acceptSocket;
}

SOCKET AcceptData::acceptSocket()
{
	return acceptSocket_;
}

//-----------------------------------------------------------------//

Session::Session()
	:type_(SESSION_TYPE_CLIENT),
	lock_(L"Seesion"),
	referenceCount_(0),
	isClosed_(true)
{
	this->updateHeartBeat();
}

Session::Session(closeSessionFuc SessionFuc)
	:type_(SESSION_TYPE_CLIENT), 
	lock_(L"Seesion"),
	referenceCount_(0),
	isClosed_(true)
{
	closeSessionFuc_ = SessionFuc;
	this->updateHeartBeat();
}

Session::~Session()
{
}

void Session::IncreseReferenceCount()
{
	InterlockedIncrement(&referenceCount_);
	//SLog(L"* session referenceCount_ => [%d][%d][%s]", 
	//	referenceCount_,
	//	this->id(), 
	//	this->clientAddress().c_str());
}

void Session::DecreseReferenceCount()
{
	InterlockedDecrement(&referenceCount_);
	//SLog(L"* session referenceCount_ => [%d][%d][%s]",
	//	referenceCount_,
	//	this->id(),
	//	this->clientAddress().c_str());
}

bool Session::setSocketOpt()
{
	return setSocketOpt(socketData_.acceptData_->acceptSocket());
}

bool Session::setSocketOpt(SOCKET socket)
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
	if (::WSAIoctl(
		socket,	//입력 소켓
		SIO_KEEPALIVE_VALS,		//소켓이 살아있나 채크하는 소켓 옵션
		&keepAliveSet,			//소켓 채크 세팅 (인버퍼)
		sizeof(keepAliveSet),	//인버퍼 크기
		&returned,				//아웃 버퍼
		sizeof(returned),		//아웃 버퍼 크기
		&dwBytes,				//IOCP에서 리턴된 바이트크기
		NULL, NULL
	) != 0)
	{
		SLog(L"SocketOpt Error : %d", ::WSAGetLastError());

		return false;
	}
#endif
	return true;
}

bool Session::onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo)
{
	socketData_.acceptData_->setAcceptSocket(socket);
	int addrLen;
	::getpeername(socketData_.acceptData_->acceptSocket(), (struct sockaddr *)&socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->setSocketOpt())
	{
		return false;
	}

	return true;
}

bool Session::onAccept(const SOCKADDR_IN& addrInfo)
{
	int addrLen;
	::getpeername(socketData_.acceptData_->acceptSocket(), (struct sockaddr*) & socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->setSocketOpt())
	{
		return false;
	}

	return true;
}

bool Session::onAccept(AcceptData& accpetData, const SOCKADDR_IN& addrInfo)
{
	socketData_.acceptData_ = &accpetData;
	int addrLen;
	::getpeername(socketData_.acceptData_->acceptSocket(), (struct sockaddr*) & socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->setSocketOpt())
	{
		return false;
	}

	return true;
}

void Session::onClose(SOCKET listenSocket)
{
	if (listenSocket)
	{
		SESSIONMANAGER.forceCloseSession(listenSocket, this);
	}
	else
	{
		SESSIONMANAGER.closeSession(this);
	}
}

SOCKET_DATA& Session::socketData()
{
	return socketData_;
}

SOCKET Session::socket()
{
	return socketData_.acceptData_->acceptSocket();
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

void Session::setIsClosed(bool closed)
{
	isClosed_ = closed;
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

void Session::setCloseSessionFuc(closeSessionFuc func)
{
	closeSessionFuc_ = func;
}

tick_t Session::heartBeat()
{
	return lastHeartBeat_;
}

const bool& Session::IsClosed()
{
	return isClosed_;
}

const UINT& Session::referenceCount()
{
	return referenceCount_;
}

void Session::updateHeartBeat()
{
	lastHeartBeat_ = CLOCK.systemTick();
}

