#pragma once
//�� ���°�?
//���ó��?
//���� ���?

// �͹̳� ������, Ÿ ������ �����ؼ� ������ �ۼ��ſ� ����Ѵ�.
enum TERMINAL_STATUS
{
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Terminal
{
protected:
	Server					*server_;
	wstr_t					name_;
	TERMINAL_STATUS			status_;

	char					ip_[16];
	int						port_;

	TerminalSession			session_;

	Thread					*processThread_;

public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();

	/// �͹̳� ���� get�Լ�
	TERMINAL_STATUS &status();

	/// config���� ���� �ʱ�ȭ
	void		initialize(xmlNode_t *config);
	/// ��Ŷ ������
	void		sendPacket(Packet *packet);
	/// �͹̳� ip get�Լ�
	const char *ip();
	/// �͹̳� port get�Լ�
	int			port();

private:
	/// �͹̳� Ŀ��Ʈ ������ �Լ�
	void		connectProcess();
	/// �͹̳� run
	void		run();
};
