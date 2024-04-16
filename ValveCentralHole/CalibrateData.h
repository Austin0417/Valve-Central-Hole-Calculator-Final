#pragma once
#include "ValveData.h"


class CalibrateData : public ValveData
{
private:
	double calibration_factor_;
public:
	CalibrateData(std::string filename, std::string time, UnitSelection units, double calibration_factor) : ValveData(filename, time, units), calibration_factor_(calibration_factor) {}
	double GetCalibrationFactor() const;
	void SetCalibrationFactor(const double& factor);
	std::string ToFileFormat() const override;
	std::string GetFileNameType() const override;
	friend std::ostream& operator<<(std::ostream& os, const CalibrateData& data);
};

