#pragma once

class GameProcess : public ContentsProcess
{
private:
	void registSubPacketFunc();

	/// 초기화
	void initialize() override;
	///해제
	void release() override;

public:
	GameProcess();
	~GameProcess();
};
