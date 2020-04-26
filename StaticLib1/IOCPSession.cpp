#include "stdafx.h"
#include "IOCPSession.h"

//-----------------------------------------------------------------//
IOCPSession::IOCPSession()
	: Session()
{
	this->initialize();
}

void IOCPSession::initialize()
{
	ZeroMemory(&socketData_, sizeof(SOCKET_DATA));
	ioData_[IO_READ].setType(IO_READ);
	ioData_[IO_WRITE].setType(IO_WRITE);
}

void IOCPSession::checkErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR
		&& (::WSAGetLastError() != ERROR_IO_PENDING))
	{
		SLog(L"! socket error: %d", ::WSAGetLastError());
	}
}

void IOCPSession::recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes = 0;
	DWORD errorCode = ::WSARecv(socketData_.acceptData_->acceptSocket(), &wsaBuf, 1, &recvBytes, &flags, ioData_[IO_READ].overlapped(), NULL);
	this->checkErrorIO(errorCode);
}

bool IOCPSession::isRecving(size_t transferSize)
{
	if (ioData_[IO_READ].needMoreIO(transferSize))
	{
		this->recv(ioData_[IO_READ].wsabuf());
		return true;
	}
	return false;
}

void IOCPSession::recvStandBy()
{
	ioData_[IO_READ].clear();

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_READ].data();
	wsaBuf.len = SOCKET_BUF_SIZE;

	this->recv(wsaBuf);
}

void IOCPSession::send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = ::WSASend(socketData_.acceptData_->acceptSocket(),
		&wsaBuf, 1, &sendBytes, flags,
		ioData_[IO_WRITE].overlapped(), NULL);
	this->checkErrorIO(errorCode);
}

void IOCPSession::onSend(size_t transferSize)
{
	if (ioData_[IO_WRITE].needMoreIO(transferSize))
	{
		this->send(ioData_[IO_WRITE].wsabuf());
	}
}

void IOCPSession::sendPacket(Packet *packet)
{
	Stream stream;
	packet->encode(stream);
	if (!ioData_[IO_WRITE].setData(stream))
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_WRITE].data();
	wsaBuf.len = (ULONG)stream.size();

	this->send(wsaBuf);
	this->recvStandBy();
}

Package *IOCPSession::onRecv(size_t transferSize)
{
	packet_size_t offset = 0;
	offset += ioData_[IO_READ].setupTotalBytes();
	if (ioData_[IO_READ].totalByte() == 0
		&& transferSize != 0)
	{
		cout << "디버그 중 패킷 무시" << endl;
		return nullptr;
	}

	if (this->isRecving(transferSize))
	{
		return nullptr;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t packetDataSize = (packet_size_t)(ioData_[IO_READ].totalByte() - packetHeaderSize);
	Byte *packetData = (Byte*)ioData_[IO_READ].data() + offset;

	//PacketObfuscation::getInstance().decodingData(packetData, packetDataSize);
	Packet *packet = PacketAnalyzer::getInstance().analyzer((const char *)packetData, packetDataSize);
	if (packet == nullptr)
	{
		SLog(L"! invaild packet");
		this->onClose(true);
		return nullptr;
	}

	this->recvStandBy();

	Package *package = new Package(this, packet);
	return package;
}
