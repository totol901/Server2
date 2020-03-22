#pragma once


//---------------------------------------------------------------------//
class Object
{
private:
	wstr_t			allocFile_;
	int				allocLine_;

public:
	Object() {}
	~Object() {}
};

//---------------------------------------------------------------------//
class NameObject
{
private:
	wstr_t name_;

public:
	NameObject() {}
	virtual ~NameObject() {}

	wstr_t& name() { return name_; }
	void setName(wstr_t name) { name_ = name; }
};

//---------------------------------------------------------------------//
class Work
{
public:
	Work() {}
	virtual ~Work() {}

	virtual void tick() = 0;
	virtual void wakeup() {};
	virtual void suspend() {};
	virtual void stop() {};
	virtual void start() {};
};

//---------------------------------------------------------------------//
class GameObject : public NameObject, public Work
{
private:
	POINT		position_;
	float		direction_;

public:
	// Ŭ���� �̸� ���� �Ҹ��� �̸��� �ٸ��Ƿ�, free�� ���� ��Ű��
	GameObject() {}
	~GameObject() { this->free(); }

	virtual void initialize() {};
	virtual void free() {};
};
