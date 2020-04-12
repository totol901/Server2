#pragma once

class QI_DB_REQ_CREATE_ID : public Query
{
public:
	oid_t clientId_;

	WCHAR* procedure()
	{
		return (WCHAR*)L"p_Account_CharaterData_Insert";
	}

	QI_DB_REQ_CREATE_ID()
	{
		statement_->setQuery(this->procedure(), QUERY_WAIT_RETURN);		// 실행할 쿼리 설정
	}

	~QI_DB_REQ_CREATE_ID()
	{
		PK_I_DB_ANS_CREATE_ID iPacket;
		iPacket.clientId_ = (UInt64)clientId_;
		iPacket.result_ = FALSE;

		if (record_.resultVal() == TRUE)
		{
			iPacket.result_ = TRUE;
		}
		else
		{
			SLog(L"* this query [%s] have error", this->procedure());
		}

		Terminal* terminal = TERMINALMANAGER.get(L"LoginServer");
		terminal->sendPacket(&iPacket);
	}
};