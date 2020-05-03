#pragma once

//-----------------------------------------------------------------//
class IOCPSession : public Session
{
private:
	Lock sendLock_;
	bool isSending;

	//�ʱ�ȭ
	void			initialize();

	//IO���� üũ��
	void			checkErrorIO(DWORD ret);

	//��Ĺ recv ���ְ� ���� üũ��
	void			recv(WSABUF wsaBuf);
	//���� �ް� �ֳ� üũ
	bool			isRecving(size_t transferSize);
	//wsaBuf���� �۽�, ���� üũ��
	void			send(WSABUF wsaBuf);

private:
	void			sendPacket();
	void			sendPacketToClient(Packet* packet);

public:
	std::array<IoData, IO_DATA_MAX> ioData_;
	ThreadJobQueue<Packet*> sendPacketQueue_;

public:
	IOCPSession();
	virtual ~IOCPSession() {}

	
	/****************************************************************************
	�Լ���	: onSend
	����		: ��Ŷ ������ On�̸� ��Ŷ �۽���
	���ϰ�	: ����
	�Ű�����	: size_t
	*****************************************************************************/
	void			onSend(size_t transferSize);

	/****************************************************************************
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	���ϰ�	: ����
	�Ű�����	: Packet*
	*****************************************************************************/
	void		    sendPacket(Packet *packet);
	
	

	/****************************************************************************
	�Լ���	: onRecv
	����		: ������ �м�, Recv ������ ��Ŷ ��������
	���ϰ�	: Package*
	�Ű�����	: size_t
	*****************************************************************************/
	Package*		onRecv(size_t transferSize);

	/****************************************************************************
	�Լ���	: recvStandBy
	����		: ioData_[IO_READ] ���� �ʱ�ȭ, ������ recv���·� �ٲ���
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void			recvStandBy();

};
