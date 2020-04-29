#include "stdafx.h"
#include "SessionManager.h"

SessionManager::SessionManager()
	: lock_(L"SessionManager"),
	seedLock_(L"SessionManagerSeedLock"),
	sessionSeed_(1)
{
	this->commandFuncInitialize();
	
}

SessionManager::~SessionManager()
{
	this->release();
}

bool SessionManager::makeAcceptPool(SOCKET listenSocket)
{
	for (int i = 0; i < maxConnection(); i++)
	{
		if (!makeAcceptDataIntoPool(listenSocket))
		{
			return false;
		}
	}

	return true;
}

bool SessionManager::makeAcceptDataIntoPool(SOCKET listenSocket)
{
	int retval = 0;
	AcceptData* accpetData = new AcceptData();
	SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	//소켓 풀에 넣어줌
	acceptDataPool_.push_back(accpetData);

	//AcceptEX
	accpetData->setAcceptSocket(acceptSocket);
	retval = ::AcceptEx(listenSocket, accpetData->acceptSocket(), accpetData->data(),
		SOCKET_BUF_SIZE - ((sizeof(sockaddr_in) + 16) * 2),
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
		(LPDWORD)(&accpetData->totalByte()),
		accpetData->overlapped());
	if (retval == FALSE)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			SLog(L"! AcceptEx failed with error: %u", ::WSAGetLastError());

			return false;
		}
	}

	return true;
}

