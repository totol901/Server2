#pragma once

typedef enum
{
	IO_READ,
	IO_WRITE,
	IO_ERROR,
} IO_OPERATION;

#define IO_DATA_MAX     (2)

class IoData
{
private:
	OVERLAPPED		overlapped_;
	IO_OPERATION	ioType_;
	size_t  		totalBytes_;
	size_t			currentBytes_;
	std::array<char, SOCKET_BUF_SIZE> buffer_;

public:
	IoData();
	~IoData() {}

	/****************************************************************************
	함수명	: clear
	설명		: IoData 클래스의 데이터를 0으로 초기화함
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void clear();

	/****************************************************************************
	함수명	: needMoreIO
	설명		: IO가 더 필요한지 판별함
	리턴값	: bool
	매개변수	: size_t
	*****************************************************************************/
	bool needMoreIO(size_t transferSize);

	/****************************************************************************
	함수명	: setupTotalBytes
	설명		: totalBytes_를 설정함, 설정 되있을시 무시 후 패킷 크기 반환
	리턴값	: int32_t
	매개변수	: 없음
	*****************************************************************************/
	int32_t setupTotalBytes();

	/****************************************************************************
	함수명	: wsabuf
	설명		: 현재까지 온 버퍼, 길이 버퍼를 리턴함
	리턴값	: WSABUF
	매개변수	: 없음
	*****************************************************************************/
	WSABUF wsabuf();

	/****************************************************************************
	함수명	: setData
	설명		: buffer_에 stream의 크기를 헤더로, 버퍼를 뒤에 추가해주는 패킷 만듬, totalBytes_ 갱신
	리턴값	: bool
	매개변수	: Stream&
	*****************************************************************************/
	bool setData(Stream &stream);

public:
	//get함수
	size_t totalByte();
	IO_OPERATION &type();

	char *data();
	LPWSAOVERLAPPED overlapped();

	//set함수
	void setType(IO_OPERATION type);

};

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
