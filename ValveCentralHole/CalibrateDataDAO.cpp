#include "CalibrateDataDAO.h"


bool CalibrateDataDAO::InsertCalibrateData(const CalibrateData& data) const
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

		return query.exec();
	}
	return false;
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
				std::string filename = query.value(0).toString().toStdString();
				double calibration_factor = query.value(1).toDouble();
				UnitSelection units = CalibrationGaugeParameters::StringToUnitSelection(query.value(2).toString());
				std::string time = query.value(3).toString().toStdString();

				result.emplace_back(filename, time, units, calibration_factor);
			}
		}
	}

	return result;
}
