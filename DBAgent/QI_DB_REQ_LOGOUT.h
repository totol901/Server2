#pragma once

class QI_DB_REQ_LOGOUT : public Query
{
public:
	WCHAR* procedure()
	{
		return (WCHAR*)L"p_Account_CharacterLogOutDate_Update";
	}

	QI_DB_REQ_LOGOUT()
	{
		statement_->setQuery(this->procedure(), QUERY_WAIT_RETURN);		// ������ ���� ����
	}
	~QI_DB_REQ_LOGOUT()
	{
	}
};
