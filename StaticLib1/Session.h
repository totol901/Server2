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

	//���� �ɼ�(���� ���� üũ) ����
	bool				setSocketOpt();

public:
	Session();
	virtual ~Session();

	/****************************************************************************
	�Լ���	: onAccept
	����		: Accept�� ���� ������ ó��, ���� �ɼ� ó��
	���ϰ�	: bool
	�Ű�����	: const SOCKET&, const SOCKADDR_IN&
	*****************************************************************************/
	virtual bool		onAccept(const SOCKET& socket, const SOCKADDR_IN& addrInfo);

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
	virtual void		onClose(bool force = false);

	/****************************************************************************
	�Լ���	: updateHeartBeat
	����		: lastHeartBeat_�� ������
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void				updateHeartBeat();

public:
	//get�Լ�
	SOCKET&				socket();
	wstr_t				clientAddress();
	oid_t				id();
	int8_t				type();
	tick_t				heartBeat();

	//set�Լ�
	void				setId(oid_t id);
	void				setType(int8_t type);

};
