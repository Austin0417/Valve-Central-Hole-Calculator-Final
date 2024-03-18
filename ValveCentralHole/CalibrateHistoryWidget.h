#pragma once
#include <QWidget>
#include "ui_CalibrateHistoryWidget.h"
#include "CalibrateData.h"
#include "MeasureData.h"


class CalibrateHistoryWidget : public QWidget
{
private:
	enum RowFormat
	{
		FILENAME,
		CALIBRATION_FACTOR,
		TIME
	};

	std::unique_ptr<QTableWidget> history_table_;
	std::unique_ptr<QPushButton> clear_btn_;
	const std::vector<CalibrateData>& calibrate_history_;
	std::function<void()> clear_calibrate_history_callback_;

	Ui::CalibrateHistoryWidget ui;

	void AddTableRow(const CalibrateData& data);
	void SetTableData();
public:
	CalibrateHistoryWidget(const std::vector<CalibrateData>& calibrate_history, QWidget* parent);
	void SetClearCalibrateHistoryCallback(const std::function<void()>& callback);
	void ClearTable();
};

