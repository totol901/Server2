#pragma once

class QI_DB_REQ_ID_PW : public Query
{
public:
	oid_t clientId_;

	WCHAR *procedure()
	{
		return (WCHAR *)L"p_AccountData_Select";
	}

	QI_DB_REQ_ID_PW() 
	{
		statement_->setQuery(this->procedure(), QUERY_CALL_BACK);		// 실행할 쿼리 설정
	}

	~QI_DB_REQ_ID_PW() 
	{
		PK_I_DB_ANS_ID_PW iPacket;
		iPacket.clientId_ = (UInt64)clientId_;
		iPacket.result_ = FALSE;
		if (!record_.isEof()) 
		{
			record_.moveFirst();
		}

		while (!record_.isEof()) 
		{
			int oidAccount = 0;
			if (record_.get((char*)"oidAccount", oidAccount)) 
			{
				iPacket.oidAccountId_ = oidAccount;
				iPacket.result_ = TRUE;
				break;
			}
			else
			{
				SLog(L"* this query [%s] have error", this->procedure());
				break;
			}
			record_.moveNext();
		}

		Terminal *terminal = TERMINALMANAGER.get(L"LoginServer");
		terminal->sendPacket(&iPacket);
	}
};