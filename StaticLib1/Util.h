/******************************************************************************
파일명	: Util.h
목적		: 유틸 함수들 모음
******************************************************************************/
#pragma once

///---------------------------------------------------------------
/// 자주쓰일 매크로 함수들 선언
//-------------------------------------------------------------------//
//snprintf 재정의, 안전을 위해 array 사용을 기본으로 합니다.
#define snprintf(dst, format, ...)     _snprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dst, format, ...)    _snwprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)

//-------------------------------------------------------------------//
//범위 보정 및 체크
template<typename T1, typename T2, typename T3>
constexpr auto fixInRange(T1 minimum, T2  x, T3  maximum) { return min(maximum, max(x, minimum)) ; }
template<typename T1, typename T2, typename T3>
constexpr auto isInRange(T1 minimum, T2  x, T3  maximum) { return (x == fixInRange(minimum, x, maximum)) ? true : false; }

//-------------------------------------------------------------------//
/****************************************************************************
함수명	: isOverFlower_uint
설명		: original에 add를 더 했을시 overflow 유무 검사함
리턴값	: bool
매개변수	: unsigned int, unsigned int
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
//컴파일 기본 메크로 회피용 __FUNCTION__ 같은..
#define __W(x)              L##x
#define _W(x)               __W(x)

//-------------------------------------------------------------------//
/****************************************************************************
함수명	: StrConvA2T
설명		: src(아스키 코드) -> dest(TCHAR) 변환해줌
리턴값	: 없음
매개변수	: CHAR*, TCHAR*, size_t
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
함수명	: StrConvT2A
설명		: src(TCHAR) -> dest(아스키 코드) 변환해줌
리턴값	: 없음
매개변수	: TCHAR*, CHAR*, size_t
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
함수명	: StrConvA2W
설명		: src(아스키 코드) -> dest(유니코드) 변환해줌
리턴값	: 없음
매개변수	: CHAR*, WCHAR*, size_t
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
함수명	: StrConvW2A
설명		: src(유니코드) -> dest(아스키 코드) 변환해줌
리턴값	: 없음
매개변수	: WCHAR*, CHAR*, size_t
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
//컴파일 기본 메크로 회피용 __FUNCTION__ 같은..
#define __W(x)              L##x
#define _W(x)               __W(x)