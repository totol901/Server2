#pragma once

#include "../StaticLib1/stdafx.h"

struct Vector3
{
	float x_;
	float y_;
	float z_;

	const Vector3& operator *(float num)
	{
		x_ *= num;
		y_ *= num;
		z_ *= num;
	}

	const Vector3& operator +(Vector3 vec)
	{
		x_ += vec.x_;
		y_ += vec.y_;
		z_ += vec.z_;
	}

	Vector3(float x, float y, float z)
		:x_(x), y_(y), z_(z)
	{}
	Vector3()
		:x_(0), y_(0), z_(0)
	{}
};

struct Quaternion
{
	float x_;
	float y_;
	float z_;
	float w_;

	Quaternion(float x, float y, float z, float w)
		:x_(x), y_(y), z_(z), w_(w)
	{}
	Quaternion()
		:x_(0), y_(0), z_(0), w_(1.0f)
	{}
};

#include "User.h"
#include "UserMap.h"
#include "UserManager.h"
#include "GameProcess.h"
#include "GameIOCPServer.h"
#include "GameServerClass.h"
#include "GameManager.h"



#define GAMEMANAGER GameManager::getInstance()
#define USERMANAGER UserManager::getInstance()
