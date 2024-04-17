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
	return false;
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
				int id = query.value(0).toInt();
				std::string filename = query.value(1).toString().toStdString();
				double area = query.value(2).toDouble();
				UnitSelection units = CalibrationGaugeParameters::StringToUnitSelection(query.value(3).toString());
				std::string time = query.value(4).toString().toStdString();
				result.emplace_back(id, filename, time, units, area);
			}
		}
	}
	return result;
}

bool MeasureDataDAO::DeleteSelectedMeasureData(const MeasureData& measure_data) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		query.prepare("DELETE FROM valve_measure_history WHERE id=:id");
		query.bindValue(":id", measure_data.GetId());
		return query.exec();
	}
	return false;
}

bool MeasureDataDAO::DeleteSelectedMeasureDataList(const std::vector<MeasureData>& measure_data_list) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		for (const auto& data : measure_data_list)
		{
			if (!DeleteSelectedMeasureData(data))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
