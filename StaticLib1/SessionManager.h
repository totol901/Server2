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

private:
	SessionList		            sessionList_;
	int							sessionCount_;
	int							maxConnection_;
	Lock						lock_;
	Lock						seedLock_;

	oid_t						sessionSeed_;			// ���� �޴������� �����ϴ� �õ�

	// ���� ���� ��ɾ�
	typedef std::function<void(SessionList *sessionList, wstr_t *arg)> cmdFunc;
	std::unordered_map<wstr_t, cmdFunc>   serverCommand_;

public:
	HRESULT init(int maxConnection = SESSION_CAPACITY);
	void release();

	/****************************************************************************
	�Լ���	: createSessionId
	����		: sessionSeed_++�Ͽ� ��������
	���ϰ�	: oid_t
	�Ű�����	: ����
	*****************************************************************************/
	oid_t				createSessionId();

	/****************************************************************************
	�Լ���	: addSession
	����		: session�� �߰����� ������ ������ �����ϸ� false
	���ϰ�	: bool
	�Ű�����	: Session*
	*****************************************************************************/
	bool				addSession(Session *session);

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
	void				forceCloseSession(Session *session);

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

public:
	//get�Լ�
	const std::list<Session*>& sessionList() { return sessionList_; }
};
