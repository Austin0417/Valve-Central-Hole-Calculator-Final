#pragma once
#include "ValveData.h"


class MeasureData : public ValveData
{
private:
	double measured_valve_area_;
public:
	MeasureData(std::string filename, std::string time, UnitSelection units, double area) : ValveData(filename, time, units), measured_valve_area_(area) {}
	double GetMeasuredValveArea() const;
	void SetMeasuredValveArea(const double& area);
	std::string ToFileFormat() const override;
	virtual std::string GetFileNameType() const override;
};

