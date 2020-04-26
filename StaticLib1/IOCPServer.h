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
	//���� ���� ���� �� �������� ����� �Լ�
	bool					createListenSocket();

	/*/���� �������� Ŭ���̾�Ʈ�� ������ ���ӽ�Ű�� �Լ�
	static DWORD WINAPI		acceptThread(LPVOID serverPtr);
	*/

	//IOCP �̿��Ͽ� ���� ó����
	static DWORD WINAPI		workerThread(LPVOID serverPtr);

protected:
	//���� ����� ������� �ΰ����� �͵� ó�� ���ִ� �Լ� ������ (ex.���� Ǯ���� ���� ���� ���)
	typedef std::function<void(Session* session)> closeSessionFuc;
	closeSessionFuc closeSessionFuc_;

public:
	IOCPServer(ContentsProcess *contentsProcess);
	~IOCPServer();

	void onAcceptEX(IoData* ioData);
	bool closeAndMakeNewAcceptSocket(AcceptData* accpetData);

	/****************************************************************************
	�Լ���	: run
	����		: acceptThread, workerThread ����, ��� ��ɾ� ���
	���ϰ�	: bool
	�Ű�����	: ����
	*****************************************************************************/
	bool					run();

	/****************************************************************************
	�Լ���	: onAccept
	����		: Ŭ���̾�Ʈ ���ӽ� ���� �������, ���ǰ� IOCP ������
	���ϰ�	: ����
	�Ű�����	: SOCKET, SOCKADDR_IN
	void					onAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
	*****************************************************************************/

public:
	//get�Լ�
	const SOCKET&			listenSocket();
	HANDLE					iocp();

};
