#pragma once

class LoginProcess : public ContentsProcess
{
private:
	/// 초기화
	void initialize() override;
	///해제
	void release() override;

public:
	LoginProcess();
	~LoginProcess();

	

	void registSubPacketFunc();
	static void C_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_ANS_ID_PW(Session *session, Packet *rowPacket);
	static void I_LOGIN_NOTIFY_ID_LOADED(Session *session, Packet *rowPacket);
};

