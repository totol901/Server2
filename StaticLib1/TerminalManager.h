#pragma once

// 다른 서버간의 연결을 위한 통로
class TerminalManager : public Singleton<TerminalManager>
{
	friend Singleton;
private:
	TerminalManager();
	virtual ~TerminalManager();

	std::unordered_map<wstr_t, Terminal*> terminalPool_;
	Server						*server_;

public:
	/// config파일 통해 초기화
	void initialize(xml_t *config);
	void release();

	/// 터미널 넣어주는 함수
	void put(wstr_t serverName, Terminal *terminal);
	/// 터미널 찾아주는 함수
	Terminal* get(wstr_t name);

	/// ip로 터미널 판별 함수
	bool isTerminal(const char *ip);
	/// 터미널 매니저 run
	void run(Server *server);
};
