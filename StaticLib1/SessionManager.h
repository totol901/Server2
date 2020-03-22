#pragma once

#define SESSION_CAPACITY		(5000)

class SessionManager : public Singleton<SessionManager>
{
	friend Singleton;
private:
	SessionManager();
	~SessionManager();

	// 세션의 빈번한 추가삭제를 위해 list로 구성
	typedef std::list<Session*>		SessionList;

private:
	SessionList		            sessionList_;
	int							sessionCount_;
	int							maxConnection_;
	Lock						lock_;

	oid_t						sessionSeed_;			// 세션 메니져에서 관리하는 시드

	// 서버 수동 명령어
	typedef std::function<void(SessionList *sessionList, wstr_t *arg)> cmdFunc;
	std::unordered_map<wstr_t, cmdFunc>   serverCommand_;

public:
	HRESULT init(int maxConnection = SESSION_CAPACITY);
	void release();

	/****************************************************************************
	함수명	: createSessionId
	설명		: sessionSeed_++하여 리턴해줌
	리턴값	: oid_t
	매개변수	: 없음
	*****************************************************************************/
	oid_t				createSessionId();

	/****************************************************************************
	함수명	: addSession
	설명		: session을 추가해줌 동일한 세션이 존재하면 false
	리턴값	: bool
	매개변수	: Session*
	*****************************************************************************/
	bool				addSession(Session *session);

	/****************************************************************************
	함수명	: closeSession
	설명		: session을 제거해줌, 소캣을 닫으라고 클라이언트에게 요청
	리턴값	: bool
	매개변수	: Session*
	*****************************************************************************/
	bool				closeSession(Session *session);

	/****************************************************************************
	함수명	: forceCloseSession
	설명		: session을 제거해줌, 소캣을 강제로 닫음
	리턴값	: bool
	매개변수	: Session*
	*****************************************************************************/
	void				forceCloseSession(Session *session);

	/****************************************************************************
	함수명	: session
	설명		: id의 세션을 리턴해줌
	리턴값	: Session*
	매개변수	: oid_t
	*****************************************************************************/
	Session				*session(oid_t id);

	/****************************************************************************
	함수명	: runCommand
	설명		: cmd에 따라 서버 커맨드 처리
	리턴값	: 없음
	매개변수	: wstr_t
	*****************************************************************************/
	void                runCommand(wstr_t cmd);

	/****************************************************************************
	함수명	: commandFuncInitialize
	설명		: 커맨드 초기화(치트키)
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void                commandFuncInitialize();

public:
	//get함수
	const std::list<Session*>& sessionList() { return sessionList_; }
};
