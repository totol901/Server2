#pragma once

using namespace std::chrono;

// TODO : 위의 싱글턴을 쓰면 될것을 굳이 매크로 만들 이유 없다고 봄
#define NOW_TICK					CLOCK.systemTick
#define NOW_STRING					CLOCK.nowTime

//전역 상수
const tick_t TICK_MIN(60);
const tick_t TICK_HOUR(TICK_MIN * 60);
const tick_t TICK_DAY(TICK_HOUR * 24);

//전역 inline 함수
inline tick_t TICK_TO_MIN(int& x) { return (x / TICK_MIN); }
inline tick_t TICK_TO_MIN(int&& x) { return (x / TICK_MIN); }
inline tick_t MIN_TO_TICK(int& x) { return (x * TICK_MIN); }
inline tick_t MIN_TO_TICK(int&& x) { return (x * TICK_MIN); }
inline tick_t TICK_TO_HOUR(int& x) { return (x / TICK_HOUR); }
inline tick_t TICK_TO_HOUR(int&& x) { return (x / TICK_HOUR); }
inline tick_t HOUR_TO_TICK(int& x) { return (x * TICK_HOUR); }
inline tick_t HOUR_TO_TICK(int&& x) { return (x * TICK_HOUR); }
inline tick_t TICK_TO_DAY(int& x) { return (x / TICK_DAY); }
inline tick_t TICK_TO_DAY(int&& x) { return (x / TICK_DAY); }
inline tick_t DAY_TO_TICK(int& x) { return (x * TICK_DAY); }
inline tick_t DAY_TO_TICK(int&& x) { return (x * TICK_DAY); }

//요일 enum문
typedef enum
{
	DAY_SUNDAY = 0,
	DAY_MONDAY = 1,
	DAY_TUESDAY = 2,
	DAY_WEDNESDAY = 3,
	DAY_THURSDAY = 4,
	DAY_FRIDAY = 5,
	DAY_SATURDAY = 6,
} DayOfTheWeek;

//data 포멧 매크로
#define DATETIME_FORMAT         L"D%Y-%m-%dT%H:%M:%S"
#define DATE_FORMAT             L"%Y-%m-%d"
#define TIME_FORMAT             L"%H:%M:%S"
#define DB_TIME_FORMAT          L"%4d-%2d-%2d %2d:%2d:%2d"

//timePoint 자료형 정의
typedef system_clock::time_point timePoint;

class Clock : public Singleton<Clock>
{
	friend Singleton;
private:
	tick_t	serverStartTick_;	//서버 시작 시간

private:
	Clock();
	~Clock();

	/****************************************************************************
	함수명	: tickToStr
	설명		: tick을 fmt에 맞게 str에 쓰고 리턴함
	리턴값	: wstr_t
	매개변수	: tick_t, (WCHAR*)
	*****************************************************************************/
	wstr_t	tickToStr(tick_t tick, const WCHAR* fmt = DATETIME_FORMAT);

public:
	HRESULT init();
	void release();

	/****************************************************************************
	함수명	: systemTick
	설명		: 현재 시스템 시간(chrono)의 tick_t(time_t)을 리턴함
	리턴값	: tick_t
	매개변수	: 없음
	*****************************************************************************/
	tick_t	systemTick();

	/****************************************************************************
	함수명	: strToTick
	설명		: str을 fmt에 맞게 불러와 tick_t으로 리턴함
	리턴값	: tick_t
	매개변수	: wstr_t, (WCHAR*)
	*****************************************************************************/
	tick_t	strToTick(wstr_t str, const WCHAR* fmt = DB_TIME_FORMAT);

	/****************************************************************************
	함수명	: nowTime
	설명		: 현재 시간 tick_t을 fmt에 맞게 str로 불러옴
	리턴값	: wstr_t
	매개변수	: (WCHAR*)
	*****************************************************************************/
	wstr_t	nowTime(const WCHAR* fmt = DATETIME_FORMAT);

	/****************************************************************************
	함수명	: nowTimeWithMilliSec
	설명		: 현재 시간을 millisec까지 표시하여 fmt에 맞게 str로 불러옴
	리턴값	: wstr_t
	매개변수	: (WCHAR*)
	*****************************************************************************/
	wstr_t	nowTimeWithMilliSec(const WCHAR* fmt = DATETIME_FORMAT);

	/****************************************************************************
	함수명	: today
	설명		: 오늘의 년, 월, 일을 str로 리턴함
	리턴값	: wstr_t
	매개변수	: 없음
	*****************************************************************************/
	wstr_t today();

	/****************************************************************************
	함수명	: tomorrow
	설명		: 내일의 년, 월, 일을 str로 리턴함
	리턴값	: wstr_t
	매개변수	: 없음
	*****************************************************************************/
	wstr_t tomorrow();

	/****************************************************************************
	함수명	: yesterday
	설명		: 어제의 년, 월, 일을 str로 리턴함
	리턴값	: wstr_t
	매개변수	: 없음
	*****************************************************************************/
	wstr_t yesterday();

	/****************************************************************************
	함수명	: todayOfTheWeek
	설명		: 오늘의 요일(enum)을 리턴함
	리턴값	: DayOfTheWeek
	매개변수	: 없음
	*****************************************************************************/
	DayOfTheWeek todayOfTheWeek();

public:
	//Get함수
	tick_t	serverStartTick() const;
};

