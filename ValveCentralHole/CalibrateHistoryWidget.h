#pragma once
#include <QWidget>
#include "ui_CalibrateHistoryWidget.h"
#include "CalibrateData.h"
#include "MeasureData.h"


class CalibrateHistoryWidget : public QWidget
{
	Q_OBJECT
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
	std::unordered_map<QTableWidgetItem*, bool> sort_mapping_;	// key is pointers to addresses of the QTableWidgetItem column headers, value is boolean indicating if the column is sorted in ascending order

	Ui::CalibrateHistoryWidget ui;

	void InitializeHistoryTable(bool is_refresh = false);
	void AddTableRow(const CalibrateData& data);
	void SetTableData(const std::vector<CalibrateData>& data, bool is_refresh = false);
public:
	enum class SortOption
	{
		FILENAME_ASCENDING,
		FILENAME_DESCENDING,
		CALIBRATION_ASCENDING,
		CALIBRATION_DESCENDING,
		TIME_ASCENDING,
		TIME_DESCENDING
	};

	CalibrateHistoryWidget(const std::vector<CalibrateData>& calibrate_history, QWidget* parent);
	void SetClearCalibrateHistoryCallback(const std::function<void()>& callback);
	void ClearTable();

signals:
	void DisplaySortedTable(const std::vector<CalibrateData>& sorted);
};

