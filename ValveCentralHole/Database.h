#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


class Database
{
private:
	static QSqlDatabase db_;
public:
	static QSqlDatabase& GetDatabase();
	static void InitializeDatabaseTables();
};

