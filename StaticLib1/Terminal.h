#pragma once
//왜 쓰는가?
//사용처는?
//구현 방식?

// 터미널 쓰레드, 타 서버와 연결해서 데이터 송수신에 사용한다.
enum TERMINAL_STATUS
{
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Terminal
{
protected:
	Server					*server_;
	wstr_t					name_;
	TERMINAL_STATUS			status_;

	char					ip_[16];
	int						port_;

	TerminalSession			session_;

	Thread					*processThread_;

public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();

	/// 터미널 상태 get함수
	TERMINAL_STATUS &status();

	/// config파일 통해 초기화
	void		initialize(xmlNode_t *config);
	/// 패킷 보내기
	void		sendPacket(Packet *packet);
	/// 터미널 ip get함수
	const char *ip();
	/// 터미널 port get함수
	int			port();

private:
	/// 터미널 커낵트 스래드 함수
	void		connectProcess();
	/// 터미널 run
	void		run();
};
