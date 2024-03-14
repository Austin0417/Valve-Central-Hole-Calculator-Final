#pragma once
#include <QString>


enum class UnitSelection
{
	MILLIMETERS,
	INCHES
};

enum class ThresholdMode {
	STANDARD,
	INVERTED
};


class CalibrationGaugeParameters
{
private:
	double gauge_diameter_;
	UnitSelection unit_selection_;
	int threshold_value_;
	ThresholdMode threshold_mode_;
	QString image_file_name_;
	bool is_valid_ = true;

public:
	CalibrationGaugeParameters() : gauge_diameter_(0), unit_selection_(UnitSelection::MILLIMETERS), threshold_value_(0), threshold_mode_(ThresholdMode::STANDARD), image_file_name_(""), is_valid_(false) {}
	CalibrationGaugeParameters(double gauge_diameter, UnitSelection unit_selection, int threshold_value, ThresholdMode threshold_mode, QString image_file_name)
		: gauge_diameter_(gauge_diameter), unit_selection_(unit_selection),
		threshold_value_(threshold_value), threshold_mode_(threshold_mode), image_file_name_(image_file_name) {}


	static QString UnitSelectionToString(const UnitSelection& unit_selection);
	static QString ThresholdModeToString(const ThresholdMode& threshold_mode);
	static UnitSelection StringToUnitSelection(const QString& unit_selection);
	static ThresholdMode StringToThresholdMode(const QString& threshold_mode);

	CalibrationGaugeParameters& SetGaugeDiameter(double gauge_diameter);
	CalibrationGaugeParameters& SetUnitSelection(const UnitSelection& unit_selection);
	CalibrationGaugeParameters& SetThresholdValue(int threshold_value);
	CalibrationGaugeParameters& SetThresholdMode(const ThresholdMode& threshold_mode);
	CalibrationGaugeParameters& SetImageFileName(const QString& image_file_name);
	double GetGaugeDiameter() const;
	const UnitSelection& GetUnitSelection() const;
	int GetThresholdValue() const;
	const ThresholdMode& GetThresholdMode() const;
	const QString& GetImageFileName() const;
	bool IsValid() const;

	QString toString() const;
};

