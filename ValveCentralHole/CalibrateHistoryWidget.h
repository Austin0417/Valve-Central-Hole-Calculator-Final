#pragma once
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <QWidget>
#include <QFileDialog>
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
	std::unique_ptr<QPushButton> export_csv_btn_;
	std::unique_ptr<QFileDialog> save_csv_file_dialog_;
	std::function<void()> clear_calibrate_history_callback_;
	std::unordered_map<QTableWidgetItem*, bool> sort_mapping_;	// key is pointers to addresses of the QTableWidgetItem column headers, value is boolean indicating if the column is sorted in ascending order

	std::vector<CalibrateData> calibrate_history_;

	Ui::CalibrateHistoryWidget ui;

	// Multithreading data members
	std::vector<std::vector<QTableWidgetItem*>> table_items_;
	bool data_retrieval_complete = false;
	std::condition_variable cv_;
	std::mutex mutex_;

	void InitializeHistoryTable(bool is_refresh = false);
	void AddTableRow(const CalibrateData& data);
	void SetTableData(const std::vector<CalibrateData>& data, bool is_refresh = false);
	std::vector<QTableWidgetItem*> GetItemsFromRow(int row);
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

	CalibrateHistoryWidget(QWidget* parent = nullptr);
	void SetClearCalibrateHistoryCallback(const std::function<void()>& callback);
	void RefreshTableData();
	void ClearTable();

signals:
	void OnDataInitialLoadComplete();
	void OnTableRefreshComplete();
	void DisplaySortedTable(const std::vector<CalibrateData>& sorted);
};

