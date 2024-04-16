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

	std::function<void()> clear_calibrate_history_callback_;
	std::function<void()> clear_valve_area_history_callback_;
public:
	CalculationHistoryDialog(QWidget* parent = nullptr);
	void SetClearCalibrateHistoryCallback(const std::function<void()>& callback);
	void SetClearValveAreaHistoryCallback(const std::function<void()>& callback);
	std::unique_ptr<CalibrateHistoryWidget>& GetCalibrateHistoryWidget();
	std::unique_ptr<ValveAreaHistoryWidget>& GetValveAreaHistoryWidget();
	virtual int exec() override;
};

