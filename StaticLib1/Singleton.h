#pragma once

template <class T>
class Singleton
{
protected:
	///복사 및 생성 막아줌
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (const Singleton&&) = delete;

	///생성자 막아줌
	Singleton() {}
	virtual~Singleton() {}

public:
	/**************************************************************************
	함수명	: getInstance
	설명		: 정적 변수 instance의 래퍼런스를 리턴
	리턴값	: static template<T>
	매개변수	: 없음
	***************************************************************************/
	static T& getInstance()
	{
		static T instance;

		return instance;
	}
};
