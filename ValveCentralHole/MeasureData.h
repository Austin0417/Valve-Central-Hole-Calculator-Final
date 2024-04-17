#pragma once
#include "ValveData.h"


class MeasureData : public ValveData
{
private:
	double measured_valve_area_;
public:
	MeasureData(int id, const std::string& filename, const std::string& time, UnitSelection units, double area) : ValveData(id, filename, time, units), measured_valve_area_(area) {}
	MeasureData(const std::string& filename, const std::string& time, UnitSelection units, double area) : ValveData(filename, time, units), measured_valve_area_(area) {}
	double GetMeasuredValveArea() const;
	void SetMeasuredValveArea(const double& area);
	std::string ToFileFormat() const override;
	virtual std::string GetFileNameType() const override;
	friend std::ostream& operator<<(std::ostream& os, const MeasureData& measure_data);
};

