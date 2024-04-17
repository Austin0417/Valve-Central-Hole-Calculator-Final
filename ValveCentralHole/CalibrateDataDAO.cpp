#include "CalibrateDataDAO.h"


std::optional<CalibrateData> CalibrateDataDAO::InsertCalibrateData(const CalibrateData& data) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		query.prepare("INSERT INTO calibration_history (calibration_gauge_filename, calculated_calibration_factor, calibration_units, time_of_calibration) VALUES (:filename, :calibration_factor, :calibration_units, :time)");
		query.bindValue(":filename", QString::fromStdString(data.GetFileName()));
		query.bindValue(":calibration_factor", data.GetCalibrationFactor());
		query.bindValue(":calibration_units", CalibrationGaugeParameters::UnitSelectionToString(data.GetUnits()));
		query.bindValue(":time", QString::fromStdString(data.GetTimeProcessed()));

		if (query.exec())
		{
			return FindCalibrateData(data);
		}
	}
	return std::nullopt;
}

std::optional<CalibrateData> CalibrateDataDAO::FindCalibrateData(const CalibrateData& data) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM calibration_history WHERE time_of_calibration=:time");
		query.bindValue(":time", QString::fromStdString(data.GetTimeProcessed()));

		if (query.exec() && query.next())
		{
			int id = query.value(0).toInt();
			std::string filename = query.value(1).toString().toStdString();
			double calibration_factor = query.value(2).toDouble();
			UnitSelection units = CalibrationGaugeParameters::StringToUnitSelection(query.value(3).toString());
			std::string time = query.value(4).toString().toStdString();
			return CalibrateData(id, filename, time, units, calibration_factor);
		}
	}
	return std::nullopt;
}


bool CalibrateDataDAO::ClearCalibrateData() const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		return query.exec("DELETE FROM calibration_history");
	}
	return false;
}


std::vector<CalibrateData> CalibrateDataDAO::GetAllCalibrateData() const
{
	QSqlDatabase& db = Database::GetDatabase();
	std::vector<CalibrateData> result;
	if (db.isValid())
	{
		QSqlQuery query(db);

		if (query.exec("SELECT * FROM calibration_history"))
		{
			while (query.next())
			{
				int id = query.value(0).toInt();
				std::string filename = query.value(1).toString().toStdString();
				double calibration_factor = query.value(2).toDouble();
				UnitSelection units = CalibrationGaugeParameters::StringToUnitSelection(query.value(3).toString());
				std::string time = query.value(4).toString().toStdString();

				result.emplace_back(id, filename, time, units, calibration_factor);
			}
		}
	}

	return result;
}

bool CalibrateDataDAO::DeleteSelectedData(const CalibrateData& data) const
{
	QSqlDatabase& db = Database::GetDatabase();
	if (db.isValid())
	{
		QSqlQuery query(db);
		query.prepare("DELETE FROM calibration_history WHERE id=:data_id");
		query.bindValue(":data_id", data.GetId());

		return query.exec();
	}
	return false;
}


bool CalibrateDataDAO::DeleteSelectedDataList(const std::vector<CalibrateData>& data_to_delete) const
{
	QSqlDatabase& db = Database::GetDatabase();

	if (db.isValid())
	{
		for (const auto& data : data_to_delete)
		{
			if (!DeleteSelectedData(data))
			{
				return false;
			}
		}
		return true;
	}

	return false;
}
