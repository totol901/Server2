#pragma once

// �ٸ� �������� ������ ���� ���
class TerminalManager : public Singleton<TerminalManager>
{
	friend Singleton;
private:
	TerminalManager();
	virtual ~TerminalManager();

	std::unordered_map<wstr_t, Terminal*> terminalPool_;
	Server						*server_;
	
public:
	/// config���� ���� �ʱ�ȭ
	void initialize(xml_t *config);
	void release();

	/// �͹̳� �־��ִ� �Լ�
	void put(wstr_t serverName, Terminal *terminal);
	/// �͹̳� ã���ִ� �Լ�
	Terminal* get(wstr_t name);

	/// ip�� �͹̳� �Ǻ� �Լ�
	bool isTerminal(const char *ip);
	/// �͹̳� �Ŵ��� run
	void run(Server *server);
};
