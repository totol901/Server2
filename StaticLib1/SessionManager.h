#pragma once

#define SESSION_CAPACITY		(5000)

class SessionManager : public Singleton<SessionManager>
{
	friend Singleton;
private:
	SessionManager();
	~SessionManager();

	// ������ ����� �߰������� ���� list�� ����
	typedef std::list<Session*>		SessionList;
	typedef std::unordered_map<oid_t, Session*> SessionMap;
	typedef std::vector<AcceptData*>		AcceptDataPool;

private:
	SessionMap		            sessionMap_;
	SessionList		            closedSessionList_;
	AcceptDataPool				acceptDataPool_;
	int							sessionCount_;
	int							maxConnection_;
	Lock						lock_;
	Lock						seedLock_;

	oid_t						sessionSeed_;			// ���� �޴������� �����ϴ� �õ�

	// ���� ���� ��ɾ�
	typedef std::function<void(SessionMap* sessionMap, wstr_t *arg)> cmdFunc;
	std::unordered_map<wstr_t, cmdFunc>   serverCommand_;

	bool addClosedSession(Session* session);
	bool eraseClosedSession(Session* session);
	bool makeSessionPool();

	/****************************************************************************
	�Լ���	: createSessionId
	����		: sessionSeed_++�Ͽ� ��������
	���ϰ�	: oid_t
	�Ű�����	: ����
	*****************************************************************************/
	oid_t				createSessionId();

public:
	HRESULT				init(int maxConnection = SESSION_CAPACITY);
	void				release();

	Session* findClosedSession();
	bool makeAcceptPool(SOCKET listenSocket);
	//std::vector<AcceptData*>& GetAcceptDataPool();

	bool makeAcceptDataIntoPool(SOCKET listenSocket);

	void eraseAcceptData(AcceptData* acceptData);
	
	/****************************************************************************
	�Լ���	: addSession
	����		: session�� �߰����� ������ ������ �����ϸ� false
	���ϰ�	: bool
	�Ű�����	: Session*
	*****************************************************************************/
	bool				eraseClosedSessionAndAddSession(Session *session);

	/****************************************************************************
	�Լ���	: closeSession
	����		: session�� ��������, ��Ĺ�� ������� Ŭ���̾�Ʈ���� ��û
	���ϰ�	: bool
	�Ű�����	: Session*
	*****************************************************************************/
	bool				closeSession(Session *session);

	/****************************************************************************
	�Լ���	: forceCloseSession
	����		: session�� ��������, ��Ĺ�� ������ ����
	���ϰ�	: bool
	�Ű�����	: Session*
	*****************************************************************************/
	void				forceCloseSession(SOCKET listenSocket, Session *session);

	/****************************************************************************
	�Լ���	: session
	����		: id�� ������ ��������
	���ϰ�	: Session*
	�Ű�����	: oid_t
	*****************************************************************************/
	Session				*session(oid_t id);

	/****************************************************************************
	�Լ���	: runCommand
	����		: cmd�� ���� ���� Ŀ�ǵ� ó��
	���ϰ�	: ����
	�Ű�����	: wstr_t
	*****************************************************************************/
	void                runCommand(wstr_t cmd);

	/****************************************************************************
	�Լ���	: commandFuncInitialize
	����		: Ŀ�ǵ� �ʱ�ȭ(ġƮŰ)
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void                commandFuncInitialize();

	bool makePools(SOCKET listenSocket);

public:
	//get�Լ�
	const int&					maxConnection() { return maxConnection_; }
	//const std::unordered_map<oid_t, Session*>&	sessionMap() { return sessionMap_; }
	//const std::list<Session*>&	closedSessionList() { return closedSessionList_; }
};
