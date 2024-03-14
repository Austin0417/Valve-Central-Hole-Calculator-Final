#include "Database.h"

QSqlDatabase Database::db_;


QSqlDatabase& Database::GetDatabase()
{
	if (!db_.isValid())
	{
		db_ = QSqlDatabase::addDatabase("QPSQL");
		db_.setDatabaseName("valve_central_hole");
		db_.setHostName("localhost");
		db_.setPassword("I<3games");
		db_.setPort(5432);
		db_.setUserName("postgres");

		if (!db_.open())
		{
			qDebug() << "Failed to open the database (database handle is invalid)";
		}
	}

	return db_;
}

void Database::InitializeDatabaseTables()
{
	QSqlDatabase& db = GetDatabase();
	if (!db.isValid())
	{
		qDebug() << "Failed to initialize database";
		return;
	}

	// Defining custom enum data types
	QSqlQuery query(db);
	if (!query.exec("CREATE TYPE DIAMETER_UNIT AS ENUM ('millimeters', 'inches');"))
	{
		qDebug() << query.lastError().text();
	}
	query.clear();

	if (!query.exec("CREATE TYPE THRESHOLD_MODE AS ENUM ('standard', 'inverted');"))
	{
		qDebug() << query.lastError().text();
	}
	query.clear();


	QString queryString = "CREATE TABLE IF NOT EXISTS calibration_gauge_parameters ("
		"id SERIAL PRIMARY KEY,"
		"gauge_diameter DOUBLE PRECISION,"
		"diameter_unit_selection DIAMETER_UNIT,"
		"threshold_value SMALLINT,"
		"threshold_mode_selection THRESHOLD_MODE,"
		"image_file_name VARCHAR(255))";
	if (!query.exec(queryString))
	{
		qDebug() << query.lastError().text();
	}
}
