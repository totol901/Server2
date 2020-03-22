/******************************************************************************
���ϸ�	: Util.h
����		: ��ƿ �Լ��� ����
******************************************************************************/
#pragma once

///---------------------------------------------------------------
/// ���־��� ��ũ�� �Լ��� ����
//-------------------------------------------------------------------//
//snprintf ������, ������ ���� array ����� �⺻���� �մϴ�.
#define snprintf(dst, format, ...)     _snprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dst, format, ...)    _snwprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)

//-------------------------------------------------------------------//
//���� ���� �� üũ
template<typename T1, typename T2, typename T3>
constexpr auto fixInRange(T1 minimum, T2  x, T3  maximum) { return min(maximum, max(x, minimum)) ; }
template<typename T1, typename T2, typename T3>
constexpr auto isInRange(T1 minimum, T2  x, T3  maximum) { return (x == fixInRange(minimum, x, maximum)) ? true : false; }

//-------------------------------------------------------------------//
/****************************************************************************
�Լ���	: isOverFlower_uint
����		: original�� add�� �� ������ overflow ���� �˻���
���ϰ�	: bool
�Ű�����	: unsigned int, unsigned int
*****************************************************************************/
inline bool isOverFlower_uint(unsigned int original, unsigned int add)
{
	unsigned int before = original;
	unsigned int after = original + add;
	if ((original & 0x80000000) != (after & 0x80000000))
	{
		return false;
	}
	return true;
}

//-------------------------------------------------------------------//
//������ �⺻ ��ũ�� ȸ�ǿ� __FUNCTION__ ����..
#define __W(x)              L##x
#define _W(x)               __W(x)

//-------------------------------------------------------------------//
/****************************************************************************
�Լ���	: StrConvA2T
����		: src(�ƽ�Ű �ڵ�) -> dest(TCHAR) ��ȯ����
���ϰ�	: ����
�Ű�����	: CHAR*, TCHAR*, size_t
*****************************************************************************/
inline void StrConvA2T(CHAR *src, TCHAR *dest, size_t destLen)
{
#ifdef  UNICODE                     // r_winnt
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
#endif
}

/****************************************************************************
�Լ���	: StrConvT2A
����		: src(TCHAR) -> dest(�ƽ�Ű �ڵ�) ��ȯ����
���ϰ�	: ����
�Ű�����	: TCHAR*, CHAR*, size_t
*****************************************************************************/
inline void StrConvT2A(TCHAR *src, CHAR *dest, size_t destLen)
{
#ifdef  UNICODE                     // r_winnt
	if (destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
#endif
}

/****************************************************************************
�Լ���	: StrConvA2W
����		: src(�ƽ�Ű �ڵ�) -> dest(�����ڵ�) ��ȯ����
���ϰ�	: ����
�Ű�����	: CHAR*, WCHAR*, size_t
*****************************************************************************/
inline void StrConvA2W(CHAR *src, WCHAR *dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
}

/****************************************************************************
�Լ���	: StrConvW2A
����		: src(�����ڵ�) -> dest(�ƽ�Ű �ڵ�) ��ȯ����
���ϰ�	: ����
�Ű�����	: WCHAR*, CHAR*, size_t
*****************************************************************************/
inline void StrConvW2A(WCHAR *src, CHAR *dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
}


//-------------------------------------------------------------------//
// delete object
#undef	SAFE_DELETE
#define SAFE_DELETE(obj)						\
{												\
	if ((obj)) delete(obj);		    			\
    (obj) = nullptr;							\
}

// delete object array
#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(arr)					\
{												\
	if ((arr)) delete [] (arr);		    		\
    (arr) = nullptr;							\
}

// delete gameObject
#define SAFE_FREE(obj)							\
{												\
	if ((obj)) obj->free();						\
    (obj) = nullptr;							\
}

#define SAFE_RELEASE(obj)                       \
{                                               \
	if (obj) { obj.release(); }                 \
}

//-------------------------------------------------------------------//
//������ �⺻ ��ũ�� ȸ�ǿ� __FUNCTION__ ����..
#define __W(x)              L##x
#define _W(x)               __W(x)