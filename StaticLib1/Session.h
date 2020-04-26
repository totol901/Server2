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
	�Լ���	: clear
	����		: IoData Ŭ������ �����͸� 0���� �ʱ�ȭ��
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void clear();

	/****************************************************************************
	�Լ���	: needMoreIO
	����		: IO�� �� �ʿ����� �Ǻ���
	���ϰ�	: bool
	�Ű�����	: size_t
	*****************************************************************************/
	bool needMoreIO(size_t transferSize);

	/****************************************************************************
	�Լ���	: setupTotalBytes
	����		: totalBytes_�� ������, ���� �������� ���� �� ��Ŷ ũ�� ��ȯ
	���ϰ�	: int32_t
	�Ű�����	: ����
	*****************************************************************************/
	int32_t setupTotalBytes();

	/****************************************************************************
	�Լ���	: wsabuf
	����		: ������� �� ����, ���� ���۸� ������
	���ϰ�	: WSABUF
	�Ű�����	: ����
	*****************************************************************************/
	WSABUF wsabuf();

	/****************************************************************************
	�Լ���	: setData
	����		: buffer_�� stream�� ũ�⸦ �����, ���۸� �ڿ� �߰����ִ� ��Ŷ ����, totalBytes_ ����
	���ϰ�	: bool
	�Ű�����	: Stream&
	*****************************************************************************/
	bool setData(Stream& stream);

public:
	//get�Լ�
	const size_t& totalByte();
	const IO_OPERATION& type();

	char* data();
	LPWSAOVERLAPPED overlapped();

	//set�Լ�
	void setType(IO_OPERATION type);
};

//-----------------------------------------------------------------//
//! virtual ��� ����
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



	//���� �ɼ�(���� ���� üũ) ����
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
	�Լ���	: onAccept
	����		: Accept�� ���� ������ ó��, ���� �ɼ� ó��
	���ϰ�	: bool
	�Ű�����	: const SOCKET&, const SOCKADDR_IN&
	*****************************************************************************/
	virtual bool		onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo);
	virtual bool		onAccept(const SOCKADDR_IN& addrInfo);
	virtual bool		onAccept(AcceptData& accpetData, const SOCKADDR_IN& addrInfo);

	virtual void		onSend(size_t transferSize) = 0;
	virtual void		sendPacket(Packet *packet) = 0;

	virtual Package*	onRecv(size_t transferSize) = 0;
	virtual void		recvStandBy() {};

	/****************************************************************************
	�Լ���	: onClose
	����		: ��Ĺ�� �ݾ���
	���ϰ�	: ����
	�Ű�����	: bool
	*****************************************************************************/
	virtual void		onClose(SOCKET listenSocket = NULL);

	/****************************************************************************
	�Լ���	: updateHeartBeat
	����		: lastHeartBeat_�� ������
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void				updateHeartBeat();


public:
	//get�Լ�
	SOCKET_DATA&		socketData();
	SOCKET&&			socket();
	wstr_t				clientAddress();
	oid_t				id();
	int8_t				type();
	tick_t				heartBeat();
	const bool&			IsClosed();
	const UINT&			referenceCount();

	//set�Լ�
	void				setIsClosed(bool closed);
	void				setId(oid_t id);
	void				setType(int8_t type);
	void				setCloseSessionFuc(closeSessionFuc func);
};

