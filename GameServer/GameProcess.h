#pragma once

class GameProcess : public ContentsProcess
{
private:
	void registSubPacketFunc();

	/// �ʱ�ȭ
	void initialize() override;
	///����
	void release() override;

public:
	GameProcess();
	~GameProcess();
};
