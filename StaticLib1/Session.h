#pragma once

struct SOCKET_DATA
{
	SOCKET				socket_;
	SOCKADDR_IN			addrInfo_;
};

enum
{
	SESSION_TYPE_TERMINAL,
	SESSION_TYPE_CLIENT,
};

class Package;
class Session
{
protected:
	SOCKET_DATA		    socketData_;
	oid_t				id_;
	int8_t				type_;
	tick_t				lastHeartBeat_;

	//소켓 옵션(좀비 소켓 체크) 설정
	bool				setSocketOpt();

public:
	Session();
	virtual ~Session();

	/****************************************************************************
	함수명	: onAccept
	설명		: Accept한 소켓 데이터 처리, 소켓 옵션 처리
	리턴값	: bool
	매개변수	: const SOCKET&, const SOCKADDR_IN&
	*****************************************************************************/
	virtual bool		onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo);

	virtual void		onSend(size_t transferSize) = 0;
	virtual void		sendPacket(Packet *packet) = 0;

	virtual Package*	onRecv(size_t transferSize) = 0;
	virtual void		recvStandBy() {};

	/****************************************************************************
	함수명	: onClose
	설명		: 소캣을 닫아줌
	리턴값	: 없음
	매개변수	: bool
	*****************************************************************************/
	virtual void		onClose(bool force = false);

	/****************************************************************************
	함수명	: updateHeartBeat
	설명		: lastHeartBeat_을 갱신함
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void				updateHeartBeat();

public:
	//get함수
	SOCKET&				socket();
	wstr_t				clientAddress();
	oid_t				id();
	int8_t				type();
	tick_t				heartBeat();

	//set함수
	void				setId(oid_t id);
	void				setType(int8_t type);

};
