#include "MeasureDataDAO.h"


bool MeasureDataDAO::InsertMeasureData(const MeasureData& data) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		query.prepare("INSERT INTO valve_measure_history (valve_image_filename, measured_valve_area, valve_measure_units, time_of_measurement) VALUES (:filename, :area, :units, :time)");
		query.bindValue(":filename", QString::fromStdString(data.GetFileName()));
		query.bindValue(":area", data.GetMeasuredValveArea());
		query.bindValue(":units", CalibrationGaugeParameters::UnitSelectionToString(data.GetUnits()));
		query.bindValue(":time", QString::fromStdString(data.GetTimeProcessed()));

		return query.exec();
	}
}

bool MeasureDataDAO::ClearMeasureData() const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);

		return query.exec("DELETE FROM valve_measure_history");
	}
	return false;
}

std::vector<MeasureData> MeasureDataDAO::GetAllMeasureData() const
{
	QSqlDatabase& db = Database::GetDatabase();
	std::vector<MeasureData> result;
	if (db.isValid())
	{
		QSqlQuery query(db);
		if (query.exec("SELECT * FROM valve_measure_history"))
		{
			while (query.next())
			{
				std::string filename = query.value(0).toString().toStdString();
				double area = query.value(1).toDouble();
				UnitSelection units = CalibrationGaugeParameters::StringToUnitSelection(query.value(2).toString());
				std::string time = query.value(3).toString().toStdString();
				result.emplace_back(filename, time, units, area);
			}
		}
	}
	return result;
}
