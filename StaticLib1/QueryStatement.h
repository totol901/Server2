#pragma once

typedef enum
{
	QUERY_NOT_RETURN,				//리턴없는 쿼리
	QUERY_WAIT_RETURN,				//DB응답을 꼭 기다린뒤 처리하는 쿼리
	QUERY_CALL_BACK,				//DB응답을 기다리지 않지만 뒷처리는 있는 쿼리
}QUERY_TYPE;

class QueryStatement
{
	//ADODB::_CommandPtr		command_;	//ADO 명령문 제어
	QUERY_TYPE				type_;		//쿼리 타입

	wstr_t					query_;		//쿼리문
	int						paramCount_;//파라미터 갯수

public:
	QueryStatement();
	~QueryStatement();

	void setQuery(WCHAR *query, QUERY_TYPE type = QUERY_NOT_RETURN);

	const WCHAR *query();
	QUERY_TYPE type();

	//------------------ 파라메터 추가 -----------------/
	template<typename T>
	void addArg(WCHAR *fmt, T value);

	void addParam(CHAR *value);
	void addParam(WCHAR *value);
	void addParam(INT32 value);
	void addParam(UINT32 value);
	void addParam(INT64 value);
	void addParam(UINT64 value);
	void addParam(FLOAT value);
	void addParam(DOUBLE value);
};