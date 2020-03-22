#pragma once

class DBManager : public Singleton < DBManager >
{
	friend Singleton;
private:
	int                                 workerCount_;
	std::vector<Database *>             dbPool_;

	wstr_t								serverName_;
	wstr_t								dbName_;
	wstr_t								login_;
	wstr_t								password_;
	ThreadJobQueue<Query *>				*queryPool_;

	DBManager();
	~DBManager();

	void initialize(xml_t *config);

public:
	size_t runQueryCount();
	void pushQuery(Query *query);
	bool popQuery(Query **query);

	void run();
};