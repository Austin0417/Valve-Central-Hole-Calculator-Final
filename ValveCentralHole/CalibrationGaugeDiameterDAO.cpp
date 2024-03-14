#include "CalibrationGaugeParametersDAO.h"


bool CalibrationGaugeParametersDAO::IsTableEmpty()
{
	QSqlDatabase& db = Database::GetDatabase();
	if (!db.isValid())
	{
		return false;
	}

	QSqlQuery query(db);

	if (query.exec("SELECT * FROM calibration_gauge_parameters"))
	{
		return !query.next();
	}
	return false;
}


void CalibrationGaugeParametersDAO::IsTableEmpty(const std::function<void(bool)>& callback)
{
	QSqlDatabase& db = Database::GetDatabase();
	if (!db.isValid())
	{
		callback(false);
		return;
	}

	QSqlQuery query(db);

	if (query.exec("SELECT * FROM calibration_gauge_parameters"))
	{
		callback(!query.next());
	}
}

void CalibrationGaugeParametersDAO::AddGaugeParameters(const CalibrationGaugeParameters& gauge_parameters, const std::function<void(bool)>& callback)
{
	QSqlDatabase& db = Database::GetDatabase();
	if (!db.isValid())
	{
		callback(false);
		return;
	}

	QSqlQuery query(db);

	query.prepare("INSERT INTO calibration_gauge_parameters"
		" (gauge_diameter, diameter_unit_selection, threshold_value, threshold_mode_selection, image_file_name)"
		" VALUES (:diameter, :unit_selection, :threshold_value, :threshold_mode, :image_name)");
	query.bindValue(":gauge_diameter", gauge_parameters.GetGaugeDiameter());
	query.bindValue(":unit_selection", CalibrationGaugeParameters::UnitSelectionToString(gauge_parameters.GetUnitSelection()));
	query.bindValue(":threshold_value", gauge_parameters.GetThresholdValue());
	query.bindValue(":threshold_mode", CalibrationGaugeParameters::ThresholdModeToString(gauge_parameters.GetThresholdMode()));
	query.bindValue(":image_name", gauge_parameters.GetImageFileName());

	callback(query.exec());
}


void CalibrationGaugeParametersDAO::UpdateGaugeParameters(const CalibrationGaugeParameters& gauge_parameters, const std::function<void(bool)>& callback)
{
	QSqlDatabase& db = Database::GetDatabase();
	if (!db.isValid())
	{
		callback(false);
		return;
	}

	QSqlQuery query(db);

	query.prepare("UPDATE calibration_gauge_parameters SET gauge_diameter=:gauge_diameter, diameter_unit_selection=:unit_selection, threshold_value=:threshold_value, "
		"threshold_mode_selection=:threshold_mode, image_file_name=:image_name WHERE id = 1");

	query.bindValue(":gauge_diameter", gauge_parameters.GetGaugeDiameter());
	query.bindValue(":unit_selection", CalibrationGaugeParameters::UnitSelectionToString(gauge_parameters.GetUnitSelection()));
	query.bindValue(":threshold_value", gauge_parameters.GetThresholdValue());
	query.bindValue(":threshold_mode", CalibrationGaugeParameters::ThresholdModeToString(gauge_parameters.GetThresholdMode()));
	query.bindValue(":image_name", gauge_parameters.GetImageFileName());

	callback(query.exec());
}

CalibrationGaugeParameters CalibrationGaugeParametersDAO::RetrieveGaugeParameters()
{
	QSqlDatabase& db = Database::GetDatabase();
	if (!db.isValid() || IsTableEmpty())
	{
		return CalibrationGaugeParameters();
	}

	QSqlQuery query(db);
	if (query.exec("SELECT * FROM calibration_gauge_parameters"))
	{
		if (query.next())
		{
			CalibrationGaugeParameters result;
			result.SetGaugeDiameter(query.value(1).toDouble())
				.SetUnitSelection(CalibrationGaugeParameters::StringToUnitSelection(query.value(2).toString()))
				.SetThresholdValue(query.value(3).toInt())
				.SetThresholdMode(CalibrationGaugeParameters::StringToThresholdMode(query.value(4).toString()))
				.SetImageFileName(query.value(5).toString());
			return result;
		}
	}

	return CalibrationGaugeParameters();
}