void SessionManager::eraseAcceptData(AcceptData* acceptData)
{
	SAFE_LOCK(lock_);
	for (auto iter = acceptDataPool_.begin(); iter != acceptDataPool_.end();)
	{
		if (*iter == acceptData)
		{
			acceptDataPool_.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
}

HRESULT SessionManager::init(int maxConnection)
{
	maxConnection_ = maxConnection;

	return S_OK;
}

void SessionManager::release()
{
	SAFE_LOCK(lock_);
	for (auto session : sessionMap_)
	{
		session.second->onClose();
	}

	for (auto iter : closedSessionList_)
	{
		SAFE_DELETE((iter));
	}

	for (auto acceptData : acceptDataPool_)
	{
		::closesocket(acceptData->acceptSocket());
		SAFE_DELETE(acceptData);
	}
}

Session* SessionManager::findClosedSession()
{
	SAFE_LOCK(lock_);
	for (auto session : closedSessionList_)
	{
		if (session->IsClosed() && session->referenceCount() == 0)
		{
			return session;
		}
	}

	return nullptr;
}

oid_t SessionManager::createSessionId()
{
	SAFE_LOCK(seedLock_);
	oid_t temp = sessionSeed_++;

	return temp;
}

bool SessionManager::eraseClosedSessionAndAddSession(Session *session)
{
	SAFE_LOCK(lock_);

	if (!eraseClosedSession(session))
	{
		return false;
	}

	auto findSession = sessionMap_.find(session->id());
	if (findSession != sessionMap_.end())
	{
		SLog(L"* addSessionMap already exist. id[%d]", session->id());
		return false;
	}

	sessionMap_.insert(std::pair<oid_t, Session*>(session->id(), session));

	return true;
}

//소켓을 닫으라는 클라이언트에게 보냅니다.
bool SessionManager::closeSession(Session *session)
{
	SAFE_LOCK(lock_);

	IOCPSession* IocpSession = (IOCPSession*)session;
	if (session == nullptr)
	{
		return false;
	}

	::shutdown(IocpSession->socket(), SD_BOTH);

	ZeroMemory(IocpSession->socketData().acceptData_->overlapped(), 
		sizeof(*IocpSession->socketData().acceptData_->overlapped()));

	::TransmitFile(IocpSession->socket(), NULL, NULL, NULL,
		(LPOVERLAPPED)IocpSession->socketData().acceptData_->overlapped(), 
		NULL, TF_DISCONNECT | TF_REUSE_SOCKET);

	//SLog(L"* server accpet socket failed!");

	auto findSession = sessionMap_.find(session->id());
	if (findSession != sessionMap_.end())
	{
		Session *delSession = findSession->second;
		SLog(L"* detected close by client [%s]", delSession->clientAddress().c_str());

		sessionMap_.erase(delSession->id());
		--sessionCount_;

		closedSessionList_.push_back(delSession);
		
		return true;
	}

	return false;
}

//소켓을 강제로 닫습니다.
void SessionManager::forceCloseSession(SOCKET listenSocket, Session *session)
{
	SAFE_LOCK(lock_);
	IOCPSession* iocpSession = (IOCPSession*)session;
	if (!session)
	{
		return;
	}

	//graceful close 유도. 클라이언트에서 서버 접속을 종료하도록 유도해야 한다.
	LINGER linger;
	linger.l_onoff = 1;   //사용
	linger.l_linger = 0;  //대기시간, 0일시 완료 안된 패킷 버리고 즉시 종료.

	if (session->socketData().acceptData_ != NULL)
	{
		::setsockopt(session->socket(), SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

		::closesocket(session->socket());
		
		SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
		ZeroMemory(session->socketData().acceptData_->overlapped(),
			sizeof(*session->socketData().acceptData_->overlapped()));

		//AcceptEX
		session->socketData().acceptData_->setAcceptSocket(acceptSocket);
		int retval = ::AcceptEx(listenSocket, session->socketData().acceptData_->acceptSocket(), 
			session->socketData().acceptData_->data(),
			SOCKET_BUF_SIZE - ((sizeof(sockaddr_in) + 16) * 2),
			sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
			(LPDWORD)(&session->socketData().acceptData_->totalByte()),
			session->socketData().acceptData_->overlapped());
		if (retval != TRUE)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				SLog(L"! AcceptEx failed with error: %u", ::WSAGetLastError());

				return;
			}
		}
	}
	
	SLog(L"* server forceCloseSession success!");

	return ;
}

Session* SessionManager::session(oid_t id)
{
	SAFE_LOCK(lock_);
	auto findSession = sessionMap_.find(id);
	if (findSession != sessionMap_.end())
	{
		return findSession->second;
	}

	return nullptr;
}

void SessionManager::runCommand(wstr_t cmdLine)
{
	std::size_t found = cmdLine.find(L' ');
	wstr_t command;
	wstr_t arg;
	if (found != wstr_t::npos)
	{
		command = cmdLine.substr(0, found);
		arg = cmdLine.substr(found);
	}
	else
	{
		command = cmdLine;
	}

	auto itr = serverCommand_.find(command);
	if (itr == serverCommand_.end())
	{
		return;
	}

	auto cmdFunc = serverCommand_.at(command);
	if (cmdFunc)
	{
		cmdFunc(&sessionMap_, &arg);
	}
}

// 서버에서 내리는 치트키 정의
void SessionManager::commandFuncInitialize()
{
#if 0
	//기본적인 3개만 생성, 이후 늘어나면 별도 클래스로 분리
	auto notiyFunc = [](SessionList *sessionList, wstr_t *arg)
	{
		auto eachFunc = [arg](void *atom)
		{
			Session *session = (Session*)atom;
			if (session->type() == SESSION_TYPE_TERMINAL)
			{
				return;
			}
			array<CHAR, SIZE_256> tmpBuf;
			StrConvW2A((WCHAR*)arg->c_str(), tmpBuf.data(), tmpBuf.size());

			PK_S_ANS_CHATTING retPacket;
			retPacket.id_ = "Server";
			retPacket.text_ = "* Notity : ";
			retPacket.text_ += tmpBuf.data();

			session->sendPacket(&retPacket);
		};

		for (auto session : *sessionList)
		{
			eachFunc(session);
		}
	};

	auto kickoffFunc = [](SessionList *sessionList, wstr_t *arg)
	{
		vector<Session *> removeSessionVec;
		auto eachFunc = [&removeSessionVec, arg](void *atom)
		{
			Session *session = (Session*)atom;
			if (session->type() == SESSION_TYPE_TERMINAL)
			{
				return;
			}

			PK_S_ANS_CHATTING retPacket;
			retPacket.id_ = "Server";
			retPacket.text_ = "! Kick off by Server";
			session->sendPacket(&retPacket);

			removeSessionVec.push_back(session);
		};
		for (auto session : *sessionList)
		{
			eachFunc(session);
		}

		for (auto session : removeSessionVec)
		{
			session->onClose();
		}
	};

	auto exitFunc = [](SessionList *sessionList, wstr_t *arg)
	{
		_shutdown = true;
	};

	//명령어 등록
	serverCommand_.insert(make_pair(L"/notify", notiyFunc));
	serverCommand_.insert(make_pair(L"/kickoff", kickoffFunc));
	serverCommand_.insert(make_pair(L"/exit", exitFunc));
#endif
}

bool SessionManager::addClosedSession(Session* session)
{
	SAFE_LOCK(lock_);
	auto findSession = std::find(closedSessionList_.begin(), closedSessionList_.end(), session);
	if (findSession != closedSessionList_.end())
	{
		return false;
	}
	if (sessionCount_ > maxConnection_)
	{
		SLog(L"* session so busy. count[%d]", sessionCount_);
		return false;
	}

	session->setId(this->createSessionId());
	closedSessionList_.push_back(session);
	++sessionCount_;
	return true;
}

bool SessionManager::eraseClosedSession(Session* session)
{
	SAFE_LOCK(lock_);

	auto findSession = std::find(closedSessionList_.begin(), closedSessionList_.end(), session);
	if (findSession == closedSessionList_.end())
	{
		SLog(L"* ClosedSession can't erease. id[%d]", session->id());
		return false;
	}

	closedSessionList_.remove(*findSession);

	return true;
}

bool SessionManager::makeSessionPool()
{
	for (int i = 0; i < maxConnection(); i++)
	{
		IOCPSession* session = new IOCPSession();
		if (session == nullptr)
		{
			SLog(L"! accept session create fail");
			return false;
		}

		//닫긴 세션 풀에 넣어줌
		if (!addClosedSession(session))
		{
			SAFE_DELETE(session);
			return false;
		}
	}

	return true;
}

bool SessionManager::makePools(SOCKET listenSocket)
{
	if (!makeSessionPool())
	{
		return false;
	}
	if (!makeAcceptPool(listenSocket))
	{
		return false;
	}

	return true;
}
