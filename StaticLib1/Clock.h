#pragma once

using namespace std::chrono;

// TODO : ���� �̱����� ���� �ɰ��� ���� ��ũ�� ���� ���� ���ٰ� ��
#define NOW_TICK					CLOCK.systemTick
#define NOW_STRING					CLOCK.nowTime

//���� ���
const tick_t TICK_MIN(60);
const tick_t TICK_HOUR(TICK_MIN * 60);
const tick_t TICK_DAY(TICK_HOUR * 24);

//���� inline �Լ�
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

//���� enum��
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

//data ���� ��ũ��
#define DATETIME_FORMAT         L"D%Y-%m-%dT%H:%M:%S"
#define DATE_FORMAT             L"%Y-%m-%d"
#define TIME_FORMAT             L"%H:%M:%S"
#define DB_TIME_FORMAT          L"%4d-%2d-%2d %2d:%2d:%2d"

//timePoint �ڷ��� ����
typedef system_clock::time_point timePoint;

class Clock : public Singleton<Clock>
{
	friend Singleton;
private:
	tick_t	serverStartTick_;	//���� ���� �ð�

private:
	Clock();
	~Clock();

	/****************************************************************************
	�Լ���	: tickToStr
	����		: tick�� fmt�� �°� str�� ���� ������
	���ϰ�	: wstr_t
	�Ű�����	: tick_t, (WCHAR*)
	*****************************************************************************/
	wstr_t	tickToStr(tick_t tick, const WCHAR* fmt = DATETIME_FORMAT);

public:
	HRESULT init();
	void release();

	/****************************************************************************
	�Լ���	: systemTick
	����		: ���� �ý��� �ð�(chrono)�� tick_t(time_t)�� ������
	���ϰ�	: tick_t
	�Ű�����	: ����
	*****************************************************************************/
	tick_t	systemTick();

	/****************************************************************************
	�Լ���	: strToTick
	����		: str�� fmt�� �°� �ҷ��� tick_t���� ������
	���ϰ�	: tick_t
	�Ű�����	: wstr_t, (WCHAR*)
	*****************************************************************************/
	tick_t	strToTick(wstr_t str, const WCHAR* fmt = DB_TIME_FORMAT);

	/****************************************************************************
	�Լ���	: nowTime
	����		: ���� �ð� tick_t�� fmt�� �°� str�� �ҷ���
	���ϰ�	: wstr_t
	�Ű�����	: (WCHAR*)
	*****************************************************************************/
	wstr_t	nowTime(const WCHAR* fmt = DATETIME_FORMAT);

	/****************************************************************************
	�Լ���	: nowTimeWithMilliSec
	����		: ���� �ð��� millisec���� ǥ���Ͽ� fmt�� �°� str�� �ҷ���
	���ϰ�	: wstr_t
	�Ű�����	: (WCHAR*)
	*****************************************************************************/
	wstr_t	nowTimeWithMilliSec(const WCHAR* fmt = DATETIME_FORMAT);

	/****************************************************************************
	�Լ���	: today
	����		: ������ ��, ��, ���� str�� ������
	���ϰ�	: wstr_t
	�Ű�����	: ����
	*****************************************************************************/
	wstr_t today();

	/****************************************************************************
	�Լ���	: tomorrow
	����		: ������ ��, ��, ���� str�� ������
	���ϰ�	: wstr_t
	�Ű�����	: ����
	*****************************************************************************/
	wstr_t tomorrow();

	/****************************************************************************
	�Լ���	: yesterday
	����		: ������ ��, ��, ���� str�� ������
	���ϰ�	: wstr_t
	�Ű�����	: ����
	*****************************************************************************/
	wstr_t yesterday();

	/****************************************************************************
	�Լ���	: todayOfTheWeek
	����		: ������ ����(enum)�� ������
	���ϰ�	: DayOfTheWeek
	�Ű�����	: ����
	*****************************************************************************/
	DayOfTheWeek todayOfTheWeek();

public:
	//Get�Լ�
	tick_t	serverStartTick() const;
};

