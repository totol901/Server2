#pragma once

class DBAgentProcess : public ContentsProcess
{
public:
	DBAgentProcess();
	~DBAgentProcess();

	/// �ʱ�ȭ
	void initialize() override;
	///����
	void release() override;

private:
	void registSubPacketFunc();

	static void I_DB_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_REQ_CREATE_ID(Session* session, Packet* rowPacket);
	static void I_DB_REQ_LOAD_DATA(Session *session, Packet *rowPacket);
};
