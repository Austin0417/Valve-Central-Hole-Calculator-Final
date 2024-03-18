#include "CalibrateData.h"

#include "FileWriter.h"


double CalibrateData::GetCalibrationFactor() const
{
	return calibration_factor_;
}

void CalibrateData::SetCalibrationFactor(const double& factor)
{
	calibration_factor_ = factor;
}

std::string CalibrateData::ToFileFormat() const
{
	std::string units_suffix;
	switch (units_)
	{
	case UnitSelection::MILLIMETERS:
	{
		units_suffix = "mm^2";
		break;
	}
	case UnitSelection::INCHES:
	{
		units_suffix = "in^2";
		break;
	}
	}
	return processed_file_name_ + ", " + std::to_string(calibration_factor_) + " " + units_suffix + ", " + processed_at_time_ + "\n";
}

std::string CalibrateData::GetFileNameType() const
{
	return FileWriter::CALIBRATE_HISTORY_FILENAME;
}
