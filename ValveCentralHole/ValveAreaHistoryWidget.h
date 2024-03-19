#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "MeasureData.h"
#include "ui_ValveAreaHistoryWidget.h"



class ValveAreaHistoryWidget : public QWidget
{
	Q_OBJECT
private:
	enum RowFormat
	{
		FILENAME,
		MEASURED_AREA,
		TIME
	};

	std::unique_ptr<QTableWidget> history_table_;
	std::unique_ptr<QPushButton> clear_btn_;
	const std::vector<MeasureData>& measure_history_;
	std::function<void()> clear_valve_area_history_callback_;
	std::unordered_map<QTableWidgetItem*, bool> sorted_mapping_;

	Ui::ValveAreaHistoryWidget ui;
	void AddRowToTable(const MeasureData& data);
	void SetTableData(const std::vector<MeasureData>& data);
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

	ValveAreaHistoryWidget(const std::vector<MeasureData>& measure_history, QWidget* parent = nullptr);
	void SetClearValveAreaHistoryCallback(const std::function<void()>& callback);
	void ClearTable(bool exclude_first_row = false);

signals:
	void OnSortComplete(const std::vector<MeasureData>& sorted);
};

