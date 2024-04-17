#pragma once
#include <condition_variable>
#include <fstream>
#include <QWidget>
#include <QFileDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include "MeasureData.h"
#include "ui_ValveAreaHistoryWidget.h"


namespace valve_area_widget
{
	enum RowFormat
	{
		FILENAME,
		MEASURED_AREA,
		TIME,
		ID
	};
}


class ValveAreaHistoryWidget : public QWidget
{
	Q_OBJECT
private:
	std::unique_ptr<QTableWidget> history_table_;
	std::unique_ptr<QPushButton> clear_btn_;
	std::unique_ptr<QPushButton> export_csv_btn_;
	std::unique_ptr<QFileDialog> save_csv_file_dialog_;
	std::function<void()> clear_valve_area_history_callback_;
	std::unordered_map<QTableWidgetItem*, bool> sorted_mapping_;

	std::vector<MeasureData> measure_history_;

	// Multithreading data members
	std::vector<std::vector<QTableWidgetItem*>> table_items_;
	bool data_retrieval_complete = false;
	std::condition_variable cv_;
	std::mutex mutex_;

	std::unique_ptr<QMenu> table_widget_right_click_menu_;

	Ui::ValveAreaHistoryWidget ui;
	void AddRowToTable(const MeasureData& data);
	void SetTableData(const std::vector<MeasureData>& data);
	std::vector<QTableWidgetItem*> GetItemsFromRow(int row);
public:
	enum class SortOption
	{
		FILENAME_ASCENDING,
		FILENAME_DESCENDING,
		AREA_ASCENDING,
		AREA_DESCENDING,
		TIME_ASCENDING,
		TIME_DESCENDING
	};

	ValveAreaHistoryWidget(QWidget* parent = nullptr);
	void SetClearValveAreaHistoryCallback(const std::function<void()>& callback);
	void RefreshTableData();
	void ClearTable(bool exclude_first_row = false);
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

signals:
	void OnDataInitialLoadComplete();
	void OnTableRefreshComplete();
	void OnSortComplete(const std::vector<MeasureData>& sorted);
	void OnRowsDeleted();
};

