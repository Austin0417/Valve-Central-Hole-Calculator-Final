#include "MeasureData.h"

#include "FileWriter.h"


double MeasureData::GetMeasuredValveArea() const
{
	return measured_valve_area_;
}

void MeasureData::SetMeasuredValveArea(const double& area)
{
	measured_valve_area_ = area;
}

std::string MeasureData::ToFileFormat() const
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
	return processed_file_name_ + ", " + std::to_string(measured_valve_area_) + " " + units_suffix + ", " + processed_at_time_ + "\n";
}

std::string MeasureData::GetFileNameType() const
{
	return FileWriter::MEASURE_AREA_HISTORY_FILENAME;
}
