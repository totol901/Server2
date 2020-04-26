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

	//억센트 소켓은 리슨 소켓의 속성을 상속 받음
	::setsockopt(accpetSocket,
		SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
		(char*)&this->listenSocket(),
		sizeof(this->listenSocket()));

	//닫긴 세션 있는지 확인
	Session* closedSession = SESSIONMANAGER.findClosedSession();
	
	//풀방이거나 실패했다 종료 시키고 다시 ㄱㄱ
	if (closedSession == nullptr)
	{
		//accpetData, premote이용해서 소켓 연동과 데이터 업데이트 해줌
		if (!closedSession->onAccept(*accpetData, *premote))
		{
			//소켓 닫고 다시 만들어준다
			if (!closeAndMakeNewAcceptSocket(accpetData))
			{
				return;
			}
		}
	}

	//클로즈세션 리스트에서 지워주고 세션맵에 추가해줌
	SESSIONMANAGER.eraseClosedSessionAndAddSession(closedSession);

	//클로즈 세션 있으니 처리해줌
	((IOCPSession*)closedSession)->ioData_[IO_READ].clear();

	//해당 세션과 새로운 소켓 iocp에 연결해줌
	HANDLE handle = ::CreateIoCompletionPort((HANDLE)closedSession->socket(),
		this->iocp(), (ULONG_PTR) & (*closedSession), NULL);
	if (!handle)
	{
		SLog(L"! AcceptExSocket iocp error");
	}
	
	//recv 대기
	((IOCPSession*)closedSession)->recvStandBy();
}

bool IOCPServer::closeAndMakeNewAcceptSocket(AcceptData* accpetData)
{
	//서버에서 강제로 먼저 TransmitFile로 소켓 닫을경우 
	//2분간 대기상태에 놓이므로 없애고 그냥 새로 만듬
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

	//acceptex사용을 위해 리슨소켓 iocp에 연결함
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

	//바인드 안된 세션, accpet 데이터 풀 미리 생성
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

		//세션의 레퍼런스 카운트 +1 [세션 삭제시 사용중이면 삭제 못하게 하위 위함]
		session->IncreseReferenceCount();
		if (!ret)
		{
			//GetQueuedCompletionStatus 에러 받아옴
			int error = ::WSAGetLastError();

			//하트비트 처리안된 클라이언트 처리해야함
			SLog(L"GetQueuedCompletionStatus Error: %d\n", error);
			switch (error)
			{
			case ERROR_SEM_TIMEOUT:
			case ERROR_NETNAME_DELETED:
			case WSAECONNRESET: //keeplive 옵션 줬을때 정해놓은 시간 이상 통신 불가면 나는 애러
				SLog(L"* close by heartBeat[%d][%s]", session->id(), session->clientAddress().c_str());
				//세션 종료시 부가적인 행동들 처리!
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

		//클라이언트에서 graceful close 실시함
		if (transferSize == 0)
		{
			SLog(L"* close by client[%d][%s]", session->id(), session->clientAddress().c_str());
			//세션 종료시 부가적인 행동들 처리!
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
