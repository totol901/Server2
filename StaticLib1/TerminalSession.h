#pragma once

class TerminalSession : public Session
{
private:
	SOCKET terminalSocket_;
public:
	TerminalSession();
	~TerminalSession();
	/****************************************************************************
	함수명	: connectTo
	설명		: ip, port의 서버에 connect해줌
	리턴값	: bool
	매개변수	: char*, int
	*****************************************************************************/
	bool		connectTo(char *ip, int port);

	//불필요
	void		onSend(size_t transferSize);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet을 사이즈 헤더 + 데이터 형태로 보냄
	리턴값	: 없음
	매개변수	: Packet*
	*****************************************************************************/
	void		sendPacket(Packet *packet);

	/****************************************************************************
	함수명	: onRecv
	설명		: 연결된 서버로 부터 페킷 받아옴
	리턴값	: Package*
	매개변수	: size_t
	*****************************************************************************/
	Package*	onRecv(size_t transferSize);
};
