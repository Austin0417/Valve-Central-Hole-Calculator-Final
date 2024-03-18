#pragma once
#include <QDialog>
#include "ui_CalculationHistoryDialog.h"
#include "CalibrateHistoryWidget.h"
#include "ValveAreaHistoryWidget.h"
#include "CalibrateData.h"
#include "MeasureData.h"


class CalculationHistoryDialog : public QDialog
{
private:
	Ui::CalculationHistoryDialog ui;
	std::unique_ptr<QTabWidget> tab_widget_;
	std::unique_ptr<CalibrateHistoryWidget> calibration_gauge_history_;
	std::unique_ptr<ValveAreaHistoryWidget> valve_area_history_;
	const std::vector<CalibrateData>& calibrate_history_;
	const std::vector<MeasureData>& measure_history_;

public:
	CalculationHistoryDialog(const std::vector<CalibrateData>& calibrate_history, const std::vector<MeasureData>& measure_history, QWidget* parent = nullptr);

};
