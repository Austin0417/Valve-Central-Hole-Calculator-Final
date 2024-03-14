#include "CalibrationGaugeParameters.h"


QString CalibrationGaugeParameters::UnitSelectionToString(const UnitSelection& unit_selection)
{
	switch (unit_selection)
	{
	case UnitSelection::MILLIMETERS:
	{
		return "millimeters";
	}
	case UnitSelection::INCHES:
	{
		return "inches";
	}
	}
}

QString CalibrationGaugeParameters::ThresholdModeToString(const ThresholdMode& threshold_mode)
{
	switch (threshold_mode)
	{
	case ThresholdMode::STANDARD:
	{
		return "standard";
	}
	case ThresholdMode::INVERTED:
	{
		return "inverted";
	}
	}
}

UnitSelection CalibrationGaugeParameters::StringToUnitSelection(const QString& unit_selection)
{
	if (unit_selection == "millimeters")
	{
		return UnitSelection::MILLIMETERS;
	}
	else
	{
		return UnitSelection::INCHES;
	}
}

ThresholdMode CalibrationGaugeParameters::StringToThresholdMode(const QString& threshold_mode)
{
	if (threshold_mode == "standard")
	{
		return ThresholdMode::STANDARD;
	}
	else
	{
		return ThresholdMode::INVERTED;
	}
}


CalibrationGaugeParameters& CalibrationGaugeParameters::SetGaugeDiameter(double gauge_diameter)
{
	gauge_diameter_ = gauge_diameter;
	is_valid_ = true;
	return *this;
}


CalibrationGaugeParameters& CalibrationGaugeParameters::SetUnitSelection(const UnitSelection& unit_selection)
{
	unit_selection_ = unit_selection;
	is_valid_ = true;
	return *this;
}

CalibrationGaugeParameters& CalibrationGaugeParameters::SetThresholdValue(int threshold_value)
{
	threshold_value_ = threshold_value;
	is_valid_ = true;
	return *this;
}

CalibrationGaugeParameters& CalibrationGaugeParameters::SetThresholdMode(const ThresholdMode& threshold_mode)
{
	threshold_mode_ = threshold_mode;
	is_valid_ = true;
	return *this;
}

CalibrationGaugeParameters& CalibrationGaugeParameters::SetImageFileName(const QString& image_file_name)
{
	image_file_name_ = image_file_name;
	is_valid_ = true;
	return *this;
}

double CalibrationGaugeParameters::GetGaugeDiameter() const
{
	return gauge_diameter_;
}

const UnitSelection& CalibrationGaugeParameters::GetUnitSelection() const
{
	return unit_selection_;
}

int CalibrationGaugeParameters::GetThresholdValue() const
{
	return threshold_value_;
}

const ThresholdMode& CalibrationGaugeParameters::GetThresholdMode() const
{
	return threshold_mode_;
}

const QString& CalibrationGaugeParameters::GetImageFileName() const
{
	return image_file_name_;
}


bool CalibrationGaugeParameters::IsValid() const
{
	return is_valid_;
}

QString CalibrationGaugeParameters::toString() const
{
	return "{gauge_diameter: " + QString::number(gauge_diameter_) + ", unit_selection: "
		+ UnitSelectionToString(unit_selection_) + ", threshold_value: " + QString::number(threshold_value_) + ", threshold_mode: " + ThresholdModeToString(threshold_mode_) +
		", image_path: " + image_file_name_ + "}";
}




