#pragma once

class LoginProcess : public ContentsProcess
{
private:
	/// �ʱ�ȭ
	void initialize() override;
	///����
	void release() override;

public:
	LoginProcess();
	~LoginProcess();

	void registSubPacketFunc();
	static void C_REQ_CREATE_ID(Session* session, Packet* rowPacket);
	static void I_DB_ANS_CREATE_ID(Session* session, Packet* rowPacket);
	static void C_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_ANS_ID_PW(Session *session, Packet *rowPacket);
	static void I_LOGIN_NOTIFY_ID_LOADED(Session *session, Packet *rowPacket);
	static void S_CHECK_ALREADY_LOGIN_SUCCESS(Session* session, Packet* rowPacket);
	static void S_CHECK_ALREADY_LOGIN_FAIL(Session* session, Packet* rowPacket);
};
