#include "ValveData.h"


const std::string& ValveData::GetFileName() const
{
	return processed_file_name_;
}


const std::string& ValveData::GetTimeProcessed() const
{
	return processed_at_time_;
}

const UnitSelection& ValveData::GetUnits() const
{
	return units_;
}

void ValveData::SetFileName(const std::string& filename)
{
	processed_file_name_ = filename;
}

void ValveData::SetTimeProcessed(const std::string& time)
{
	processed_at_time_ = time;
}

void ValveData::SetUnits(const UnitSelection& unit_selection)
{
	units_ = unit_selection;
}


std::string ValveData::ToFileFormat() const
{
	return processed_file_name_ + ", " + processed_at_time_;
}
