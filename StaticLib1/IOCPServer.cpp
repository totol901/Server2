#include "stdafx.h"
#include "IOCPServer.h"

IOCPServer::IOCPServer(ContentsProcess *contentsProcess)
	:Server(contentsProcess),
	acceptThread_(nullptr),
	closeSessionFuc_(nullptr),
	checkHeartBeatThread_(nullptr)
{
}

IOCPServer::~IOCPServer()
{
	::CloseHandle(iocp_);
	::closesocket(listenSocket_);
}

void IOCPServer::onAcceptEX(IoData* ioData)
{
	AcceptData* accpetData = (AcceptData*)ioData;
	SOCKET accpetSocket = accpetData->acceptSocket();

	int locallen, remotelen = 0;
	sockaddr_in* plocal = nullptr, * premote = nullptr;

	::GetAcceptExSockaddrs(
		accpetData->data(),
		SOCKET_BUF_SIZE - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		(sockaddr**)&plocal,
		&locallen,
		(sockaddr**)&premote,
		&remotelen);

	//�＾Ʈ ������ ���� ������ �Ӽ��� ��� ����
	::setsockopt(accpetSocket,
		SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
		(char*)&this->listenSocket(),
		sizeof(this->listenSocket()));

	//�ݱ� ���� �ִ��� Ȯ��
	Session* closedSession = SESSIONMANAGER.findClosedSession();
	
	//Ǯ���̰ų� �����ߴ� ���� ��Ű�� �ٽ� ����
	if (closedSession == nullptr)
	{
		//accpetData, premote�̿��ؼ� ���� ������ ������ ������Ʈ ����
		if (!closedSession->onAccept(*accpetData, *premote))
		{
			//���� �ݰ� �ٽ� ������ش�
			if (!closeAndMakeNewAcceptSocket(accpetData))
			{
				return;
			}
		}
	}

	//Ŭ����� ����Ʈ���� �����ְ� ���Ǹʿ� �߰�����
	SESSIONMANAGER.eraseClosedSessionAndAddSession(closedSession);

	//Ŭ���� ���� ������ ó������
	((IOCPSession*)closedSession)->ioData_[IO_READ].clear();

	//�ش� ���ǰ� ���ο� ���� iocp�� ��������
	HANDLE handle = ::CreateIoCompletionPort((HANDLE)closedSession->socket(),
		this->iocp(), (ULONG_PTR) & (*closedSession), NULL);
	if (!handle)
	{
		SLog(L"! AcceptExSocket iocp error");
	}
	
	//recv ���
	((IOCPSession*)closedSession)->recvStandBy();
}

bool IOCPServer::closeAndMakeNewAcceptSocket(AcceptData* accpetData)
{
	//�������� ������ ���� TransmitFile�� ���� ������� 
	//2�а� �����¿� ���̹Ƿ� ���ְ� �׳� ���� ����
	::closesocket(accpetData->acceptSocket());
	SESSIONMANAGER.eraseAcceptData(accpetData);
	SAFE_DELETE(accpetData);

	if (!SESSIONMANAGER.makeAcceptDataIntoPool(listenSocket()))
	{
		return true;
	}

	SLog(L"* server accpet socket failed!");

	return false;
}

bool IOCPServer::createListenSocket()
{
	DWORD dwBytes = 0;
	
	listenSocket_ = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket_ == INVALID_SOCKET)
	{
		SErrLog(L"! listenSocket fail : %d", ::WSAGetLastError());
		return false;
	}

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons((u_short)port_);
	::inet_pton(AF_INET, ip_, &(serverAddr.sin_addr));

	//int reUseAddr = 1;
	//::setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, (char *)&reUseAddr, (int)sizeof(reUseAddr));

	int retval = ::bind(listenSocket_, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		SErrLog(L"! bind fail");
		return false;
	}

	//acceptex����� ���� �������� iocp�� ������
	HANDLE handle = ::CreateIoCompletionPort(
		(HANDLE)listenSocket_,
		this->iocp(),
		NULL, 
		NULL);

	const int BACK_SOCKETS = 5;
	retval = ::listen(listenSocket_, BACK_SOCKETS);
	if (retval == SOCKET_ERROR)
	{
		SErrLog(L"! listen fail");
		return false;
	}

	std::array<char, SIZE_64> ip;
	::inet_ntop(AF_INET, &(serverAddr.sin_addr), ip.data(), ip.size());
	SLog(L"* server listen socket created, ip: %S, port: %d", ip.data(), port_);

	//���ε� �ȵ� ����, accpet ������ Ǯ �̸� ����
	if (!SESSIONMANAGER.makePools(listenSocket_))
	{
		return false;
	}
	
	return true;
}

bool IOCPServer::run()
{
	if (MAX_IOCP_THREAD < workerThreadCount_)
	{
		SErrLog(L"! workerThread limit[%d], but config setting [%d]", MAX_IOCP_THREAD, workerThreadCount_);
		return false;
	}

	iocp_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, workerThreadCount_);
	if (iocp_ == nullptr)
	{
		return false;
	}
	this->createListenSocket();

	//acceptThread_ = MAKE_THREAD(IOCPServer, acceptThread);
	for (int i = 0; i < workerThreadCount_; ++i)
	{
		workerThread_[i] = MAKE_THREAD(IOCPServer, workerThread);
	}
	//checkHeartBeatThread_ = MAKE_THREAD(IOCPServer, checkHeartBeat);

	this->status_ = SERVER_READY;

	while (!_shutdown)
	{
		std::wstring cmdLine;
		std::getline(std::wcin, cmdLine);

		SLog(L"Input was: %s", cmdLine.c_str());
		SESSIONMANAGER.runCommand(cmdLine);
	}

	return true;
}

