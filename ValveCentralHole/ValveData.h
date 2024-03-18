#pragma once
#include <string>
#include "CalibrationGaugeParameters.h"


class ValveData
{
protected:
	std::string processed_file_name_;
	std::string processed_at_time_;
	UnitSelection units_;
public:
	ValveData(std::string name, std::string time, UnitSelection units) : processed_file_name_(name), processed_at_time_(time), units_(units) {}
	const std::string& GetFileName() const;
	const std::string& GetTimeProcessed() const;
	const UnitSelection& GetUnits() const;
	void SetFileName(const std::string& filename);
	void SetTimeProcessed(const std::string& time);
	void SetUnits(const UnitSelection& unit_selection);
	virtual std::string ToFileFormat() const;
	virtual std::string GetFileNameType() const = 0;
};

