#pragma once

typedef enum
{
	IO_READ,
	IO_WRITE,
	IO_ACCEPT,
	IO_ERROR,
} IO_OPERATION;

#define IO_DATA_MAX     (2)

class IoData
{
protected:
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
	bool setData(Stream& stream);

public:
	//get함수
	const size_t& totalByte();
	const IO_OPERATION& type();

	char* data();
	LPWSAOVERLAPPED overlapped();

	//set함수
	void setType(IO_OPERATION type);
};

//-----------------------------------------------------------------//
//! virtual 사용 금지
class AcceptData : public IoData
{
private:
	SOCKET			acceptSocket_;

public:
	AcceptData() 
		:IoData()
	{ ioType_ = IO_ACCEPT; }
	~AcceptData() {}

	void setAcceptSocket(SOCKET acceptSocket);
	SOCKET acceptSocket();
};

//-----------------------------------------------------------------//

struct SOCKET_DATA
{
	AcceptData*			acceptData_;
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

	Lock				lock_;

	UINT				referenceCount_;

	bool				isClosed_;



	//소켓 옵션(좀비 소켓 체크) 설정
	bool				setSocketOpt();

	typedef std::function<void(Session* session)> closeSessionFuc;
	closeSessionFuc closeSessionFuc_;

public:
	Session();
	Session(closeSessionFuc SessionFuc);
	virtual ~Session();

	void IncreseReferenceCount();
	void DecreseReferenceCount();

	/****************************************************************************
	함수명	: onAccept
	설명		: Accept한 소켓 데이터 처리, 소켓 옵션 처리
	리턴값	: bool
	매개변수	: const SOCKET&, const SOCKADDR_IN&
	*****************************************************************************/
	virtual bool		onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo);
	virtual bool		onAccept(const SOCKADDR_IN& addrInfo);
	virtual bool		onAccept(AcceptData& accpetData, const SOCKADDR_IN& addrInfo);

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
	virtual void		onClose(SOCKET listenSocket = NULL);

	/****************************************************************************
	함수명	: updateHeartBeat
	설명		: lastHeartBeat_을 갱신함
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void				updateHeartBeat();


public:
	//get함수
	SOCKET_DATA&		socketData();
	SOCKET&&			socket();
	wstr_t				clientAddress();
	oid_t				id();
	int8_t				type();
	tick_t				heartBeat();
	const bool&			IsClosed();
	const UINT&			referenceCount();

	//set함수
	void				setIsClosed(bool closed);
	void				setId(oid_t id);
	void				setType(int8_t type);
	void				setCloseSessionFuc(closeSessionFuc func);
};

