#pragma once

//-----------------------------------------------------------------//
class IOCPSession : public Session
{
private:
	//초기화
	void			initialize();

	//IO에러 체크함
	void			checkErrorIO(DWORD ret);

	//소캣 recv 해주고 에러 체크함
	void			recv(WSABUF wsaBuf);
	//소켓 받고 있나 체크
	bool			isRecving(size_t transferSize);
	//wsaBuf버퍼 송신, 에러 체크함
	void			send(WSABUF wsaBuf);

public:
	std::array<IoData, IO_DATA_MAX> ioData_;

public:
	IOCPSession();
	virtual ~IOCPSession() {}

	/****************************************************************************
	함수명	: onSend
	설명		: 페킷 보내기 On이면 패킷 송신함
	리턴값	: 없음
	매개변수	: size_t
	*****************************************************************************/
	void			onSend(size_t transferSize);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet데이터를 ioData_ 형태로 변형 후 send함
	리턴값	: 없음
	매개변수	: Packet*
	*****************************************************************************/
	void		    sendPacket(Packet *packet);

	/****************************************************************************
	함수명	: onRecv
	설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
	리턴값	: Package*
	매개변수	: size_t
	*****************************************************************************/
	Package*		onRecv(size_t transferSize);

	/****************************************************************************
	함수명	: recvStandBy
	설명		: ioData_[IO_READ] 버퍼 초기화, 소켓을 recv상태로 바꿔줌
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void			recvStandBy();

};
