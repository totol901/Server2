#pragma once

#define MAX_IOCP_THREAD		SIZE_64

class IOCPServer : public Server
{
private:
	SOCKET					listenSocket_;
	HANDLE					iocp_;
	Thread					*acceptThread_;
	std::array<Thread *, SIZE_64> workerThread_;
	Thread					*checkHeartBeatThread_;

private:
	//리슨 소켓 생성 및 리슨상태 만드는 함수
	bool					createListenSocket();

	/*/리슨 소켓으로 클라이언트를 서버에 접속시키는 함수
	static DWORD WINAPI		acceptThread(LPVOID serverPtr);
	*/

	//IOCP 이용하여 세션 처리함
	static DWORD WINAPI		workerThread(LPVOID serverPtr);

protected:
	//세션 종료시 해줘야할 부가적인 것들 처리 해주는 함수 포인터 (ex.유저 풀에서 유저 제거 등등)
	typedef std::function<void(Session* session)> closeSessionFuc;
	closeSessionFuc closeSessionFuc_;

public:
	IOCPServer(ContentsProcess *contentsProcess);
	~IOCPServer();

	void onAcceptEX(IoData* ioData);
	bool closeAndMakeNewAcceptSocket(AcceptData* accpetData);

	/****************************************************************************
	함수명	: run
	설명		: acceptThread, workerThread 생성, 비상 명령어 대기
	리턴값	: bool
	매개변수	: 없음
	*****************************************************************************/
	bool					run();

	/****************************************************************************
	함수명	: onAccept
	설명		: 클라이언트 접속시 세션 만들어줌, 세션과 IOCP 연동함
	리턴값	: 없음
	매개변수	: SOCKET, SOCKADDR_IN
	void					onAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
	*****************************************************************************/

public:
	//get함수
	const SOCKET&			listenSocket();
	HANDLE					iocp();

};
