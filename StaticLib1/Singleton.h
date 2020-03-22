#pragma once

template <class T>
class Singleton
{
protected:
	///���� �� ���� ������
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (const Singleton&&) = delete;

	///������ ������
	Singleton() {}
	virtual~Singleton() {}

public:
	/**************************************************************************
	�Լ���	: getInstance
	����		: ���� ���� instance�� ���۷����� ����
	���ϰ�	: static template<T>
	�Ű�����	: ����
	***************************************************************************/
	static T& getInstance()
	{
		static T instance;

		return instance;
	}
};
