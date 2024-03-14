#pragma once
#include "CalibrationGaugeParameters.h"
#include "Database.h"
#include <functional>


class CalibrationGaugeParametersDAO
{
public:
	bool IsTableEmpty();
	void IsTableEmpty(const std::function<void(bool)>& callback);
	void AddGaugeParameters(const CalibrationGaugeParameters& gauge_parameters, const std::function<void(bool)>& callback);
	void UpdateGaugeParameters(const CalibrationGaugeParameters& gauge_parameters, const std::function<void(bool)>& callback);
	CalibrationGaugeParameters RetrieveGaugeParameters();
};

