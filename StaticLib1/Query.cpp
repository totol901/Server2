#include "stdafx.h"
#include "Query.h"

Query::Query()
{
	statement_ = new QueryStatement();
}
Query::~Query()
{
	record_.close();
	SAFE_DELETE(statement_);
}

void Query::setResult(recordPtr record)
{
	record_.setRecord(record);
}

QueryRecord& Query::result()
{
	return record_;
}

void Query::setStatement(QueryStatement *statement)
{
	statement_ = statement;
}

QueryStatement* Query::statement()
{
	return statement_;
}