const SOCKET& IOCPServer::listenSocket()
{
	return listenSocket_;
}

HANDLE IOCPServer::iocp()
{
	return iocp_;
}

/*
void IOCPServer::onAccept(SOCKET accepter, SOCKADDR_IN addrInfo)
{
	IOCPSession *session = new IOCPSession();
	
	if (session == nullptr)
	{
		SLog(L"! accept session create fail");
		return;
	}
	if (!session->onAccept(accepter, addrInfo))
	{
		SAFE_DELETE(session);
		return;
	}
	if (!SESSIONMANAGER.addSession(session))
	{
		SAFE_DELETE(session);
		return;
	}
	session->ioData_[IO_READ].clear();

	HANDLE handle = ::CreateIoCompletionPort((HANDLE)accepter,
		this->iocp(), (ULONG_PTR)&(*session), NULL);
	if (!handle)
	{
		SAFE_DELETE(session);
		return;
	}

	SLog(L"* client accecpt from [%s]", session->clientAddress().c_str());
	session->recvStandBy();
}
*/

/*
DWORD WINAPI IOCPServer::acceptThread(LPVOID serverPtr)
{
	IOCPServer	*server = (IOCPServer *)serverPtr;

	while (!_shutdown)
	{
		SOCKET		acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAddr;
		static int addrLen = sizeof(recvAddr);
		acceptSocket = ::WSAAccept(server->listenSocket(),
			(struct sockaddr *)&recvAddr, &addrLen, NULL, 0);
		if (acceptSocket == SOCKET_ERROR)
		{
			if (!server->status() == SERVER_STOP)
			{
				SLog(L"! Accept fail");
				break;
			}
		}
		server->onAccept(acceptSocket, recvAddr);

		if (server->status() != SERVER_READY)
		{
			SLog(L"! Accept error not SERVER_READY");
			//break;
		}
	}

	return 0;
}
*/

DWORD WINAPI IOCPServer::workerThread(LPVOID serverPtr)
{
	IOCPServer *server = (IOCPServer*)serverPtr;

	while (!_shutdown)
	{
		IoData			*ioData = nullptr;
		IOCPSession		*session = nullptr;
		DWORD			transferSize;

		BOOL ret = ::GetQueuedCompletionStatus(server->iocp(),
			&transferSize, (PULONG_PTR)&session,
			(LPOVERLAPPED *)&ioData, INFINITE);

		if (ioData->type() == IO_ACCEPT)
		{
			server->onAcceptEX(ioData);

			continue;
		}

		//������ ���۷��� ī��Ʈ +1 [���� ������ ������̸� ���� ���ϰ� ���� ����]
		session->IncreseReferenceCount();
		if (!ret)
		{
			//GetQueuedCompletionStatus ���� �޾ƿ�
			int error = ::WSAGetLastError();

			//��Ʈ��Ʈ ó���ȵ� Ŭ���̾�Ʈ ó���ؾ���
			SLog(L"GetQueuedCompletionStatus Error: %d\n", error);
			switch (error)
			{
			case ERROR_SEM_TIMEOUT:
			case ERROR_NETNAME_DELETED:
			case WSAECONNRESET: //keeplive �ɼ� ������ ���س��� �ð� �̻� ��� �Ұ��� ���� �ַ�
				SLog(L"* close by heartBeat[%d][%s]", session->id(), session->clientAddress().c_str());
				//���� ����� �ΰ����� �ൿ�� ó��!
				if (server->closeSessionFuc_)
				{
					server->closeSessionFuc_(session);
				}

				session->onClose(server->listenSocket());
				break;

			default:
				break;
			}

			session->DecreseReferenceCount();
			continue;
		}

		if (session == nullptr)
		{
			SLog(L"! socket data broken");
			return 0;
		}

		//Ŭ���̾�Ʈ���� graceful close �ǽ���
		if (transferSize == 0)
		{
			SLog(L"* close by client[%d][%s]", session->id(), session->clientAddress().c_str());
			//���� ����� �ΰ����� �ൿ�� ó��!
			if (server->closeSessionFuc_)
			{
				server->closeSessionFuc_(session);
			}
			
			session->onClose();
			session->DecreseReferenceCount();

			continue;
		}

		switch (ioData->type())
		{
		case IO_WRITE:
			session->onSend((size_t)transferSize);
			session->DecreseReferenceCount();
			continue;

		case IO_READ:
		{
			Package *package = session->onRecv((size_t)transferSize);
			if (package != nullptr)
			{
				server->putPackage(package);
			}
			continue;
		}
		
		case IO_ERROR:
			SLog(L"* close by client error [%d][%s]", session->id(), session->clientAddress().c_str());
			session->onClose();
			session->DecreseReferenceCount();
			continue;
		}
	}

	return 0;
}
