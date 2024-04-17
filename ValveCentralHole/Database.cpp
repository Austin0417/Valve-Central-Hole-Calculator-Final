#include "Database.h"

QSqlDatabase Database::db_;


QSqlDatabase& Database::GetDatabase()
{
	if (!db_.isValid())
	{
		db_ = QSqlDatabase::addDatabase("QSQLITE");
		db_.setDatabaseName("valve_central_hole_db");

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

	QSqlQuery query(db);



	QString query_string = "CREATE TABLE IF NOT EXISTS calibration_history ("
		"id INTEGER PRIMARY KEY,"
		"calibration_gauge_filename TEXT,"
		"calculated_calibration_factor DECIMAL(10, 5),"
		"calibration_units TEXT,"
		"time_of_calibration TEXT)";

	if (!query.exec(query_string))
	{
		qDebug() << query.lastError().text();
	}
	else
	{
		qDebug() << "Created calibration history table successfully";
	}

	query.clear();
	query_string = "CREATE TABLE IF NOT EXISTS valve_measure_history ("
		"id INTEGER PRIMARY KEY,"
		"valve_image_filename TEXT,"
		"measured_valve_area DECIMAL(10, 5),"
		"valve_measure_units TEXT,"
		"time_of_measurement TEXT)";

	if (!query.exec(query_string))
	{
		qDebug() << query.lastError().text();
	}
	else
	{
		qDebug() << "Created measure history table successfully";
	}

}
