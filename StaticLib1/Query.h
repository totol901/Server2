#pragma once

class Query
{
protected:
	QueryStatement		*statement_;
	QueryRecord			record_;

public:
	Query();
	virtual ~Query();

	void setResult(recordPtr record);
	QueryRecord &result();

	void setStatement(QueryStatement *statement);
	QueryStatement *statement();

	// ������ �����Լ��� ȣ���ؼ� ��ó���� ��Ű�°� �´µ�... �׷��� h, cpp ���� �Ѵ� ������ ��.
	// �׳� �Ҹ��ڿ� ��ó���� �ϵ��� ó��
//protected:
//	virtual void doRespon() = 0;
};