#pragma once
#include <string>
#include "CalibrationGaugeParameters.h"


class ValveData
{
protected:
	std::string processed_file_name_;
	std::string processed_at_time_;
	UnitSelection units_;
	int id_;
public:
	ValveData(const std::string& name, const std::string& time, UnitSelection units) : id_(-1), processed_file_name_(name), processed_at_time_(time), units_(units) {}
	ValveData(int id, const std::string& name, const std::string& time, UnitSelection units) : id_(id), processed_file_name_(name), processed_at_time_(time), units_(units) {}
	const std::string& GetFileName() const;
	const std::string& GetTimeProcessed() const;
	const UnitSelection& GetUnits() const;
	int GetId() const;
	void SetFileName(const std::string& filename);
	void SetTimeProcessed(const std::string& time);
	void SetUnits(const UnitSelection& unit_selection);
	void SetId(int id);
	virtual std::string ToFileFormat() const;
	virtual std::string GetFileNameType() const = 0;
};

